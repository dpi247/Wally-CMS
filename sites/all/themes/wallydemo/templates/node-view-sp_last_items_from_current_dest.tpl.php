<?php 

// Give the index of the row into the view.
$row_index=$variables["view"]->row_index;

/* Récupération de l'alias de l'url du package -> $node_path
 * 
 * print($node_path);
 */
$node_path = wallydemo_get_node_uri($node);


if($node->type == "wally_articlepackage"){
  $mainstory = $node->field_mainstory_nodes[0];
} else {  
  $mainstory = $node->field_mainobject_nodes[0];
}


/* Récupération du titre de l'object principal donc du package à l'affichage -> $title
 * 
 * print($title);
 */  
$title = $mainstory->title;


?>

<li>
  <a href="<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a>
</li>

<?php 
//ici unset des variables
unset($node);
unset($mainstory);
?>