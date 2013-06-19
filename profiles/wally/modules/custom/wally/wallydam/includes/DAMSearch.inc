<?php

require_once 'DAMSearchQuery.inc';

/**
 * @file DAMSearch.inc
 * @author odm
 *
 * Provide the DAMSearch class declaration
 * The DAMSearch object is used to perform research into the DAM
 * The research process includes :
 *   - Creating a JSON search query
 *   - Sending the query to the DAM through the ESB
 *   - Getting the results of the query
 *   - Transform the results in a usable format
 */
class DAMSearch {
  private $profile;
  private $loadedProfile;
  private $query;
  private $rawResults;
  private $results;
  private $queryURL;
  private $successfullQuery;
  private $errorLogs;

  /**
   * Constructor, initiate the object's attributes according to a given profile
   *
   * @param string $init_profile
   *   The profile ID used to initiate the DAMSearch object
   */
  function DAMSearch($init_profile = 'default') {
    $this->loadedProfile = FALSE;
    if ($profile = $this->initProfile($init_profile)) {
      $this->rawResults = array();
      $this->results = array();
      $this->successfullQuery = FALSE;
      $this->query = new DAMSearchQuery($profile);
      $this->queryURL = wally_variable_get('wallydam_search_query_URL', '');
      $this->errorLogs = array();
    }
  }

  /**
   * Build the query, send it to the DAM, get raw results and transform them
   *
   * @return array
   *   An array of results in case of success, NULL otherwise
   */
  public function executeQuery() {
    if ($this->loadedProfile) {
      $searchQuery = $this->query;
      $searchQuery->buildQuery();

      if ($searchQuery->isBuilt()) {
        $this->sendQuery();
      } else {
        $this->setErrorLog('execute', t('Query not built'));
        if ($searchQueryLogs = $searchQuery->getErrorLogs()) {
          $this->errorLogs = array_merge($this->errorLogs, $searchQueryLogs);
        }
      }
    } else {
      $this->setErrorLog('execute', t('Profile not loaded'));
    }
  }

  /**
   * Get the DAMSearchQuery object associated to this DAMSearch
   *
   * @return DAMSearchQuery
   *   The DAMSearchQuery object
   */
  public function getSearchQuery() {
    return $this->query;
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
   * Get current profile ID
   *
   * @return number
   *   The current profile ID
   */
  public function getProfile() {
    return $this->profile;
  }

  /**
   * Get results from search
   *
   * @return array
   *   The results
   */
  public function getRawResults() {
    return $this->rawResults;
  }

  /**
   * Set the ititial profile ID
   *
   * @param string $profile_id
   *   The profile ID, a numeric ID or "default"
   *
   * @return number|NULL
   *   The initiated profile ID in case of success, NULL otherwise
   */
  private function initProfile($profile_id = 'default') {
    $this->loadProfile($profile_id);

    if ($this->loadedProfile) {
      $profile = $this->profile;
      if (isset($profile['settings']) && !empty($profile['settings'])) {
        if (empty($this->errorLogs)) {
          return $this->profile;
        }
      } else {
        $this->setErrorLog('init profile', t('Profile "@pid" not configured yet', array('@pid' => $profile_id)));
      }
    } else {
      $this->setErrorLog('init profile', t('Couldn\'t set profile ID "@pid"', array('@pid' => $profile_id)));
    }

    return NULL;
  }

  /**
   * Set a profile ID, if none given set the default profile ID
   *
   * @param string $profile_id
   *   The profile ID, a numeric ID or "default"
   *
   * @return number|NULL
   *   The profile ID in case of success, NULL otherwise
   */
  private function loadProfile($profile_id = 'default') {
    if (is_null($profile_id) || empty($profile_id) || $profile_id == 'default') {
      $profile_id = $this->getDefaultProfileID();
    }

    if ($profile_id) {
      $profile = wallydam_get_search_profile($profile_id);
      if ($profile && isset($profile['pid']) && !empty($profile['pid'])) {
        $this->profile = $profile;
        $this->loadedProfile = TRUE;
      } else {
        $this->setErrorLog('init profile', t('Unable to load profile "@pid"', array('@pid' => $profile_id)));
      }
    } else {
      $this->setErrorLog('init profile', t('No default profile defined'));
    }

    return NULL;
  }

  /**
   * Check wheter a given profile ID does exists or not
   *
   * @param string $profile_id
   *   The profile ID to be checked
   *
   * @return boolean
   *   TRUE if the profile exists, FALSE otherwise
   */
  private function checkProfileExists($profile_id) {
    return wallydam_check_profile_exists($profile_id);
  }

  /**
   * Get the default profile ID
   *
   * @return number
   *   The profile ID
   */
  private function getDefaultProfileID() {
    return wallydam_get_default_search_profile();
  }

  /**
   * Send the builded query to the DAM and get the raw JSON results
   *
   * @return string
   *   The raw JSON results
   */
  private function sendQuery() {
    $searchQuery = $this->query;
    if (!empty($this->queryURL)) {
      $query_tmp_file = file_directory_temp().'/dam-post-query.json';
      $fh = fopen($query_tmp_file, 'w');
      fwrite($fh, $searchQuery->getJSONQuery());
      fclose($fh);

      $postfields = array('query' => '@'.$query_tmp_file);
      $target_url = $this->queryURL;
      $content_type = 'application/json';

      if ($rawDatas = wallydam_post($postfields, $target_url, $content_type, TRUE)) {
        if ($decoded_datas = json_decode($rawDatas)) {
          if (isset($decoded_datas->results) && !empty($decoded_datas->results)) {
            $this->rawResults = $decoded_datas;
            $this->successfullQuery = TRUE;
          } else {
            $this->setErrorLog('query', t('The decoded JSON contains no results'));
          }
        } else {
          $this->setErrorLog('query', t('Unable to decode JSON'));
        }
      } else {
        $this->setErrorLog('query', t('The HTTP request didn\'t succeed, response code : @respcode', array('@respcode' => $HTTPRequestResults->code)));
      }

      unlink($query_tmp_file);
    } else {
      $this->setErrorLog('query', t('Request URL not set'));
    }
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
    watchdog('DAM Search', t('An error occured during : @location. Error message : @message'), array('@location' => $location, '@message' => $message), WATCHDOG_ERROR);
  }
}
