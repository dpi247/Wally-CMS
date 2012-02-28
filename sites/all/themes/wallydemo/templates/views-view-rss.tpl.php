<?php
// $Id: views-view-rss.tpl.php,v 1.3 2008/12/02 00:02:06 merlinofchaos Exp $
/**
 * @file views-view-rss.tpl.php
 * Default template for feed displays that use the RSS style.
 *
 * @ingroup views_templates
 */

drupal_set_header('Content-Type: text/xml; charset=utf-8');
print '<?xml version="1.0" encoding="utf-8" ?>';
print "\n";

?>
<rss version="2.0" xml:base="<?php print $link; ?>"<?php print $namespaces; ?>>
  <channel>
    <title>Sudpresse</title>
    <link><?php print $link; ?></link>
    <description><?php print $description; ?></description>
    <language><?php print $langcode; ?></language>
    <copyright><?php print $copyright; ?></copyright>
    <ttl>2</ttl>
    <image>
      <title>Sudpresse.be</title>
      <url>http://www.sudinfo.be/sites/all/themes/wallydemo/images/logos/logo_sudinfo.gif</url>
      <link><?php print $link; ?></link>
			<width>91</width>
			<height>29</height>
    </image>
    <?php print $channel_elements; ?>
    <?php print $items; ?>
  </channel>
</rss>