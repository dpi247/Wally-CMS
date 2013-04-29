<?php

require_once 'DAMSearchQuery.inc';

/**
 * @file DAMSearch.inc
 * @author odm
 *
 * Provide the DAMSearch class declaration.
 * The DAMSearch object is used to perform research into the DAM.
 * The research process includes :
 *   - Creating a JSON search query
 *   - Sending the query to the DAM through the ESB
 *   - Getting the results of the query
 *   - Transform the results in a usable format
 */
class DAMSearch {
  private $profile;
  private $query;
  private $rawResults;
  private $results;

  /**
   * Constructor, initiate the object's attributes according to a given profile.
   *
   * @param string $init_profile
   *   The profile ID used to initiate the DAMSearch object.
   */
  function DAMSearch($init_profile = 'default') {
    $this->rawResults = '';
    $this->results = array();

    $this->query = new DAMSearchQuery();

    $this->initProfile($init_profile);
  }

  /**
   * Build the query, send it to the DAM, get raw results and transform them
   *
   * @return array
   *   An array of results in case of success, NULL otherwise
   */
  public function executeQuery() {
    $this->query->buildQuery();
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
   * Get current profile ID
   *
   * @return number
   *   The current profile ID
   */
  public function getProfileID() {
    return $this->profile;
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
  private function initProfile($profile_id) {
    if ($this->setProfileID($init_profile)) {
      return $this->profile;
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
  private function setProfileID($profile_id = 'default') {
    if ($profile_id == 'default' || !$this->checkProfileExists($profile_id)) {
      $profile_id = $this->getDefaultProfileID();
    }

    if ($profile_id) {
      $this->profile = $profile_id;
      return $this->profile;
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
    return FALSE;
  }

  /**
   * Get the default profile ID
   *
   * @return number
   *   The profile ID
   */
  private function getDefaultProfileID() {
    return 1;
  }

  /**
   * Send the builded query to the DAM and get the raw JSON results
   *
   * @return string
   *   The raw JSON results
   */
  private function sendQuery() {
    return NULL;
  }

  /**
   * Transform raw JSON results from the DAM to a more usable format
   *
   * @return array
   *   The transformed results
   */
  private function transformResults() {

  }
}
