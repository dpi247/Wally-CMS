<?php
// $Id: views-view-rss.tpl.php,v 1.3 2008/12/02 00:02:06 merlinofchaos Exp $
/**
 * @file views-view-rss.tpl.php
 * Default template for feed displays that use the RSS style.
 *
 * @ingroup views_templates
 */

drupal_set_header('Content-Type: application/json; charset=utf-8');
?>

{ 
  'main':{
    'title':<?php print $title; ?>,
    'link':<?php print $link; ?>,
    'description':<?php print $description; ?>,
    'language':<?php print $langcode; ?>,
    'copyright':<?php print $copyright; ?>,
    'ttl':2,
    'image':{
      'title':<?php print $title; ?>,
      'url':<?php print $url; ?>,
      'link':<?php print $link; ?>,
    },
    items:[
    <?php print $items; ?>
    ]
  }
}
