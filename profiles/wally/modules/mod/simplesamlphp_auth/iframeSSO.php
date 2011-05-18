<html>
<body>

<?php
	require_once('/var/www/php/ssoLibs/ssoPhpToolbox/IDPInstance_rossel.php');
	require_once('/var/www/php/ssoLibs/ssoPhpToolbox/SSOToolbox.php');
  	$target = $_GET["target"];
	$toolbox = new SSOToolbox(array('targetUrl'=>$target));
	$sp = $toolbox->getSP();
	$newTarget = $sp->getSimpleSamlPath()."sso_redirect.php?url=" . urlencode($target) . '&dummy=' . time();
	$result = $toolbox->getAutoPostLoginForm($_GET["login"],$_GET["pwd"], $newTarget);
	echo $result;
?>

</body>
</html>
