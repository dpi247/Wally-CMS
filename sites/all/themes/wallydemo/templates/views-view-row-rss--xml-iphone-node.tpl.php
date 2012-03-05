<?php
// $Id: views-view-row-rss-mobile.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss-mobile.tpl.php
 * Default view template to display a item in a Flow feed for Mobile application.
 *
 * @ingroup views_templates
 */


$guid = $node->nid;
$artbody = $node->field_mainstory_nodes[0]->field_textbody[0]["safe"];
$cat = taxonomy_get_term($node->field_destinations[0]["tid"]); 
$date = date("r",$node->created);

$artbody = $node->field_mainstory_nodes[0]->field_textbody[0]["value"];
$site_url = "http://".$_SERVER["SERVER_NAME"];

$linkList = node_load($node->field_linkedobjects[0]["nid"]);
$oneLink = array();
$allLink = array();
if(is_array($linkList->field_links_list)) {
  foreach ($linkList->field_links_list as $l) {  
    $n = node_load($l["nid"]);
    $oneLink["title"] = $n->title;
    $iLink = $n->field_internal_link[0]["nid"];
    if($iLink) $oneLink["path"] = $site_url.'/xml_iphone/node/'.$iLink.'?format=-xml-iphone-node';    
    else $oneLink["path"] = $n->field_link_item[0]["url"];
    array_push($allLink, $oneLink);
  }
}

$node_path = drupal_get_path_alias("node/".$node->nid);
$url = "http://".$_SERVER['SERVER_NAME']."/$node_path";    
$photo_url = "http://".$_SERVER['SERVER_NAME']."/".$node->field_embededobjects_nodes[0]->field_photofile[0]['filepath']; 

/*
$cat->name = string_to_numericentities_mod($cat->name);
$title = string_to_numericentities_mod(strip_tags($title));
$description = string_to_numericentities_mod($description);
$artbody = string_to_numericentities_mod($artbody);
*/

$cat->name = $cat->name;
$title = strip_tags($title);
$description = $description;
$artbody = $artbody;


?>
<item>
  <category><![CDATA[<?php print $cat->name;?>]]></category>
  <title><![CDATA[<?php print $title; ?>]]></title>
  <media:content url="<?php print $photo_url;?>" />
  <media:thumbnail url="<?php print $url_thumb;?>"/>
  <link><?php print $site_url ?>/xml_iphone/node/<?php print $guid ?>?format=-xml-iphone-node</link>
  <description><?php print $description;?></description>
  <artbody><![CDATA[<?php print $artbody;?>]]></artbody>
  <author><![CDATA[R&#233;daction en ligne]]></author>
  <pubDate><?php print $date;?></pubDate>
  <guid isPermaLink="false"><?php print $guid; ?></guid>
  <?php if (count($allLink) > 0) {?>
  <related>
  	<?php foreach ($allLink as $l) { ?>
	<relatedArticle id="<?php print $l["path"] ?>"><![CDATA[<?php print $l["title"] ?>]]></relatedArticle>
	<?php } ?>
  </related>
  <?php } ?>
  <soirurl value="<?php print $link; ?>" />
</item>
