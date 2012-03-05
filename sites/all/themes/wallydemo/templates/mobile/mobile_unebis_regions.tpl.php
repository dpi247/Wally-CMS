<?php
//dsm($data,'DATA');

$inner_html = "";

  foreach($data as $term){
  	$term_name = $term['term']->name;
    $term_id = $term['term']->tid;
    $term_path = drupal_get_path_alias('taxonomy/term/'.$term_id);  	
    $inner_html .= "<li><a href=\"/".$term_path."\">".$term_name."</a></li>";
  }
  
?>
<h1 id="h1_taxonomy"><span>Régions</span></h1>  
<div id="liste_regions">
  <ul>
  <?php print $inner_html; ?>
  </ul>
</div>

<?php 
//ici unset variables
unset($data);
unset($inner_html);
?>