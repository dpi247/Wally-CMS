<html>
<body>

<?php
	require_once 'includes/constants.inc';
	require_once(SSO_LIBS_PATH.'/ssoPhpToolbox/IDPInstance_rossel.php');
	require_once(SSO_LIBS_PATH.'/ssoPhpToolbox/SSOToolbox.php');
	
  $target = $_GET["target"];
  $params = array(
    'targetUrl' => $target,
    'simplesamlphp_auth_installdir' => SSO_LIBS_PATH.'/simplesamlphp_1_4',
  );

	$toolbox = new SSOToolbox($params);
	$sp = $toolbox->getSP();
	$newTarget = $sp->getSimpleSamlPath()."sso_redirect.php?url=" . urlencode($target) . '&dummy=' . time();
	$result = $toolbox->getAutoPostLoginForm($_GET["login"],$_GET["pwd"], $newTarget);
	echo $result;
?>

</body>
</html>
