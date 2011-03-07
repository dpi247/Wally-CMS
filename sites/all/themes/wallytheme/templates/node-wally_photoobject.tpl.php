<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $

$packages_id = custom_soirmag_getpackagesid($node);
$packages_nb = count($packages_id);

// if the wallyObject is linked to several packages, we have to know the last created one.
// this fist one is used as the main element's target
// the next ones are displayed trought the "see also" html part
if($packages_nb > 1){
	$packages_id_ordered = custom_soirmag_order_packages($packages_id, 'created', 'ASC');
  $package_id = $packages_id_ordered[0]['nid'];
  $html_see_also = custom_soirmag_display_taxonomy_term_list_see_also($packages_id);
}else{
	$package_id = $packages_id[0]['nid'];
}

$package_path = drupal_get_path_alias("node/".$package_id);
$node_last_modif = $node->changed;
$date_edition = custom_soirmag_date_edition_diplay($node_last_modif, 'default_destination_view');

$photo['credit'] = $node->title;
$photo['summary'] = $node->field_summary[0]['value'];
$photo['fullpath'] = $node->field_photofile[0]['filepath'];
$photo['mini'] = theme('imagecache', 'sm_mini_article', $node->field_photofile[0]["filename"],$photo['credit']); 

$htmltags = custom_soirmag_taxonomy_tags_particle($node);

?>

<div class="photoobject_item">
<a href="/<?php print $package_path ?>" title="<?php print $title ?>"><?php print $photo['mini'] ; ?></a>
<h2><a href="/<?php print $package_path ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<small><?php print $date_edition ; ?></small>
<p><?php $photo['summary'] ; ?></p>
<?php if(isset($html_see_also)) print $html_see_also; ?>
	<p> 
	<?php 
		echo $htmltags;
	?>
	</p>
</div>
