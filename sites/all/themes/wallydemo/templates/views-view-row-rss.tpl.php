<?php
// $Id: views-view-row-rss.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss.tpl.php
 * Default view template to display a item in a Flow feed.
 *
 * @ingroup views_templates
 * 
 */
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

_wallydemo_prepare_publication_dates($node);
$date_edition = _wallydemo_get_edition_date($node, 'flux_rss');

if ($mainstory->type == "wally_textobject"){
   $strapline = preg_replace('/<(.*)>/iU', '', $mainstory->field_textchapo[0]['value']);
}else{
   $strapline = preg_replace('/<(.*)>/iU', '', $mainstory->field_summary[0]['value']);
}
$package_signature = _wallydemo_get_package_signature($mainstory) ;
$main_title = $mainstory->title;
$maindestination = taxonomy_get_term($maindestination_tid);
$maindestination_name = $maindestination->name;
$nodeUrl = "http://".$_SERVER["SERVER_NAME"]."/".drupal_get_path_alias('node/'.$node->nid);
$nodeCommentsUrl = $nodeUrl."#ancre_commentaires";
?><item>
  <category><![CDATA[<?php print $maindestination_name; ?>]]></category>
  <title><![CDATA[<?php print strip_tags(html_entity_decode($title)); ?>]]></title>
  <link><?php print $nodeUrl; ?></link>
  <description><![CDATA[<?php print "<img src=\"$photoUrl\" width=\"120\" align=\"left\" hspace=\"4\" vspace=\"4\" />"?> <?php print $strapline; ?>]]></description>
  <comments><?php print $nodeCommentsUrl; ?></comments>  
  <pubDate><?php print $date_edition; ?></pubDate>
  <author><?php print $package_signature; ?></author>
  <guid isPermaLink="false"><?php print $node->nid; ?> at http://<?php print $_SERVER["SERVER_NAME"]; ?></guid>
  <?php if ($photo == TRUE){?>
  <media:content url="<?php print $photoUrl;?>" type="<?php print $photoType;?>" fileSize="<?php print $photoObject_size;?>">
  <media:title><?php print $photoSummary;?></media:title>
  <media:thumbnail url="<?php print $photoThumbUrl;?>"/>
  </media:content>
  <?php }?>
</item>