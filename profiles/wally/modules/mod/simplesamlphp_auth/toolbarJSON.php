<?php
	session_start();
	require_once('/var/www/php/ssoLibs/ssoPhpToolbox/IDPInstance_rossel.php');
	require_once('/var/www/php/ssoLibs/ssoPhpToolbox/SSOToolbox.php');
  require_once('lib/sfYamlParser.php');
  $target = $_GET["target"];
	$toolbox = new SSOToolbox(array('targetUrl'=>$target));
	$infoSSO = $toolbox->isAuthenticated($target);	

	if (isset($infoSSO[0]) && $infoSSO[0]) {
		// Retrieve user
		$items = build_menu_items('http://pdf.lesoir.be/toolbar/module/toolbar.yaml');
		$jsonData = array('items' => $items, 'user' => $infoSSO[1]['title'][0]." ".$infoSSO[1]['cn'][0], 'mode' => 'authenticated', 'iframe' => '', 'profil' => "http://pdf.lesoir.be/mon_profil/");
	}
	else {
		$items = build_menu_items('http://pdf.lesoir.be/toolbar/module/toolbar_nc.yaml');
		$jsonData = array('items' => $items, 'mode' => 'unauthenticated', 'iframe' => $infoSSO[1]);
	}
	
	echo $_GET['cbk']."(".json_encode($jsonData).")";
	
	function build_menu_items($url) {
		$yaml = new sfYamlParser();
		$items = $yaml->parse(file_get_contents($url));
		return $items;
	}
?>
