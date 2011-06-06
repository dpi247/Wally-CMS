<?php 
if(is_array($items)){
	foreach($items as $linklist){
		if(isset($linklist['nid'])){
		$linklist_node=node_load($linklist['nid'], NULL, TRUE);
		echo "<div id=\"related-article\" class=\"brique\">\n";
		echo "<h2>".$linklist_node->title."</h2>\n";
		echo "<p>".$linklist_node->field_summary[0]["value"]."</p>";
		echo "<ul>\n";
		if(is_array($linklist_node->field_links_list)){
		foreach($linklist_node->field_links_list as $linkitem){
			$linkitem_node=node_load($linkitem['nid'], NULL, TRUE);
			$node_path = "";
			if(isset($linkitem_node->field_internal_link[0]["nid"]) && $linkitem_node->field_internal_link[0]["nid"] != "" && $linkitem_node->field_internal_link[0]["nid"] != NULL){
				$node_path = drupal_get_path_alias($linkitem_node->field_internal_link[0]["nid"]);
				echo "<li><a href=\"".$node_path."\">".$linkitem_node->title."</a></li>\n";
			}elseif(isset($linkitem_node->field_link_item[0]["url"]) && $linkitem_node->field_link_item[0]["url"] != NULL){
				$attributes = "";
				foreach($linkitem_node->field_link_item[0]['attributes'] as $key => $value){
					if($value!=0){
						$attributes .= $key."=\"".$value."\" ";
					}
				}
				echo "<li><a href=\"".$linkitem_node->field_link_item[0]["url"]."\" ".$attributes.">".$linkitem_node->title."</a></li>\n";
			}
		}
		echo "</ul>\n";
		echo "</div>\n";
		}
	}
	}
}
?>
