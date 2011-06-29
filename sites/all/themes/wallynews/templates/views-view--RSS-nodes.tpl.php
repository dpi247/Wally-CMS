<?php 
$site_name = variable_get('site_name','Wally Dev');
$site_slogan = variable_get('site_slogan', 'Wally Dev');
$section = "La Une";

$meta_author =  variable_get('site_meta_author_default', 'Soirmag');
$meta_owner = variable_get('site_meta_owner_default', 'soirmag');
$meta_robots = variable_get('site_meta_robots_default', 'index, follow, noarchive');
$meta_refresh = variable_get('site_meta_refresh_default', '420');

$meta_keywords = variable_get('site_meta_keywords_default', $section);
$meta_keywords = str_replace('[site-name]',$site_name,$meta_keywords);
$meta_keywords = str_replace('[site-slogan]',$site_slogan,$meta_keywords);
$meta_keywords = str_replace('[destination-term]',$section,$meta_keywords);

$meta_description = variable_get('site_meta_description_default', $section);
$meta_description = str_replace('[site-name]',$site_name,$meta_description);
$meta_description = str_replace('[site-slogan]',$site_slogan,$meta_description);
$meta_description = str_replace('[destination-term]',$section,$meta_description);

//$meta_title = variable_get('site_meta_title_default', $section);
//$meta_title = str_replace('[site-name]',$site_name,$meta_title);
//$meta_title = str_replace('[site-slogan]',$site_slogan,$meta_title);
//$meta_title = str_replace('[destination-term]',$section,$meta_title);

wallytoolbox_add_meta(array("name"=>"keywords"), $meta_keywords);
wallytoolbox_add_meta(array("name"=>"description"), $meta_description);
wallytoolbox_add_meta(array("name"=>"author"), $meta_author);
wallytoolbox_add_meta(array("name"=>"owner"), $meta_owner);
wallytoolbox_add_meta(array("name"=>"Robots"), $meta_robots);
wallytoolbox_add_meta(array("http-equiv"=>"refresh"), $meta_refresh);
?>
<div id="event">
<?php 
if ($rows): ?>
	<?php print $rows; ?>
  <?php elseif ($empty): ?>
      <?php print $empty; ?>
  <?php endif; ?>
</div>
