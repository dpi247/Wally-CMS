<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $

$packages_id = wallythemes_getpackagesid($node);
$packages_nb = count($packages_id);

// if the wallyObject is linked to several packages, we have to know the last created one.
// this fist one is used as the main element's target
// the next ones are displayed trought the "see also" html part
if($packages_nb > 1){
	$packages_id_ordered = wallythemes_order_packages($packages_id, 'created', 'ASC');
  $package_id = $packages_id_ordered[0]['nid'];
  $html_see_also = wallythemes_display_taxonomy_term_list_see_also($packages_id);
}else{
	$package_id = $packages_id[0]['nid'];
}

$package_path = drupal_get_path_alias("node/".$package_id);
$node_last_modif = $node->changed;
$date_edition = wallythemes_date_edition_diplay($node_last_modif, 'default_destination_view');

$video['summary'] = $node->field_summary[0]['value'];
$video['thumbnail'] = $node->field_video3rdparty[0]['data']['thumbnail']["url"];

$htmltags = wallythemes_taxonomy_tags_particle($node);

?>

<div class="videoobject_item">
  <?php if ($video['thumbnail'] != "") { ?>
    <a href="/<?php print $package_path ?>" title="<?php print $title ?>"><img src="<?php print $video["thumbnail"] ; ?>" width="56" height="33"/></a>
  <?php } ?>
  <h2><a href="/<?php print $package_path ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <small><?php print $date_edition ; ?></small>
  <p><?php print $video['summary'] ; ?></p>
  <?php if(isset($html_see_also)) print $html_see_also; ?>
  <p> 
	<?php 
		echo $htmltags;
	?>
	</p>
  
</div>
