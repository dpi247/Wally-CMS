<?php
// $Id: views-view-row-rss-mobile.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss-mobile.tpl.php
 * Default view template to display a item in a Flow feed for Mobile application.
 *
 * @ingroup views_templates
 */
$guid = $node->nid;
$maindestination_tid = $node->field_destinations[0]["tid"];
$photo = FALSE;
$photoObject_path = "";
if($node->type == "wally_articlepackage"){
  $mainstory = $node->field_mainstory_nodes[0];
} else {  
  $mainstory = $node->field_mainobject_nodes[0];
  $mainstory_type = $mainstory->type;
  if($mainstory_type == "wally_photoobject"){ 
    $photoObject_path = $mainstory->field_photofile[0]['filepath'];
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size == $mainstory->field_photofile[0]['filesize'];
    if (isset($photoObject_path) && $photoObject_size > 0) {
      $photo = TRUE;
    }
  }
}
if ($photoObject_path == ""){
  $embeded_objects = $node->field_embededobjects_nodes;
  $photoObject = wallydemo_get_first_photoEmbededObject_from_package($embeded_objects);
  If ($photoObject) {
    $photoObject_path = $photoObject->field_photofile[0]['filepath'];
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
    if (isset($photoObject_path) && $photoObject_size > 0) {
      $photo = TRUE;
    }    
  }
}
if ($photo == TRUE){
  $file_img = theme('imagecache', 'flowpublish_preset',  $photoObject_path);
  $path_photo = imagecache_create_url('flowpublish_preset', $photoObject_path);
  $path_photo_thumb = imagecache_create_url('flowpublish_thumb_preset', $photoObject_path);
  $photoUrl = $path_photo;
  $photoThumbUrl = $path_photo_thumb;
  $photoSummary = preg_replace('/<(.*)>/iU', '', $photoObject->field_summary[0]['value']);
  $photoSummary = preg_replace('/&(.*);/iU', '', $photoSummary);
  $photoType = $photoObject->field_photofile[0]['filemime'];
}
$node_publi_date = strtotime($node->field_publicationdate[0]['value']);
// HOTFIX RED #7055
$node_publi_date = $node_publi_date + 7200;
$date_edition = _wallydemo_date_edition_diplay($node_publi_date, 'flux_rss');
if ($mainstory->type == "wally_textobject"){
   $strapline = preg_replace('/<(.*)>/iU', '', $mainstory->field_textchapo[0]['value']);
   // RED #7060
   $strapline = str_replace('&rsquo;', "'", $strapline);
   $strapline = str_replace('&#39;', "'", $strapline);   
}else{
   $strapline = preg_replace('/<(.*)>/iU', '', $mainstory->field_summary[0]['value']);
   // RED #7060
   $strapline = str_replace('&rsquo;', "'", $strapline);
   $strapline = str_replace('&#39;', "'", $strapline);   
}
$package_signature = _wallydemo_get_package_signature($mainstory) ;
$title = $mainstory->title;
$artbody = preg_replace('/<(.*)>/iU', '', $mainstory->field_textbody[0]['value']);
// RED #7060
$artbody = str_replace('&rsquo;', "'", $artbody);
$artbody = str_replace('&#39;', "'", $artbody);

$maindestination = taxonomy_get_term($maindestination_tid);
$maindestination_name = $maindestination->name;

$nodeUrl = "http://".$_SERVER["SERVER_NAME"]."/".drupal_get_path_alias('node/'.$node->nid);
$moburl = str_replace("/www.", "/mobile.", $nodeUrl);

/* Récupération de l'alias de l'url du package -> $node_path
 * 
 * print($node_path);
 */
//$aliases = wallytoolbox_get_path_aliases("node/".$node->nid);
$aliases = wallytoolbox_get_all_aliases("node/".$node->nid);
$node_path = $aliases[0];
$socialSharingBaseUrl = wallydemo_get_social_sharing_base_url($mainDestination,$domain);
$socialSharingDomainAndPathUrl = $socialSharingBaseUrl."/".$node_path;

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

$data_comments = wallydemo_get_data_facebook_reactions_for_nid($guid);
if($data_comments != FALSE){
	$nbcomments = count($data_comments->data);
	$lastComment = $data_comments->data[$nbcomments-1]['message'];
}

?>
<item>
  <category><![CDATA[<?php print $maindestination_name;?>]]></category>
  <title><![CDATA[<?php print strip_tags(html_entity_decode($title)); ?>]]></title>
  <media:content url="<?php print $photoUrl; ?>" />
  <media:thumbnail url="<?php print $photoThumbUrl; ?>"/>
  <media:description type="plain"><![CDATA[<?php print wallydemo_check_plain($strapline); ?>]]></media:description>
  <link><?php print $site_url ?>/xml_iphone/node/<?php print $guid ?>?format=-xml-iphone-node</link>
  <description><![CDATA[<?php print wallydemo_check_plain($strapline);?>]]></description>
  <artbody><![CDATA[<?php print wallydemo_check_plain($artbody);?>]]></artbody>
  <author><![CDATA[<?php print $package_signature ; ?>]]></author>
  <pubDate><?php print $date_edition;?></pubDate>
  <guid isPermaLink="false"><?php print $guid; ?></guid>
  <moburl url="<?php print $moburl; ?>" />
  <iphonurl url="<?php print $moburl; ?>" />
  <soirurl url="<?php print $socialSharingDomainAndPathUrl ; ?>" />
  <shareurl url="<?php print $socialSharingDomainAndPathUrl ; ?>" />
  <articleurl url="<?php print $nodeUrl ; ?>" />
  <?php if ($lastComment) {?>  
  <lastcomment><![CDATA[<?php print strip_tags($lastComment); ?>]]></lastcomment>  
  <nbcomments><![CDATA[<?php print $nbcomments; ?>]]></nbcomments>
  <?php } ?>
  <?php if (count($allLink) > 0) {?>
  <related>
  	<?php foreach ($allLink as $l) { ?>
	<relatedArticle id="<?php print urlencode($l["path"]); ?>"><![CDATA[<?php print $l["title"] ?>]]></relatedArticle>
	<?php } ?>
  </related>
  <?php } ?>
</item>

