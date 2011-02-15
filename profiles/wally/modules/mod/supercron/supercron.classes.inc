<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
*/

/**
 * Description of supercronclass
 *
 * @author louis-eric
 */

function _supercron_parallel_call($url) {
    $parts = parse_url($url);
    $u     = $parts['path'] .'?'. $parts['query'];
    $port  = isset($parts['port']) ? $parts['port'] : 80;
    $fp    = fsockopen($parts['host'], $port, $errno, $errstr, 20);
    if ($fp) {
        $out = "GET {$u} HTTP/1.1\r\n";
        $out .= "Host: {$parts['host']}\r\n";
        $out .= "Connection: Close\r\n\r\n";
        fwrite($fp, $out);

        stream_set_blocking($fp, FALSE);
        stream_set_timeout($fp, 2);
        fclose($fp);
    }
}

class supercron_module_collection {

    const drupal_cron_last = 'cron_last';
    const drupal_cron_semaphore = 'cron_semaphore';

    public $modules = array();

    function  __construct() {

        $this->update();
    }

    public function update() {
// Load module list from the database and see if any uninstalled or disabled modules need to be removed
        $modulenames = db_query('SELECT * FROM {supercron_enabled_mods} ORDER BY module_name ASC');
        while ($row = db_fetch_object($modulenames)) {
            if (module_hook($row->module_name, 'cron'))
                $this->modules[$row->module_name] = new supercron_module($row->module_name, $row->weight, $row->detached, $row->enabled, $row->rule, $row->timestamp);
            else
                db_query('DELETE FROM {supercron_enabled_mods} WHERE module_name = \'%s\'', $row->module_name);

        }

// Add newly installed modules to the list
        $cron_enabled_modules = self::get_list_of_cron_enabled_modules();

        $max_weight = db_result(db_query("SELECT MAX(weight) FROM {supercron_enabled_mods}"));
        if ($max_weight == NULL) $max_weight = 0; else $max_weight++;

        foreach ($cron_enabled_modules as $modulename) {
            if (!isset($this->modules[$modulename])) {
                db_query(
                        "INSERT INTO {supercron_enabled_mods} (module_name, weight, output) VALUES ('%s', %d, '')",
                        $modulename, $max_weight
                );

                $this->modules[$modulename] = new supercron_module($modulename, $max_weight);

                $max_weight++;
            }
        }
    }

    static public function get_list_of_cron_enabled_modules() {
        return module_implements('cron', TRUE);
    }

    private function is_empty() {
        if (empty($this->modules)) return TRUE;
        else {
            $count = 0;
            foreach ($this->modules as $module) if ($module->is_enabled()) $count++;
            return $count === 0;
        }
    }

    protected function record_last_execution_time() {
        variable_set(self::drupal_cron_last, time());
        watchdog('cron', 'Cron run completed.', array(), WATCHDOG_NOTICE);
    }

    protected function capture_global_cron_semaphore() {
        variable_set(self::drupal_cron_semaphore, time());
    }

    protected function release_global_cron_semaphore() {
        variable_del(self::drupal_cron_semaphore);
    }

    public static function compare_weight($module1, $module2) {
        if ($module1->weight == $module2->weight)
            return 0;
        return ($module1->weight < $module2->weight) ? -1 : 1;
    }

    protected function clear_logs() {
        if (supercron_registry()->logs_must_be_cleared()) {
            db_query(
                    "DELETE FROM {watchdog} WHERE type = 'cron' AND severity = %d",
                    WATCHDOG_NOTICE
            );
        }
    }

    public function crontab_invoke_specific_module($module_name) {
        if (isset ($this->modules[$module_name])) return $this->modules[$module_name]->invoke();
        else return FALSE;
    }

    public function invoke_specific_module($module_name, $forced_multithreading = FALSE) {
        if (isset ($this->modules[$module_name])) return $this->modules[$module_name]->invoke();
        else return FALSE;
    }

    public function straight_invoke_specific_module($module_name) {
        if (isset ($this->modules[$module_name])) return $this->modules[$module_name]->straight_invoke();
        else return FALSE;
    }

    public function invoke_all() {

        $this->clear_logs();

        if ($this->is_empty()) {
            $this->record_last_execution_time();
            return FALSE;
        }

        $this->capture_global_cron_semaphore();

// Get list of ordered modules, invoke each in turn
        $call_list = array();
        foreach ($this->modules as $module)
            if ($module->is_enabled()) $call_list[] = $module;
        usort($call_list, "supercron_module_collection::compare_weight");

        foreach ($call_list as $module) $module->invoke();

        $this->record_last_execution_time();
        $this->release_global_cron_semaphore();

    }

}

class supercron_module {

    public  $module_name = '';
    private $multithread = FALSE;
    private $enabled = TRUE;
    public  $weight = -1;
    public  $scheduling_rule = '';
    private $last_run = -1;

    function  __construct($name, $weight, $multithread = FALSE, $enabled = TRUE, $rule = '', $timestamp = -1) {
        $this->module_name = $name;
        $this->multithread = $multithread;
        $this->weight = $weight;
        $this->enabled = $enabled;
        $this->scheduling_rule = $rule;
        $this->last_run = $timestamp;
    }

    protected function record_execution_timings($start_time, $end_time) {
        $interval = $end_time - $start_time;

        db_query(
                "UPDATE {supercron_enabled_mods} "
                . "SET timestamp = %d,"
                . " last_exec_interval = %d,"
                . " total_exec_interval = total_exec_interval + %d,"
                . " times_count = times_count + 1"
                . " WHERE module_name = '%s'",
                $start_time, $interval, $interval, $this->module_name
        );
    }

    static function is_rule_valid($rule) {
// check for valid rule format
        $format = '([0-9*,/-]+)';
        $format = "@^{$format}\s+{$format}\s+{$format}\s+{$format}\s+{$format}$@";
        if (!preg_match($format, $rule, $rules)) {
            return FALSE;
        }

        return $rules;
    }

    static function decode_rule($rule, $min, $max) {
        if ($rule == '*')
            return range($min, $max);

        $result = array();
        foreach (explode(',', $rule) as $token) {
            if (preg_match('/^([0-9]+)-([0-9]+)$/', $token, $r)) {
                $result = array_merge($result, range($r[1], $r[2]));
            }
            elseif (preg_match('/^\*\/([0-9]+)$/', $token, $r)) {
                for ($i = $min;
                $i <= $max;
                $i++)
                    if ($i % $r[1] == 0)
                        $result[] = $i;
            }
            elseif (is_numeric($token)) {
                $result[] = $token;
            }
        }
        return $result;
    }

    static function t_calc($time, $d = -1, $h = -1, $m = -1) {
        if ($d < 0) {
            return date('n', $time) * 31 * 24 * 60 + date('j', $time) * 24 * 60 + date('H', $time) * 60 + date('i', $time);
        }
        else {
            return $time * 31 * 24 * 60 + $d * 24 * 60 + $h * 60 + $m;
        }
    }

    static function week_days($year, $mon, $weekdays) {
        $result = array();
        for ($i = 1;
        checkdate($mon, $i, $year);
        $i++) {
            $w = date('w', mktime(12, 00, 00, $mon, $i, $year));
            if (in_array($w, $weekdays)) {
                $result[] = $i;
            }
        }
        return $result;
    }

    public function is_enabled() {
        return $this->enabled;
    }

    protected function is_scheduled() {

        if (TRUE === empty($this->rule))
            return TRUE;


        $now = $_SERVER['REQUEST_TIME'];

// job never run or run one year ago
        if ((!$this->last_run) || ($now - $this->last_run > 365 * 86400)) {
            return TRUE;
        }

// check for valid rule format
        $rules = $this->is_rule_valid($this->rule);
        if (FALSE === $rules) {
            watchdog('cron', 'Invalid rule found (%module): %rule', array(
                    '%module' => $this->module_name,
                    '%rule'   => $this->rule
            ));

            return FALSE;
        }

        $weekdayspec = ($rules[5] != '*');
        $mondayspec  = ($rules[3] != '*');

        $rules[5] = self::decode_rule($rules[5], 0, 6);
        $rules[4] = self::decode_rule($rules[4], 1, 12);
        $rules[3] = self::decode_rule($rules[3], 1, 31);
        $rules[2] = self::decode_rule($rules[2], 0, 23);
        $rules[1] = self::decode_rule($rules[1], 0, 59);

        $last_t     = self::t_calc($this->last_run + 30);
        $now_t      = self::t_calc($now);
        $now_t_delta = $now_t - $last_t + ($last_t > $now_t ? 12 * 31 * 24 * 60 : 0);
        $year      = date('Y', $this->last_run);

        if ($mondayspec || (!$mondayspec && !$weekdayspec)) {
            $first = -1;

            foreach ($rules[4] as $mon) {
                foreach ($rules[3] as $d) {
                    if (checkdate($mon, $d, $year)) {
                        foreach ($rules[2] as $h) {
                            foreach ($rules[1] as $m) {
                                $t = self::t_calc($mon, $d, $h, $m);

                                if ($first < 0) {
                                    $first = $t;
                                }

                                if ($t > $last_t) {
                                    $next_t = $t;
                                    break 4;
                                }
                            }
                        }
                    }
                }
            }

            if (!$next_t) {
                $next_t = $first;
            }

            $next_t_delta = $next_t - $last_t + ($last_t > $next_t ? 12 * 31 * 24 * 60 : 0);

            if ($now_t_delta >= $next_t_delta) {
                return TRUE;
            }
        }

        if ($weekdayspec) {
            foreach ($rules[4] as $mon) {
                foreach (self::week_days($year, $mon, $rules[5]) as $d) {
                    foreach ($rules[2] as $h) {
                        foreach ($rules[1] as $m) {
                            $t = self::t_calc($mon, $d, $h, $m);

                            if ($t > $last_t) {
                                $next_t = $t;
                                break 4;
                            }
                        }
                    }
                }
            }

            if (!$next_t) {
// ust get the first of following year (if day_of_week is specified it's
// not the same of previous one)
                foreach ($rules[4] as $mon) {
                    foreach (self::week_days($year + 1, $mon, $rules[5]) as $d) {
                        foreach ($rules[2] as $h) {
                            foreach ($rules[1] as $m) {
                                $next_t = _self::t_calc($mon, $d, $h, $m);
                                break 4;
                            }
                        }
                    }
                }
            }

            $next_t_delta = $next_t - $last_t + ($last_t > $next_t ? 12 * 31 * 24 * 60 : 0);

            if ($now_t_delta >= $next_t_delta) {
                return TRUE;
            }
        }

        return FALSE;

    }


    private function log_output($output) {
        db_query(
                "UPDATE {supercron_enabled_mods}"
                . " SET output = Concat(output, '%s<br />')"
                . " WHERE module_name = '%s'",
                $output, $this->module_name
        );
    }

    public function get_call_path() {
        return supercron_registry()->get_controller_script_path() . '?safety=' . urlencode(supercron_registry()->get_safety_variable()) . '&module=' . urlencode($this->module_name);
    }

    public function is_executing() {
        $state = db_fetch_object(db_query("SELECT executing FROM {supercron_enabled_mods} WHERE module_name = '%s'", $this->module_name));
        if ($state)
            return (bool)$state->executing; else return FALSE;

    }

    public function set_executing($state = TRUE) {
        db_query(
                "UPDATE {supercron_enabled_mods}"
                . " SET executing = %d"
                . " WHERE module_name = '%s'",
                (bool)$state, $this->module_name
        );
    }

    public function straight_invoke() {
        if (!$this->is_executing()) {
            try {
                ob_start();
                $this->set_executing(TRUE);
                $start_time = time();
                module_invoke($this->module_name, 'cron');
                $this->record_execution_timings($start_time, time());
                $this->set_executing(FALSE);
                $output = ob_get_contents();
                ob_end_clean();
                if (!empty($output))
                    $this->log_output($output);
            }
            catch (Exception $e) {
                ob_end_clean();
                $this->set_executing(FALSE);
                $message = $e->getMessage();
                watchdog(
                        'cron exception',
                        t("Module name: @module;\nException message: @message;"),
                        array(
                        '@module'  => $this->module_name,
                        '@message' => !empty($message) ? $message : t('no message')
                        ),
                        WATCHDOG_ERROR
                );
            }
        }
        else {
            watchdog('SuperCron', 'SuperCron prevented the execution of the !module cron hook as it was already running.', array('!module' => $this->module_name), WATCHDOG_NOTICE);
            return FALSE;

        }
    }

    public function crontab_invoke() {
        if (!$this->is_enabled()) return FALSE;
        if (!$this->is_scheduled()) return FALSE;
        $this->straight_invoke();
    }

    public function invoke($forced_multithreading = FALSE, $forced_execution = FALSE) {
        if (!$forced_execution) {
            if (!$this->is_enabled()) return FALSE;
            if (!$this->is_scheduled()) return FALSE;
        }

        if ($this->multithread OR $forced_multithreading) {
            $url = str_replace('%', $this->module_name, supercron_registry()->get_external_invocation_url());
            $url = url($url,array(absolute => TRUE));
            _supercron_parallel_call($url);
        }
        else {
            $this->straight_invoke();
        }

    }

}

class supercron_firewall_class {

    const firewall_mode_exclusive = 'only';
    const firewall_mode_inclusive = 'except';

    static function is_valid_ip($ip) {
        // Pattern taken from http://www.blog.highub.com/regular-expression/php-regex-regular-expression/php-regex-validate-ip-address/

        return preg_match( "/^(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/", $input);
    }

    function ip_is_in_list($ip) {
        $result = db_query('SELECT COUNT(*) AS rescount FROM {supercron_ips} WHERE ip = "%s"', $ip);
        if ($result) {
            $row = db_fetch_object($result);
            return ((int)$row->rescount > 0);
        }
        else
            return FALSE;
    }

    function add_ip($ip) {
        $return = FALSE;
        if (!$this->ip_is_in_list($ip)) {
            $ip_record = new stdClass();
            $ip_record->ip = $ip;
            $return = (drupal_write_record('supercron_ips', $ip_record));
            if ($return) watchdog('SuperCron Firewall', 'IP !ip added.', array('!ip' => $ip), WATCHDOG_NOTICE);
        }
        return $return;
    }

    function remove_ip($ip) {
        $return = (($this->ip_is_in_list($ip)) AND db_query('DELETE FROM {supercron_ips} WHERE ip = "%s"', $ip));
        if ($return) watchdog('SuperCron Firewall', 'IP !ip deleted.', array('!ip' => $ip), WATCHDOG_NOTICE);
        return $return;
    }

    function get_local_ips() {

        $ips = gethostbynamel($_SERVER['HTTP_HOST']);
        if (empty($ips)) $ips = array();

        if (!in_array('127.0.0.1', $ips)) $ips[] = '127.0.0.1';


    }


}

class supercron_registry_class {

    const prefs_clear_logs = 'supercron_dblog_clear_messages';
    const prefs_safety_variable = 'supercron_safety';
    const prefs_autocron_enabled = 'supercron_autocron_enabled';
    const prefs_autocron_autoenabled = 'supercron_autocron_auto_enabled';
    const prefs_autocron_url = 'supercron_autocron_url';
    const prefs_autocron_treshold = 'supercron_autocron_treshold';
    const prefs_autocron_autoenabled_treshold = 'supercron_autocron_auto_enabled_treshold';
    const prefs_external_invocation_url = 'supercron_external_invocation_url';
    const prefs_firewall_enabled = 'supercron_firewall_enabled';
    const prefs_firewall_mode = 'supercron_firewall_mode';
    const prefs_php_binary_location = 'supercron_php_binary_path';
    const prefs_controller_path = 'supercron_script_path';
    const prefs_controller_custom_path = 'supercron_script_path_text';
    const prefs_throttle_sensitive = 'supercron_respond_to_throttling';
    const prefs_exceptions_feed_url = 'supercron_exceptions_RSS_url';

    const controller_path_is_drupal_root = 1;
    const controller_path_is_module_root = 2;
    const controller_path_is_custom = 3;


    const controller_name = 'supercron.php';

    const placeholder_safety_token = '@safety';

    const menu_path_autocron = 'supercron/run-autocron-';
    const menu_path_external_invocation = 'supercron/invoke/@safety/%';

    const prefs_default_autocron_autoenabled_treshold_in_hours = 12; // 12 hours
    const exceptions_feed_url_prefix = 'supercron/exceptions';


    const drupal_cron_last = 'cron_last';


    private $safety_variable = NULL;
    public  $module_location = NULL;

    function  __construct() {
        $this->get_safety_variable();
        $this->module_location = dirname(__FILE__);
    }

    function get_RSS_URL() {
        $url = variable_get(self::prefs_exceptions_feed_url, '');

        if (empty($url)) {
            // We are purposefully not using get_safety_variable() here as this URL can be published
            // elsewhere (such as web-based feed readers) in a way that would risk compromising the safety variable
            $url = self::exceptions_feed_url_prefix . '/' . $this->make_safety_token() . '.rss';
            variable_set(self::prefs_exceptions_feed_url, $url);
        }

        return $url;
    }


    static function get_clear_logs() {
        return variable_get(self::prefs_clear_logs, FALSE);
    }

    static function set_clear_logs($value) {
        variable_set(self::prefs_clear_logs, (bool)$value);
    }


    static function logs_must_be_cleared() {
        return module_exists('dblog') ? self::get_clear_logs() : TRUE;
    }



    static function set_php_binary_location($value) {
        $value = trim($value);
        if (strlen($value) > 0) variable_set(self::prefs_php_binary_location, $value);
    }

    function get_php_binary_path() {
        $location = variable_get(self::prefs_php_binary_location, '');
        if (!$location) $alternative_location = isset($_SERVER['PHP_PEAR_PHP_BIN']) ? $_SERVER['PHP_PEAR_PHP_BIN'] : '';
        if (!$exe AND !$alternative_location) $alternative_location = isset($_SERVER['_']) ? $_SERVER['_'] : '';
        if (isset($alternative_location)) {
            $this->set_php_binary_location($alternative_location);
            $location = $alternative_location;
        }
        return $location;
    }

    static function  get_php_binary_path_description_placeholder() {
        return t('[path to php binary]');
    }

    function get_php_binary_path_description(&$location_known = TRUE) {
        $result = $this->get_php_binary_path();
        if (!$result) {
            $result = self::get_php_binary_path_description_placeholder();
            $location_known = FALSE;
        }
        return $result;
    }

    static function get_last_cron_execution() {
        return variable_get(self::drupal_cron_last, 0);
    }

    function is_autocron_enabled() {
        return variable_get(self::prefs_autocron_enabled, FALSE);
    }

    function is_autocron_autoenabled() {
        return variable_get(self::prefs_autocron_autoenabled, TRUE);
    }

    static function get_autocron_autoenabled_default_treshold() {
        return 60*60*(int)self::prefs_default_autocron_autoenabled_treshold_in_hours; // In seconds
    }

    function get_autocron_autoenabled_treshold() {
        return variable_get(self::prefs_autocron_autoenabled_treshold, $this->get_autocron_autoenabled_default_treshold()); // In seconds
    }

    function set_autocron_to_enabled($value) {
        variable_set(self::prefs_autocron_enabled, (bool)$value);
    }

    function toggle_autocron() {
        $this->set_autocron_to_enabled(!$this->is_autocron_enabled());
    }

    function set_autocron_to_autoenabled($value) {
        variable_set(self::prefs_autocron_autoenabled, (bool)$value);
    }

    function execute_autocron() {
        if (!module_exists('poormanscron')) {
            $time_since_last_run = (time() - $this->get_last_cron_execution());
            $execute = ($this->is_autocron_autoenabled() AND ($time_since_last_run >= $this->get_autocron_autoenabled_treshold()));
            if (!$execute) $execute = ($this->is_autocron_enabled() AND ($time_since_last_run >= $this->get_autocron_treshold()));
            if ($execute) _supercron_parallel_call(url($this->get_autocron_url(), array('absolute'=>TRUE)));
        }
    }

    function is_throttle_sensitive() {
        return variable_get(self::prefs_throttle_sensitive, FALSE);
    }

    function should_be_throttled() {
        return
                module_exists('throttle')
                && $this->is_throttle_sensitive()
                && throttle_status();
    }

    function set_throttle_sensitivity($value) {
        variable_set(self::prefs_throttle_sensitive, (bool)$value);
    }

    function is_firewall_enabled() {
        return variable_get(self::prefs_firewall_enabled, FALSE);
    }

    function set_firewall_enablement($value) {
        variable_set(self::prefs_firewall_enabled, (bool)$value);
    }

    function toggle_firewall() {
        $this->set_firewall_enablement(!$this->is_firewall_enabled());
    }

    function get_firewall_mode() {
        return variable_get(self::prefs_firewall_mode, supercron_firewall_class::firewall_mode_exclusive);
    }

    function set_firewall_mode($value) {
        $changed = FALSE;
        if (($value == supercron_firewall_class::firewall_mode_exclusive) OR ($value == supercron_firewall_class::firewall_mode_inclusive)) {
            // We do not want another conditional here to check whether the value has changed as there is
            // a small chance that the new value would be replacing a corrupted value in the DB
            $original_value = $this->get_firewall_mode();

            variable_set(self::prefs_firewall_mode, $value);
            $changed = $value != $original_value;
        }
        return $changed;

    }

    // This should be moved to a more generic firewall class, part of ongoing cleanup
    function is_allowed_by_firewall($ip) {
        if ($this->is_firewall_enabled())
        {
        if ($this->get_firewall_mode() == 'only') return supercron_firewall()->ip_is_in_list($ip);
        else return (!supercron_firewall()->ip_is_in_list($ip));
        }
        else return TRUE;
    }


    static function clear_registry() {
        variable_del(self::prefs_autocron_autoenabled);
        variable_del(self::prefs_autocron_autoenabled_treshold);
        variable_del(self::prefs_autocron_enabled);
        variable_del(self::prefs_autocron_treshold);
        variable_del(self::prefs_autocron_url);
        variable_del(self::prefs_clear_logs);
        variable_del(self::prefs_controller_path);
        variable_del(self::prefs_controller_custom_path);
        variable_del(self::prefs_exceptions_feed_url);
        variable_del(self::prefs_external_invocation_url);
        variable_del(self::prefs_firewall_enabled);
        variable_del(self::prefs_firewall_mode);
        variable_del(self::prefs_php_binary_location);
        variable_del(self::prefs_safety_variable);
        variable_del(self::prefs_throttle_sensitive);
    }

// based on Kyle Florence's code at http://php.net/manual/en/function.rand.php
// TODO:downgrade to private dynamic when done
    static function make_random_string($length = 128, $chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()_+=-[]{}\|\'"<>,.?/') {
        mt_srand(); // not necessary, except when it is
// Length of character list
        $chars_length = (strlen($chars) - 1);

// Start our string
        $string = $chars{mt_rand(0, $chars_length)};

// Generate random string
        for ($i = 1;
        $i < $length;
        $i = strlen($string)) {
// Grab a random character from our list
            $r = $chars{rand(0, $chars_length)};

// Make sure the same two characters don't appear next to each other
            if ($r != $string{$i - 1}) $string .=  $r;
        }

// Return the string
        return $string;
    }

    static function make_safety_token() {
// TODO:downgrade to private dynamic when done
        return md5(self::make_random_string() . '-' .rand() .'-'. mt_rand() . '-' . self::make_random_string()); // yes, we are a tad paranoid
    }

    public function get_safety_variable() {
        if (is_null($this->safety_variable)) {
            $value = variable_get(self::prefs_safety_variable, '');
            if (empty($value)) {
                $value = self::make_safety_token();
                variable_set(self::prefs_safety_variable, $value);
            }
            $this->safety_variable = $value;
        }

        return $this->safety_variable;
    }

    static function is_valid_safety($candidate) {
        return $candidate === self::get_safety_variable();
    }

    function get_external_invocation_url() {
        $url = variable_get(self::prefs_external_invocation_url, '');

        if (empty($url)) {
            $url = str_replace(self::placeholder_safety_token, $this->get_safety_variable(), self::menu_path_external_invocation);
            variable_set(self::prefs_external_invocation_url, $url);
        }

        return $url;
    }

    function get_autocron_url() {
        $url = variable_get(self::prefs_autocron_url, '');

        if (empty($url)) {
            $url = self::menu_path_autocron . $this->get_safety_variable();
            variable_set(self::prefs_autocron_url, $url);
        }

        return $url;
    }

    static function get_autocron_treshold() {
        return variable_get(self::prefs_autocron_treshold, 180);
    }

    static function set_autocron_treshold($value) {
        variable_set(self::prefs_autocron_treshold, (int)$value);
    }


    static function get_script_path_value() {
        return (int)variable_get(self::prefs_controller_path, self::controller_path_is_module_root);
    }

    static function set_script_path_value($type) {
        $type = (int)$type;
        if (!in_array($type, array(self::controller_path_is_drupal_root, self::controller_path_is_module_root, self::controller_path_is_custom)))
            variable_set(self::prefs_controller_path, self::controller_path_is_module_root);
        else
            variable_set(self::prefs_controller_path, $type);
    }

    static function get_base_path() {
        global $base_path;

        return str_replace('/index.php', '', $_SERVER['SCRIPT_FILENAME']) . $base_path;
    }

    public function check_script_path(&$message) {

        $return_value = TRUE;
        $path = $this->get_controller_script_path();

        if (!file_exists($path)) {
            $message = t(
                    'The !script file should copied from the !here directory to !path; until'
                    . ' that is done, your cron-driven tasks will not execute when scheduled to.',
                    array(
                    '!script' => self::controller_name,
                    '!here'   => $this->get_path_to_module_directory(),
                    '!path'   => $path
                    )
            );
            $return_value = FALSE;

        }
        return $return_value;
    }

    static function get_standard_controller_path() {
        return self::get_base_path() . self::controller_name;

    }

    function get_module_controller_path() {
        return $this->get_path_to_module_directory() . self::controller_name;
    }

    function get_path_to_module_directory() {
        return $this->module_location . '/';
    }

    static function get_custom_controller_path() {
        return variable_get(self::prefs_controller_custom_path, '');
    }

    static function set_custom_controller_path($location) {
        variable_set(self::prefs_controller_custom_path, trim($location));
    }

    static function set_controller_path_directory($type, $location = '') {
        self::set_custom_controller_path($location);
        self::set_script_path_value($type);
    }

    function get_script_URL() {
        global $base_url;
        switch (self::get_script_path_value()) {
            case self::controller_path_is_drupal_root: return $base_url .'/'. self::controller_name;
            case self::controller_path_is_module_root: return url($this->module_location . self::controller_name, array('absolute' => TRUE));
            case self::controller_path_is_custom:
            default: return self::get_custom_controller_path();
        }
    }

    static function get_controller_script_path() {
        switch (self::get_script_path_value()) {
            case self::controller_path_is_drupal_root: return self::get_standard_controller_path();
            case self::controller_path_is_module_root: return str_replace('supercron.classes.inc', '', realpath(__FILE__)) . self::controller_name;
            case self::controller_path_is_custom:
            default: return self::get_custom_controller_path();
        }
    }

}

// Simpleton model instance factories
$supercron_registry_holder = NULL;
function supercron_registry() {
    global $supercron_registry_holder;
    if (is_null($supercron_registry_holder)) $supercron_registry_holder = new supercron_registry_class();
    return $supercron_registry_holder;
}

$supercron_modules_holder = NULL;
function supercron_modules() {
    global $supercron_modules_holder;
    if (is_null($supercron_modules_holder)) $supercron_modules_holder = new supercron_module_collection();
    return $supercron_modules_holder;
}

$supercron_firewall_holder = NULL;
function supercron_firewall() {
    global $supercron_firewall_holder;
    if (is_null($supercron_firewall_holder)) $supercron_firewall_holder = new supercron_firewall_class();
    return $supercron_firewall_holder;
}
