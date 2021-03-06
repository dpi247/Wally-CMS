<?php
// $Id: views-view-rss.tpl.php,v 1.3 2008/12/02 00:02:06 merlinofchaos Exp $
/**
 * @file views-view-rss.tpl.php
 * Default template for feed displays that use the RSS style.
 *
 * @ingroup views_templates
 */

// Ne marche pas, theme va chercher views-view-rss.tpl.php de views
drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');

?>
<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<rss version="2.0" xml:base="<?php print $link; ?>"<?php print $namespaces; ?>>
  <channel>
    <title><?php print $title; ?></title>
    <link><?php print $link; ?></link>
    <description><?php print $description; ?></description>
    <language><?php print $langcode; ?></language>
    <copyright><?php print $copyright; ?></copyright>
    <ttl>2</ttl>
    <image>
      <title><?php print $title; ?></title>
      <url><?php print $url; ?></url>
      <link><?php print $link; ?></link>
    </image>
    <?php print $channel_elements; ?>
    <?php print $items; ?>
  </channel>
</rss>