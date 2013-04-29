<?php

/**
 * @file DAMSearchQuery.inc
 * 
 * Provide the DAMSearchQuery class declaration.
 * The DAMSearchQuery object is used to build a research query for the DAM.
 */
class DAMSearchQuery {
  private $query;
  private $options;
  private $JSONQuery;
  private $isAdvanced;
  private $isBuilt;
  
  /**
   * Constructor.
   */
  function DAMSearchQuery() {
    $this->query = array(
      'map' => array(
        'q' => '',
        'FileType' => '_Text',
      ),
    );
    $this->options = array();
    $this->JSONQuery = '';
    $this->isAdvanced = FALSE;
    $this->isBuilt = FALSE;
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
        $tmp_query['map']['allOf'] = implode(',', $tmp_query['map']['allOf']);
        $tmp_query['map']['noneOf'] = implode(',', $tmp_query['map']['noneOf']);
      }
      $this->JSONQuery = json_encode($tmp_query);
      $this->isBuilt = TRUE;
      return TRUE;
    } else {
      return NULL;
    }
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
        $this->query['map']['q'] = $word;
        return TRUE;
      }
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
      }
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
    if (is_bool($val) && $val !== $this->isAdvanced) {
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
            $this->query['map']['q'] = array_shift($this->query['map']['allOf']);
          }
          unset($this->query['map']['allOf']);
        }
        if (isset($this->query['map']['noneOf'])) {
          unset($this->query['map']['noneOf']);
        }
      }
      return TRUE;
    }
    return NULL;
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
   * @return mixed
   *   The value if found, NULL otherwise
   */
  public function getOption($key) {
    return isset($this->options[$key]) ? $this->options[$key] : NULL;
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
      }
    } else {
      if (!empty($this->query['map']['q'])) {
        return TRUE;
      }
    }
    return FALSE;
  }
}
