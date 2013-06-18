<?php

/**
 * @file DAMSearchQuery.inc
 *
 * Provide the DAMSearchQuery class declaration
 * The DAMSearchQuery object is used to build a research query for the DAM
 */
class DAMSearchQuery {
  private $query;
  private $options;
  private $t;
  private $JSONQuery;
  private $isAdvanced;
  private $isBuilt;
  private $errorLogs;

  /**
   * Constructor
   */
  function DAMSearchQuery($profile) {
    $this->query = array(
      'map' => array(
        'q' => array(),
      ),
    );
    $this->options = array();
    $this->t = array();
    $this->JSONQuery = '';
    $this->isAdvanced = FALSE;
    $this->isBuilt = FALSE;
    $this->errorLogs = array();

    $this->initQuery($profile);
  }

  /**
   * Get the error logs
   *
   * @return array|NULL
   *   The error log if not empty, NULL otherwise
   */
  public function getErrorLogs() {
    if (!empty($this->errorLogs)) {
      return $this->errorLogs;
    }
    return NULL;
  }

  /**
   * Is the query built or not?
   *
   * @return boolean
   *   Is the query built or not
   */
  public function isBuilt() {
    return $this->isBuilt;
  }

  /**
   * Is this an advanced query or not?
   *
   * @return boolean
   *   Is the query advanced or not
   */
  public function isAdvanced() {
    return $this->isAdvanced;
  }

  /**
   * Get the JSON encoded query
   *
   * @return string
   *   The JSON encoded query
   */
  public function getJSONQuery() {
    return $this->JSONQuery;
  }

  /**
   * Set a query parameter
   *
   * @param string $key
   *   The parameter key
   * @param mixed $value
   *   The value to set
   *
   * @return boolean
   *   TRUE
   */
  public function setQueryParameter($key, $value) {
    $this->query['map'][$key] = $value;
    return TRUE;
  }

  /**
   * Get a query parameter
   *
   * @param string $key
   *   The requested parameter's key
   *
   * @return mixed|NULL
   *   The value if found, NULL otherwise
   */
  public function getQueryParamter($key) {
    return isset($this->query['map'][$key]) ? $this->query['map'][$key] : NULL;
  }

  /**
   * Set an option for this query
   *
   * @param string $key
   *   The option key
   * @param mixed $value
   *   The value to set
   *
   * @return boolean
   *   TRUE
   */
  public function setOption($key, $value) {
    $this->options[$key] = $value;
    return TRUE;
  }

  /**
   * Get an option for this query
   *
   * @param string $key
   *   The requested option's key
   *
   * @return mixed|NULL
   *   The value if found, NULL otherwise
   */
  public function getOption($key) {
    return isset($this->options[$key]) ? $this->options[$key] : NULL;
  }

  /**
   * Convert the query array to JSON
   *
   * @return boolean|NULL
   *   TRUE in case of success, NULL otherwise
   */
  public function buildQuery() {
    if ($this->isValidQuery()) {
      $tmp_query = $this->query;
      if ($this->isAdvanced) {
        // Convert searched words array to a string
        $tmp_query['map']['allOf'] = array(implode(',', $tmp_query['map']['allOf']));
        $tmp_query['map']['noneOf'] = array(implode(',', $tmp_query['map']['noneOf']));
      }
      $full_query = array(
        'query' => $tmp_query,
        'options' => $this->options,
        't' => $this->t,
      );
      $this->JSONQuery = json_encode($full_query);
      $this->isBuilt = TRUE;
      return TRUE;
    } else {
      $this->setErrorLog('building', t('Invalid query'));
    }
    return NULL;
  }

  /**
   * Add a word to the query
   *
   * @param string $word
   *   The word to add
   *
   * @return boolean|NULL
   *   TRUE in case of success, NULL otherwise
   */
  public function addReqWord($word) {
    if (is_string($word)) {
      if ($this->isAdvanced) {
        if (!in_array($word, $this->query['map']['allOf'])) {
          $this->query['map']['allOf'][] = $word;
          return TRUE;
        }
      } else {
        $this->query['map']['q'] = array($word);
        return TRUE;
      }
    } else {
      $this->setErrorLog('add request word', t('Given word is not a string'));
    }
    return NULL;
  }

  /**
   * Exclude a word from the query
   *
   * @param string $word
   *   The word to exclude
   *
   * @return boolean|NULL
   *   TRUE in case of success, NULL otherwise
   */
  public function excludeReqWord($word) {
    if (is_string($word)) {
      if ($this->isAdvanced) {
        if (!in_array($word, $this->query['map']['noneOf'])) {
          $this->query['map']['noneOf'][] = $word;
          return TRUE;
        }
      } else {
        $this->setErrorLog('exclude request word', t('Unable to exclude word for a simple query'));
      }
    } else {
      $this->setErrorLog('exclude request word', t('Given word is not a string'));
    }
    return NULL;
  }

  /**
   * Switch between simple and avanced modes
   *
   * @param boolean $val
   *   TRUE to switch to advanced, FALSE to switch to simple
   *
   * @return boolean|NULL
   *   TRUE in case of success, NULL otherwise
   */
  public function setIsAdvanced($val) {
    if (is_bool($val) && ($val !== $this->isAdvanced) || is_null($this->getOption('advanced'))) {
      if ($val) {
        $this->setOption('advanced', '1');
        $this->isAdvanced = TRUE;
        $this->query['map']['allOf'] = array();
        $this->query['map']['noneOf'] = array();
        if (isset($this->query['map']['q'])) {
          if (!empty($this->query['map']['q'])) {
            // Convert searched words to advanced query
            $this->query['map']['allOf'][] = $this->query['map']['q'];
          }
          unset($this->query['map']['q']);
        }
      } else {
        $this->unsetOption('advanced');
        $this->isAdvanced = FALSE;
        $this->query['map']['q'] = '';
        if (isset($this->query['map']['allOf'])) {
          if (!empty($this->query['map']['allOf'])) {
            // Convert searched words to simple query
            $this->query['map']['q'] = array(array_shift($this->query['map']['allOf']));
          }
          unset($this->query['map']['allOf']);
        }
        if (isset($this->query['map']['noneOf'])) {
          unset($this->query['map']['noneOf']);
        }
      }
      return TRUE;
    } else {
      $this->setErrorLog('set advanced', t('Unable to set advanced, not a boolean or nothing to do'));
    }
    return NULL;
  }

  /**
   * Unset an option for this query
   *
   * @param string $key
   *   The option to unset
   *
   * @return boolean|NULL
   *   TRUE in case of success, NULL otherwise
   */
  public function unsetOption($key) {
    if (isset($this->options[$key])) {
      unset($this->options[$key]);
      return TRUE;
    }
    return NULL;
  }

  /**
   * Initiate settings for the search query
   *
   * @param array $profile
   *   The search to initiate the search query
   */
  private function initQuery($profile) {
    $settings = $profile['settings'];

    $this->setIsAdvanced($settings['advanced'] ? TRUE : FALSE);
    if (isset($settings['filetype']) && !isset($settings['filetype']['all'])) {
      $file_types = array_keys($settings['filetype']);
      foreach ($file_types as $id => $file_type) {
        $file_types[$id] = '_'.$file_type;
      }
      $this->setQueryParameter('FileType', $file_types);
    }
    if (isset($settings['product']) && is_array($settings['product']) && !in_array('all', $settings['product'])) {
      $this->setQueryParameter('Product', array_keys($settings['product']));
    }
    if (isset($settings['edition']) && is_array($settings['edition']) && !in_array('all', $settings['edition'])) {
      $this->setQueryParameter('Edition', array_keys($settings['edition']));
    }
    if (isset($settings['zone']) && is_array($settings['zone']) && !in_array('all', $settings['zone'])) {
      $this->setQueryParameter('Zone', array_keys($settings['zone']));
    }
    if (isset($settings['newsroom']) && is_array($settings['newsroom']) && !in_array('all', $settings['newsroom'])) {
      $this->setOption('Newsroom', array_keys($settings['newsroom']));
    }
    if (isset($settings['withthumbs'])) {
      $this->setOption('withThumbs', array($settings['withthumbs']));
    }
    $this->setOption('facet', array('list' => array('iptc', 'gl', 'pn', 'on')));
    if (isset($settings['cn']) && $settings['cn'] !== 'none') {
      $this->setOption('cn', array($settings['cn']));
    }
  }

  /**
   * Check the validity of a query before building it
   *
   * @return boolean
   *   TRUE if the query is valid, FALSE otherwise
   */
  private function isValidQuery() {
    if ($this->isAdvanced) {
      if (isset($this->query['map']['allOf']) && !empty($this->query['map']['allOf'])) {
        if (!isset($this->query['map']['noneOf'])) {
          $this->query['map']['noneOf'] = array();
        }
        return TRUE;
      } else {
        $this->setErrorLog('validating query', t('No words given for an advanced query'));
      }
    } else {
      if (!empty($this->query['map']['q'])) {
        return TRUE;
      } else {
        $this->setErrorLog('validating query', t('No words given for a simple query'));
      }
    }
    return FALSE;
  }

  /**
   * Error logger
   *
   * @param string $location
   *   Process step in which the error occured
   * @param string $message
   *   Error message
   */
  private function setErrorLog($location, $message) {
    $newError = array(
      'location' => $location,
      'message' => $message,
    );
    $this->errorLogs[] = $newError;
    watchdog('DAM Search Query', t('An error occured during : @location. Error message : @message'), array('@location' => $location, '@message' => $message), WATCHDOG_ERROR);
  }
}
