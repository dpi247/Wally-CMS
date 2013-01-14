<?php

module_load_include('inc', 'views', 'plugins/views_plugin_cache');
module_load_include('inc', 'views', 'plugins/views_plugin_cache_time');

/**
 * Caching of query results and datas for Views displays.
 * The caching is based on time and the format argument value.
 */
class wallyflowpublish_views_plugin_cache_format extends views_plugin_cache_time {

  function get_output_key() {
    global $user;
    if (!isset($this->_output_key)) {
      $key_data = array(
        'result' => $this->view->result,
        'roles' => array_keys($user->roles),
        'super-user' => $user->uid == 1, // special caching for super user.
        'theme' => $GLOBALS['theme'],
        'language' => $GLOBALS['language'],
        'format' => $_GET['format'] ? $_GET['format'] : '',
      );
  
      $this->_output_key = $this->view->name . ':' . $this->display->id . ':output:' . md5(serialize($key_data));
    }
  
    return $this->_output_key;
  }

}
