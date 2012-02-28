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
<meta name="viewport" content="width=device-width, height=device-height" />
<meta name="HandheldFriendly" content="True" />
<title><?php print $head_title ?></title>
<?php print $head; ?>
<base href="http://<?php  print $base_url; ?>" />
<?php print $variables["meta"]; ?>
<?php //print $styles ?>
<link rel="stylesheet" type="text/css" href="<?php print $theme_path; ?>/css/mobile.css" media="handheld, all" />
<?php //print $scripts ?>
<script type="text/javascript">
<!--//--><![CDATA[//><!--
  var _gaq = _gaq || [];
  _gaq.push(['_setAccount', 'UA-19995311-1']);
  _gaq.push(['_trackPageview']);
  _gaq.push(['_setDomainName', '.sudinfo.be']);
  (function() {
    var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
  })();
//--><!]]>
</script>
</head>
<body class="<?php print $variables['body_classes']; ?>">
<?php print $SPMetriweb; ?>
<div id="mobile">
  <div id="header"><? print theme("mobile_header", $data_array); ?> </div>
  <?php if ($messages): print "<div class='messages'>".$messages."</div>"; endif; ?>
  <?php if ($help): print "<div class='messages'>".$help."</div>"; endif; ?>
  <?php print $tabs; ?> <?php print $content; ?> <? print theme("mobile_footer", $data_array); ?> </div>
</body>
</html>