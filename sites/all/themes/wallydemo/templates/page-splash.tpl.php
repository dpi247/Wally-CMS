<?php 
// Préparation des variables à transmettre aux fonctions de templating.
  $data_array = array(); 
  $data_array["footer_message"] = $footer_message; 
  $base_url = $_SERVER['HTTP_HOST'];
  $site_name = variable_get("site_name",NULL);
  //dsm($variables,"variables");

/* Récupération du path
 * 
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:og="http://ogp.me/ns#" xmlns:fb="http://www.facebook.com/2008/fbml" xml:lang="fr" lang="fr" dir="ltr">
<head>
<title><?php print $head_title ?></title>
<?php print $head; ?>
<base href="http://<?php  print $base_url; ?>" />
<?php print $variables["meta"]; ?>
<?php print $scripts ?>
</head>
<body class="<?php print $variables['body_classes']; ?>">
<?php print $SPMetriweb; ?>
<div id="splash">
   	<div id="header">
    	<a href="javascript:document.location.href = LireCookie('splash_active');"><img src="<?php print $theme_path ?>/images/splash-header.gif" width="630" height="61" alt="" /></a>
    </div>
  	<div id="content">
        <?php print $content; ?>
      	<script type="text/javascript">
			setTimeout("window.location=LireCookie('splash_active')",15000); 
		</script>
  	</div>  	
  	<div id="footer">    
	 	<p><a class="entrance" href="javascript:document.location.href = LireCookie('splash_active');">Entrez</a><br />avec <a href="http://www.rosseladvertising.be/online/fr/index.php" title="http://www.rosseladvertising.be/online/fr/index.php">Rossel Advertising</a></p>
  	</div>    
</div>
</body>
</html>