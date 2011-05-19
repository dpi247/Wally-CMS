<?php
	session_start();
	
	global $base_url;
	$user_profile = variable_get('sso_user_profile', $base_url.'/user/');
	$ssolibs_path = variable_get('sso_libs_path','/usr/local/lib/ssolibs');
	$simplesamlphp_auth_path = drupal_get_path('module', 'simplesamlphp_auth');
	require_once($ssolibs_path.'/ssoPhpToolbox/IDPInstance_rossel.php');
	require_once($ssolibs_path.'/ssoPhpToolbox/SSOToolbox.php');
  require_once($simplesamlphp_auth_path.'/lib/sfYamlParser.php');
  
  $target = $_GET["target"];
  $params = array(
    'targetUrl' => $target,
    'simplesamlphp_auth_installdir' => $ssolibs_path.'/simplesamlphp_1_4',
  );

	$toolbox = new SSOToolbox($params);
	$infoSSO = $toolbox->isAuthenticated($target);	
  
	if (isset($infoSSO[0]) && $infoSSO[0]) {
		// Retrieve user
		//$items = build_menu_items('toolbar.yaml');
		$jsonData = array('items' => '', 'user' => $infoSSO[1]['title'][0]." ".$infoSSO[1]['cn'][0], 'mode' => 'authenticated', 'iframe' => '', 'profil' => $user_profile);
	}
	else {
		//$items = build_menu_items('toolbar_nc.yaml');
		$jsonData = array('items' => '', 'mode' => 'unauthenticated', 'iframe' => $infoSSO[1]);
	}

	echo $_GET['cbk']."(".json_encode($jsonData).")";
	
	function build_menu_items($url) {
		$yaml = new sfYamlParser();
		$items = $yaml->parse(file_get_contents($url));
		return $items;
	}
