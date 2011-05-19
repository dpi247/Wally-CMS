<?php
  require_once 'includes/constants.inc';

	require_once(SSO_LIBS_PATH.'/ssoPhpToolbox/IDPInstance_rossel.php');
	require_once(SSO_LIBS_PATH.'/ssoPhpToolbox/SSOToolbox.php');
	
// 	error_log("[SSO] - Start authentication");
	session_start();
// 	error_log("[SSO] - Session started");
	$target = $_POST["target"];
	$toolbox = new SSOToolbox(array('targetUrl'=>$target));
// 	error_log("[SSO] - Toolbox initialized");
		
	if (isset($_POST) && isset($_POST['log'])){
// 		error_log("[SSO] - Try authenticate user ". $_POST['log'] );
		$response = $toolbox->authenticateUser($_POST['log'], $_POST['pass'], "");
// 		error_log("[SSO] - authentication response ".$response[0]);
		switch($response[0]) {
			case SSOToolbox::LOGIN_OK:
				$authsso = 'ok';
				break;
			case SSOToolbox::LOGIN_FAILED:
				$authsso = 'Mauvais login ou mot de passe';
				break;
			case SSOToolbox::LOGIN_UNKNOWN:
				$authsso = 'Login inconnu';
				break;
			case SSOToolbox::LOGIN_INACTIVE:
				$authsso = 'Login inactif';
				break;
			default :// other cases : example serveur down???
					$authsso = "Nous rencontrons actuellement des problèmes techniques, veuillez recommencer plus tard.<br/> Veuillez nous excuser pour ce désagrément<br/>";
		}
	}
	
	if($authsso != 'ok') {
		$ret .= '{"0":"ERROR",';
		$ret .= '"1":"'.$authsso.'"}';
	}
	else {
		$ret .= '{"0":"MESSAGES",';
		$ret .= '"1":"OK"}';	
	}
	echo $_GET['cbk']."(".$ret.")";
/*
	echo $ret;
*/
?>
