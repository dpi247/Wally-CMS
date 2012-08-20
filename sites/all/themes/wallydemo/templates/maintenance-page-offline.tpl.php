<?php
// $Id: maintenance-page.tpl.php,v 1.3.2.1 2009/04/30 00:13:31 goba Exp $

/**
 * @file maintenance-page-offline.tpl.php
 *
 *  This file controls the page that is displayed 
 *  when Drupal cannot access the database (for whatever reason).
 *  
 *  Contenu pûrement html!
 */
?>
<?php 
$host = $_SERVER['HTTP_HOST'];
if(strpos($host, "lameuse.")){
	$tag = "lameuse";
}elseif(strpos($host, "lacapitale.")){
	$tag = "capitale";
}elseif(strpos($host, "laprovince.")){
	$tag = "province";
}elseif(strpos($host, "lanouvellegazette.")){
	$tag = "nouvellegazette";
}elseif(strpos($host, "nordeclair.")){
	$tag = "nordeclair";
}else{
	$tag = "sudpresse";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Site en Maintenance | Sudpresse.be</title>
<style type="text/css">
* {
	margin:0;
	padding:0;
}
body {
	font-family: Arial, Helvetica, sans-serif;
	font-size: 13px;
}
#maintenance #content{
	width: 500px;
	padding: 30px 40px;
	margin:0 auto;
	border: 1px solid #d9d9d9;
	border-top:0 none;
	background:url("http://studio.sudinfo.be/_sandbox/_services/maintenance/cle_plate.png") no-repeat scroll 510px 20px transparent;
}
#maintenance #header {
	background:#d9d9d9;
	border-bottom: 1px solid #b2b2b2;
}
#maintenance #header div {
	width:500px;
	padding: 20px 40px;
	margin:0 auto;	
}
#maintenance h1 {
	font-size: 20px;
	color: #e50000;
}
#maintenance p {
	margin:10px 0;
}
</style>
</head>

<body>
<!-- ***** Metriweb une/sp ***** -->
<script type="text/javascript" src="/metriweb/mwTag.js">
</script>
<script type="text/javascript">
        // max. length:  123456789012345678901234
            var keyword="une/sp"; 
            var extra="fr";
            metriwebTag ("<?php echo $tag; ?>", keyword, extra);
        </script>
<!-- ***** End Metriweb une/sp ***** -->
<div id="maintenance">
  <div id="header">
    <div><a href="http://www.sudinfo.be/"><img src="http://studio.sudinfo.be/_sandbox/_services/maintenance/signature_sudpresse.png" alt="sudinfo.be" title="sudinfo.be" width="500" height="53" border="0" /></a></div>
  </div>
  <div id="content">
    <h1>Votre site d'information est en maintenance. </h1>
    <p>&nbsp; </p>
    <p>&nbsp; </p>
    <p>Cher internaute.</p>
    <p>Votre site d'information est en maintenance suite à des des problèmes 
    techniques.<br />
    Nos équipes sont prévenues et travaillent à la résolution de ce problème.<br />
    Pouvez-vous essayer à nouveau un peu plus tard?</p>
<p> Merci de votre compréhension.</p>
    <p>&nbsp;</p>
    <p> Sudpresse</p>
  </div>
</div>
</body>
</html>
