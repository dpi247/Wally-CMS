<?php 
/*dsm($subtype,'subtype2');
dsm($context,'context2');
dsm($feed,'feed2');
dsm($options,'options2');*/

/* Récupération du path vers notre theme -> $theme_path
 */
$theme_path = drupal_get_path('theme','wallydemo');
$bloc_title = $options['title'];
$tid = $options['flows'][$flow_id]->conf['destination']['tid'];
$destination_path = drupal_get_path_alias('taxonomy/term/'.$tid);

$content = "";
$cpt = 1;
$feed_length = count($feed);
foreach ($feed as $k=>$item) {
$flow_id = $item['__flow_id'];
$feed_name = $options['flows'][$flow_id]->name;
	$node =$item['node'];
	$node_path = drupal_get_path_alias("node/".$node->nid);
	$mainstory = $node->field_mainstory_nodes[0];
	$title = $mainstory->title;
 		if($cpt == 0){
 			$content .= "<li class=\"first\">";
		} else if ($cpt == $feed_length){
 			$content .= "<li class=\"last\">";
		} else {	
			$content .= "<li>";
		}
	$content .="<a href='".$node_path."'>".wallydemo_check_plain($title)."</a>
             </li>";
$cpt++;
}
 
?>

<div id="<?php print $feed_name; ?>" class="bloc-01">
  <h2><a target="_blank" href="<?php print $destination_path; ?>"><?php print $bloc_title; ?></a></h2>
  <div class="inner-bloc">
    <ul class="clearfix">
      <?php print $content; ?>
    </ul>
  </div>
</div>
