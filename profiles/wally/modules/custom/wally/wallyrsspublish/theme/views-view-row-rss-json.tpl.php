<?php
// $Id: views-view-row-rss.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss.tpl.php
 * Default view template to display a item in an RSS feed.
 *
 * @ingroup views_templates
 */
?>
{
  'title':<?php print $title; ?>,
  'link':<?php //print $link; ?>,
  'description':<?php print $description;?>,
  'elements':{<?php print $item_elements;?>},
  <?php if (!empty($url)):?>
  media:{
    'content url':<?php print $url;?>,
    'thumbnail url':<?php print $url_thumb;?>,
  },
  <?php endif;?>
}