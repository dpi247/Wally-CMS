<?php  
$output = "";

$size = count($menu);
$cpt = 1;
foreach($menu as $item){
	
	//dsm($item,'ITEM');
	$item_link = drupal_get_path_alias($item['link']['href']);
	$item_name = $item['link']['title'];
	$class = "";
	if ($cpt == $size){
		$class = " class=\"last\"";
		}
	$output .= "<li".$class."><a href=\"/".$item_link."\">".$item_name."</a></li>";
$cpt++;
}


?>
<div class="menu">
  <ul>
    <?php print $output ; ?>
  </ul>
</div>

<?php 
//ici unset variables
unset($menu);
unset($output);
?>