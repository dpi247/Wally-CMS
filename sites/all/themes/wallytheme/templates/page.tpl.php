<?php

// Préparation des variables à transmettre aux fonctions de templating.
	$data_array = array(); 
	$data_array["logo"] = $logo;
	$data_array["footer_message"] = $footer_message; 
	$data_array["site_name"] = $site_name; 
	$data_array["site_slogan"] = $site_slogan; 
	$data_array["front_page"] = $front_page;
	$data_array["splash_active"] = $splah_active;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:og="http://ogp.me/ns#" xmlns:fb="http://www.facebook.com/2008/fbml" xml:lang="fr" lang="fr" dir="ltr">
<head>
	<title><?php print $head_title ?></title>
	<?php print $head ?>
	
	<? print theme("soirmag_meta_og"); ?>
  
	<?php print $styles ?>
	<?php print $scripts ?>
	
  <!--[if lt IE 7]>
    <?php print custom_soirmag_get_ie_styles(); ?>
  <![endif]-->
</head>
<body>
<?php print $SMbeginbody;?>
		<!--[if IE 6]><div id="IE6"><![endif]-->
		<!--[if IE 7]><div id="IE7"><![endif]-->
		<!--[if (IE) & (!IE 6) & (!IE 7)]><div id="IE"><![endif]-->
		<!--[if !IE]>--><div id="NOTIE"><!--<![endif]-->

		<div id="header" class="clearfix">
		<? print theme("soirmag_header", $data_array); ?>
		</div>
  
		<div class="menu">
		<?php if (isset($primary_links)) : ?>
		  <?php print $sf_primarymenu; ?> 
		  <? // print theme("wallyct_mainmenu", 'primary-links', 'menu-primary-links'); ?>
		<?php endif; ?>
		</div>

		<div id="global" class="container_12 clearfix">
		<div id="leaderboard">

				<?php print $SMheader; ?>
			</div>
		
			<?php if ($messages): print "<div class='messages'>".$messages."</div>"; endif; ?>
			<?php if ($help): print "<div class='messages'>".$help."</div>"; endif; ?>

		  <?php print $content ?>
		</div>
<div id="prefooter">
		<?php print $SMfooter; ?>
		</div>
		<div id="wrap-footer">
        <? print theme("soirmag_footer", $data_array); ?>
		</div>
<?php //print $scripts2 ?>
<?php print $SMendbody; ?>
</body>
</html>
