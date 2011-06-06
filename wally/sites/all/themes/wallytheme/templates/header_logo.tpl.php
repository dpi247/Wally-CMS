<?php

$path = drupal_get_path('theme', 'custom_soirmag');

// Préparation des variables.
$sm_slogan = check_plain($data_array["site_slogan"]);
$sm_site_name = check_plain($data_array["site_name"]);
$sm_logo = $data_array["logo"];
if(!isset($front_page)){
	$homepage = "/";
}else{
	$homepage = check_url($front_page);
}
?>
<a id="logo" href="<?php print $homepage; ?>">
<?php 
	if ($sm_logo!="") {
		print "<img src='".check_url($sm_logo)."' alt='".$sm_site_name."' width='300' height='60' />";
	} else {
		print "<img src='/".$path."/mediastore/elements/soirmag.png' width='300' height='60' alt='".$sm_site_name."' />";
	}
?>
<?php
  if ($sm_slogan) {
    print "<span>".check_plain($sm_slogan)."</span>";
  } 
?> 
</a>