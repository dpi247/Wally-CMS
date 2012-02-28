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
$nbcomments = $node->field_mainstory_nodes[0]->comment;
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
$moburl = str_replace("/www.", "/mobile.", $url);    
$photo_url = "http://".$_SERVER['SERVER_NAME']."/".$node->field_embededobjects_nodes[0]->field_photofile[0]['filepath']; 
/*
if(@file_get_contents("https://graph.facebook.com/comments/?ids=".$url)) {
  $tmpC = json_decode(file_get_contents("https://graph.facebook.com/comments/?ids=".$url));
  $result = array_shift(get_object_vars($tmpC));
  $lastComment = $result->data[0]->message;
}
*/


$data_comments = wallydemo_get_data_facebook_reactions_for_nid($guid);
//var_dump($data_comments);
if($data_comments != FALSE){
	$nbcomments = count($data_comments->data);
	$lastComment = $data_comments->data[$nbcomments-1]['message'];
}

/*
$cat->name = string_to_numericentities_mod($cat->name);
$title = string_to_numericentities_mod($title);
$description = string_to_numericentities_mod($description);
$artbody = string_to_numericentities_mod($artbody);
*/


$cat->name = $cat->name;
$title = $title;
$description = $description;
$artbody = $artbody;

?>
<item>
  <category><![CDATA[<?php print $cat->name;?>]]></category>
  <title><![CDATA[<?php print $title; ?>]]></title>
  <media:content url="<?php print $photo_url;?>" />
  <media:thumbnail url="<?php print $url_thumb;?>"/>
  <media:description type="plain"><?php print $description?></media:description>
  <link><?php print $site_url ?>/xml_iphone/node/<?php print $guid ?>?format=-xml-iphone-node</link>
  <description><?php print $description;?></description>
  <artbody><![CDATA[<?php print $artbody;?>]]></artbody>
  <author><![CDATA[R&#233;daction en ligne]]></author>
  <pubDate><?php print $date;?></pubDate>
  <guid isPermaLink="false"><?php print $guid; ?></guid>
  <moburl url="<?php print $moburl; ?>" />
  <iphonurl url="<?php print $moburl; ?>" />
  <?php if ($lastComment) {?>  
  <lastcomment><?php print $lastComment ?></lastcomment>  
  <nbcomments><?php print $nbcomments; ?></nbcomments>
  <?php } ?>
  <?php if (count($allLink) > 0) {?>
  <related>
  	<?php foreach ($allLink as $l) { ?>
	<relatedArticle id="<?php print urlencode($l["path"]); ?>"><![CDATA[<?php print $l["title"] ?>]]></relatedArticle>
	<?php } ?>
  </related>
  <?php } ?>
</item>

