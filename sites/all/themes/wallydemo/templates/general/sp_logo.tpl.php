<?php 
$path = drupal_get_path('theme', 'wallydemo');
$my_data = _wallydemo_get_logo_data();
dsm($my_data);
//dsm($my_data);

if($my_data["default"] == 1){
	$logo_src = $my_data["default_path"];
}else{
  $logo_src = $my_data["eve_path"];
}


?>
<li id="<?php print $my_data["html_id"]; ?>">
  <a href="/<?php /*print $my_data["html_target"]; */?>">
    <img id="<?php print $my_data["html_id"]; ?>" name="<?php print $my_data["html_id"]; ?>" alt="<?php print $my_data["html_alt"]; ?>" src="/<?php print $logo_src; ?>">
  </a>
</li>

<?php 
//ici unset variables
unset($my_data);
?>