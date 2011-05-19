<?php
	session_start();
	
	require_once 'includes/constants.inc';
	require_once(SSO_LIBS_PATH.'/ssoPhpToolbox/IDPInstance_rossel.php');
	require_once(SSO_LIBS_PATH.'/ssoPhpToolbox/SSOToolbox.php');
  require_once(SSO_SIMPLESAMLPHP_AUTH_PATH.'/lib/sfYamlParser.php');
  
  $target = $_GET["target"];
  $params = array(
    'targetUrl' => $target,
    'simplesamlphp_auth_installdir' => SSO_LIBS_PATH.'/simplesamlphp_1_4',
  );

	$toolbox = new SSOToolbox($params);
	$infoSSO = $toolbox->isAuthenticated($target);	
  
	if (isset($infoSSO[0]) && $infoSSO[0]) {
		// Retrieve user
		$jsonData = array('user' => $infoSSO[1]['title'][0]." ".$infoSSO[1]['cn'][0], 'mode' => 'authenticated', 'iframe' => '', 'profil' => SSO_USER_PROFILE);
	}
	else {
		$jsonData = array('mode' => 'unauthenticated', 'iframe' => $infoSSO[1]);
	}

	echo $_GET['cbk']."(".json_encode($jsonData).")";
