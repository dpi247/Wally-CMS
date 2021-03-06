<?php
// $Id: views-view-row-rss.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss.tpl.php
 * Default view template to display a item in a Flow feed.
 *
 * @ingroup views_templates
 */
?>
<item>
  <title><?php print $title; ?></title>
  <link><?php print $link; ?></link>
  <description><?php print $description;?></description>
  <?php print $item_elements;?>
  <?php if (!empty($url)){?>
  <media:content url="<?php print $url;?>" type="<?php print $type;?>" fileSize="<?php print $filesize;?>">
  <media:title><?php print $title;?></media:title>
  <media:thumbnail url="<?php print $url_thumb;?>"/>
  <media:description type="html"><?php print $description?></media:description>
  </media:content>
  <?php }?>
</item>