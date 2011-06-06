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
$node_path = drupal_get_path_alias("node/".$node->nid);
$node_last_modif = $node->changed;
$date_edition = wallythemes_date_edition_diplay($node_last_modif, 'default_destination_view');
$strapline = $node->field_textchapo[0]['value'];
if ($strapline == ""){
 $teaser_length = '300';
 $teaser = theme("wallyct_teaser", $node->field_textbody[0]['value'], $teaser_length, $node);
 $strapline = $teaser;
}

$htmltags = wallythemes_taxonomy_tags_particle($node);

?>
<div class="digitalobject_item">
<span class="digitalobject_item">digitalObject</span>
  <h2><a href="/<?php print $package_path ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <small><?php print $date_edition ; ?></small>
  <p><?php print $strapline ; ?></p>
  <?php if(isset($html_see_also)) print $html_see_also; ?>
	<p> 
	<?php 
		echo $htmltags;
	?>
	</p>
</div>