<?php 
// Préparation des variables à transmettre aux fonctions de templating.
  $data_array = array(); 
  $data_array["footer_message"] = $footer_message; 
  $base_url = $_SERVER['HTTP_HOST'];
  $site_name = variable_get("site_name",NULL);
  //dsm($variables,"variables");
  ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:og="http://ogp.me/ns#" xmlns:fb="http://www.facebook.com/2008/fbml" xml:lang="fr" lang="fr" dir="ltr">
<head>
<title><?php print $head_title ?></title>
<?php print $head; ?>

<base href="http://<?php  print $base_url; ?>" />

<?php print $variables["meta"]; ?>
<?php print $styles ?>
<?php print $scripts ?>
</head>
<body class="<?php print $variables['body_classes']; ?>">
<?php print $SPMetriweb; ?>
<?php print $SPmenutop; ?>
<div id="global">
  <div id="header"><?php print $SPheader; ?><? print theme("sp_header", $data_array); ?></div> 
  <?php if ($messages): print "<div class='messages'>".$messages."</div>"; endif; ?>
  <?php if ($help): print "<div class='messages'>".$help."</div>"; endif; ?>
  <?php print $tabs; ?>
  <div id="content" class="clearfix"> <?php print $content; ?> </div>
  <div id="sp_footer"> <?php print $SPfooter; ?><? print theme("sp_footer", $data_array); ?></div>
</div>
</body>
</html>