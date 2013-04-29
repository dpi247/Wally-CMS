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

  /**
   * Constructor, initiate the object's attributes according to a given profile.
   *
   * @param string $init_profile
   *   The profile ID used to initiate the DAMSearch object.
   */
  function DAMSearch($init_profile = 'default') {
    $this->query = new DAMSearchQuery();
    
    $this->initProfile($init_profile);
  }

  /**
   *
   */
  public function executeQuery() {
    $this->query->buildQuery();
  }

  /**
   *
   */
  public function getSearchQuery() {

  }

  /**
   *
   */
  public function getProfileID() {
    return $this->profile;
  }
  
  /**
   * 
   * @param string $profile_id
   */
  private function initProfile($profile_id) {
    $this->setProfileID($init_profile);
  }

  /**
   *
   */
  private function setProfileID($profile_id = 'default') {
    if ($profile_id == 'default' || !$this->checkProfileExists($profile_id)) {
      $profile_id = $this->getDefaultProfileID();
    }

    $this->profile = $profile_id;

    return $this->profile;
  }

  /**
   * 
   * @param string $profile_id
   * @return boolean
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
   *
   */
  private function sendQuery() {

  }

  /**
   *
   */
  private function transformResults() {

  }
}
