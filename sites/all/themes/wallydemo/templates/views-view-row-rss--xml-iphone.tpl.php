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
?>

<item>
  <title><![CDATA[<?php print $title; ?>]]></title>
  <category><![CDATA[<?php print $cat->name;?>]]></category>
  <link><?php print $link; ?>/mobile</link>
  <description><?php print $description;?></description>
  <artbody><![CDATA[<?php print $artbody;?>]]></artbody>
  <author><![CDATA[R&#233;daction en ligne]]></author>
  <pubDate><?php print $date;?></pubDate>
  <guid isPermaLink="false"><?php print $guid; ?></guid>
  <url value="<?php print $link; ?>" />
  <?php if (!empty($url)) { ?>
  <media:content url="<?php print $url;?>" type="<?php print $type;?>" fileSize="<?php print $filesize;?>">
  <media:title><?php print $title;?></media:title>
  <media:thumbnail url="<?php print $url_thumb;?>"/>
  <media:description type="html"><?php print $description?></media:description>
  </media:content>
  <?php }?>
</item>