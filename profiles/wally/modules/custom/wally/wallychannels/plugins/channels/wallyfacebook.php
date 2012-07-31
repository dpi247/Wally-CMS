<?php

require_once "facebook-php-sdk-6c82b3f/src/facebook.php";

/**
 * Extends the Facebook class with the intent of managing
 * access_token expiration time
 */
class WallyFacebook extends Facebook
{
  /**
   * The OAuth token expiration time received in exchange for a valid authorization
   * code.  null means the access token has yet to be determined.
   *
   * @var string
   */
  protected $tokenExpires = null;
  
  /**
   * Identical to the parent constructor
   *
   * @param Array $config the application configuration.
   * @see Facebook::__construct in facebook.php
   */
  public function __construct($config) {
    parent::__construct($config);
  }

  protected static $kSupportedKeys =
    array('state', 'code', 'access_token', 'token_expires', 'user_id');
    
  /**
   * Sets the token expiration time for api calls.
   *
   * @param string $expires a token expiration time.
   * @return WallyFacebook
   */
  public function setTokenExpires($expires) {
    $this->tokenExpires = time() + $expires;
    return $this;
  }

  /**
   * Determines the token expiration time that should be used for API calls.
   *
   * @return string The token expiration time
   */
  public function getTokenExpires() {
    if ($this->tokenExpires !== null) {
      // we've done this already and cached it.  Just return.
      return $this->tokenExpires;
    }

    // first establish access token to be the application
    // access token, in case we navigate to the /oauth/access_token
    // endpoint, where SOME access token is required.
    $this->setAccessToken($this->getApplicationAccessToken());
    $user_access_response = $this->getUserAccessToken();
    if ($user_access_response['access_token']) {
      $this->setAccessToken($user_access_response['access_token']);
      $this->setTokenExpires($user_access_response['expires']);
    }

    return $this->tokenExpires;
  }
    
  /**
   * Determines the access token that should be used for API calls.
   * The first time this is called, $this->accessToken is set equal
   * to either a valid user access token, or it's set to the application
   * access token if a valid user access token wasn't available.  Subsequent
   * calls return whatever the first call returned.
   *
   * @return string The access token
   */
  public function getAccessToken() {
    if ($this->accessToken !== null) {
      // we've done this already and cached it.  Just return.
      return $this->accessToken;
    }

    // first establish access token to be the application
    // access token, in case we navigate to the /oauth/access_token
    // endpoint, where SOME access token is required.
    $this->setAccessToken($this->getApplicationAccessToken());
    $user_access_response = $this->getUserAccessToken();
    if ($user_access_response['access_token']) {
      $this->setAccessToken($user_access_response['access_token']);
      $this->setTokenExpires($user_access_response['expires']);
    }

    return $this->accessToken;
  }

  /**
   * Determines and returns the user access token, first using
   * the signed request if present, and then falling back on
   * the authorization code if present.  The intent is to
   * return a valid user access token, or false if one is determined
   * to not be available.
   *
   * @return string A valid user access token, or false if one
   *                could not be determined.
   */
  protected function getUserAccessToken() {
    // first, consider a signed request if it's supplied.
    // if there is a signed request, then it alone determines
    // the access token.
    $signed_request = $this->getSignedRequest();
    if ($signed_request) {
      // apps.facebook.com hands the access_token in the signed_request
      if (array_key_exists('oauth_token', $signed_request)) {
        $access_token = $signed_request['oauth_token'];
        $this->setPersistentData('access_token', $access_token);
        return $access_token;
      }

      // the JS SDK puts a code in with the redirect_uri of ''
      if (array_key_exists('code', $signed_request)) {
        $code = $signed_request['code'];
        $access_response = $this->getAccessTokenFromCode($code, '');
        if ($access_response['access_token']) {
          $this->setPersistentData('code', $code);
          $this->setPersistentData('access_token', $access_response['access_token']);
          $this->setPersistentData('token_expires', time() + $access_response['expires']);
          return $access_response;
        }
      }

      // signed request states there's no access token, so anything
      // stored should be cleared.
      $this->clearAllPersistentData();
      return false; // respect the signed request's data, even
                    // if there's an authorization code or something else
    }

    $code = $this->getCode();
    if ($code && $code != $this->getPersistentData('code')) {
      $access_response = $this->getAccessTokenFromCode($code);
      if ($access_response['access_token']) {
        $this->setPersistentData('code', $code);
        $this->setPersistentData('access_token', $access_response['access_token']);
        $this->setPersistentData('token_expires', time() + $access_response['expires']);
        return $access_response;
      }

      // code was bogus, so everything based on it should be invalidated.
      $this->clearAllPersistentData();
      return false;
    }

    // as a fallback, just return whatever is in the persistent
    // store, knowing nothing explicit (signed request, authorization
    // code, etc.) was present to shadow it (or we saw a code in $_REQUEST,
    // but it's the same as what's in the persistent store)
    return $this->getPersistentData('access_token');
  }
  
  /**
   * Retrieves an access token for the given authorization code
   * (previously generated from www.facebook.com on behalf of
   * a specific user).  The authorization code is sent to graph.facebook.com
   * and a legitimate access token is generated provided the access token
   * and the user for which it was generated all match, and the user is
   * either logged in to Facebook or has granted an offline access permission.
   *
   * @param string $code An authorization code.
   * @return mixed An access token exchanged for the authorization code, or
   *               false if an access token could not be generated.
   */
  protected function getAccessTokenFromCode($code, $redirect_uri = null) {
    if (empty($code)) {
      return false;
    }

    if ($redirect_uri === null) {
      $redirect_uri = $this->getCurrentUrl();
    }

    try {
      // need to circumvent json_decode by calling _oauthRequest
      // directly, since response isn't JSON format.
      $access_token_response =
        $this->_oauthRequest(
          $this->getUrl('graph', '/oauth/access_token'),
          $params = array('client_id' => $this->getAppId(),
                          'client_secret' => $this->getAppSecret(),
                          'redirect_uri' => $redirect_uri,
                          'code' => $code));
    } catch (FacebookApiException $e) {
      // most likely that user very recently revoked authorization.
      // In any event, we don't have an access token, so say so.
      return false;
    }

    if (empty($access_token_response)) {
      return false;
    }

    $response_params = array();
    parse_str($access_token_response, $response_params);
    if (!isset($response_params['access_token'])) {
      return false;
    }

    return $response_params;
  }
}
