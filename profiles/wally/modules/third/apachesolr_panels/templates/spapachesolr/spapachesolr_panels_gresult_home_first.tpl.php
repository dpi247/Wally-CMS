<?php 
	//
	// les data (resultats sont dans $results
	//

/**
 * 
 * Display a "see also" html list from an packages' id array
 * Used in the taxonomy term list who is showing wallyObjects 
 * @param $packages_id
 *  Array() of array() packages' id
 *  
 *  @return
 *    htlm list
 */
function custom_sp_display_taxonomy_term_list_see_also($packages_id){
  $html = "<h3>Voir aussi</h3><ul class=\"see_also_taxo_term_list\">";
  $cpt = 1;
	foreach($packages_id as $package_id){
    if ($cpt > 1){
		  $node = node_load($package_id['nid']);
      $node_path = drupal_get_path_alias("node/".$node->nid);
      $html .= "<li><a href=\"/".$node_path."\">".$node->title."</a></li>";     
    }
    $cpt++;
  }
  $html .= "</ul>";
  return $html;
}


/**
 * Render the differents date displays
 * 
 * @param $unix_time
 *   
 * @param $display
 */
function custom_sp_date_edition_diplay($unix_time, $display='default'){
	$french_days = array('0'=>'Dimanche', '1'=>'Lundi', '2'=>'Mardi', '3'=>'Mercredi', '4'=> 'Jeudi', '5'=>'Vendredi', '6'=>'Samedi');
	$french_months = array('1'=>'Janvier', '2'=>'Février', '3'=>'Mars', '4'=>'Avril', '5'=>'Mai', '6'=>'Juin','7'=>'Juillet', '8'=>'Août', '9'=>'Septembre', '10'=>'Octobre', '11'=>'Novembre', '12'=>'Décembre');
	//$format = array(	 'default' => "l d F Y, H<abbr title=\"heure\">\h</abbr>i"	);
	//$displayed_date = date($format[$display],$unix_time);
	$displayed_date=$french_days[date('w', $unix_time)]." ".date('j', $unix_time)." ".$french_months[date('n', $unix_time)]." ".date('Y, H<abbr title=\"heure\">\h</abbr>i', $unix_time);
	return $displayed_date;
}


/**
 * 
 * Order an array of Wally packages' id
 * @param $array
 *  array() of array() of packages's nid
 * @param $sort
 *  the key to sort the nid
 * @param $order
 *  ASC or DESC
 * @return Array() of array() ordered $out's values
 */
function custom_sp_order_packages($array,$sort,$order){
	$count = count($array);
  $cpt = 1;
  $rows = array();
	$query = "SELECT nid,".$sort." FROM node WHERE";
  foreach($array as $row){
    $query .= " nid=".$row['nid']."";
    if ($cpt<$count){
     $query .= " OR";
    }
    $cpt++;
  }
  $query .= " ORDER BY ".$sort." ".$order."";
  $res=db_query($query);
  while ($row = db_fetch_array($res)) {
    if (!in_array($row, $rows)) $rows[] = $row;
  }
  return $rows;   
}



/**
 * Get nid from packages who reference the current node
 * 
 * @param $node
 *   Node currently processed(TextObject, AudioObject, DigitalObject,PhotoObject, Videoobject, PollObject)
 *
 * @return
 *   Array() of array() of nid
 */
function custom_sp_getpackagesid($node){
  $rows = array(); 
  
  if($node->type=='wally_textobject'){
    $res=db_query("SELECT nid FROM {content_type_wally_articlepackage} WHERE field_mainstory_nid = %d", $node->nid);
    while ($row = db_fetch_array($res)) {
      if (!in_array($row, $rows)) $rows[] = $row;
    }
  }
  elseif($node->type=='wally_audioobject' || $node->type=='wally_digitalobject' || $node->type=='wally_photoobject' || $node->type=='wally_videoobject'){   
     if ($res=db_query("SELECT nid FROM {content_field_embededobjects} WHERE field_embededobjects_nid = %d", $node->nid)){
      while ($row = db_fetch_array($res)) {
        if (!in_array($row, $rows)) $rows[] = $row;
      }
    }else
    if ($res2=db_query("SELECT nid FROM {content_type_wally_gallerypackage} WHERE field_mainobject_nid = %d", $node->nid)){
      while ($row = db_fetch_array($res2)) {
        if (!in_array($row, $rows)) $rows[] = $row;
      }
    }
  }
  elseif($node->type=='wally_pollobject'){
    $res=db_query("SELECT nid FROM {content_type_wally_pollpackage} WHERE field_mainpoll_nid = %d", $node->nid);
    while ($row = db_fetch_array($res)) {
      if (!in_array($row, $rows)) $rows[] = $row;
    }
  }
  return $rows;
}

$node = node_load($results["fields"]["nid"]); 
$packages_id = custom_sp_getpackagesid($node);

$packages_nb = count($packages_id);

// if the wallyObject is linked to several packages, we have to know the last created one.
// this fist one is used as the main element's target
// the next ones are displayed trought the "see also" html part
if($packages_nb > 1){
	$packages_id_ordered = custom_sp_order_packages($packages_id, 'created', 'ASC');
  $package_id = $packages_id_ordered[0]['nid'];
  $html_see_also = custom_sp_display_taxonomy_term_list_see_also($packages_id);
}else{
	$package_id = $packages_id[0]['nid'];
}

$package_path = drupal_get_path_alias("node/".$package_id);
$node_path = drupal_get_path_alias("node/".$node->nid);
$node_last_modif = $node->changed;
$date_edition = custom_sp_date_edition_diplay($node_last_modif, 'default_destination_view');
$strapline = $node->field_textchapo[0]['value'];
if ($strapline == ""){
 $teaser_length = '300';
 $teaser = theme("wallyct_teaser", $node->field_textbody[0]['value'], $teaser_length, $node);
 $strapline = $teaser;
}
$htmltags = custom_sp_taxonomy_tags_particle($node);
?>
<hr>
<div class="textobject_taxo_term_list">
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
