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
$tid = $variables["view"]->args[0];
$tObject = taxonomy_get_term($tid);
$tname = $tObject->name;
// Get the string name of the current domain
$domain_url = $_SERVER["SERVER_NAME"];
$domain = spdatastructure_getdomain($domain_url);
$namespaces = str_replace('xmlns:dc="http://purl.org/dc/elements/1.1/"','',$namespaces);
?>
<rss version="2.0" xml:base="<?php print $link; ?>"<?php print $namespaces; ?>>
  <channel>
    <title><?php print $tname ?> - <?php print $domain; ?>.be</title>
    <link><?php print drupal_get_path_alias($link); ?></link>
    <description>Flux d'informations de la catégorie <?php print strtolower($tname); ?> du site <?php print $domain; ?>.be.</description>
    <language><?php print $langcode; ?></language>
    <copyright>Sudpresse - <?php print date('Y',time()); ?></copyright>
    <pubDate><?php print date('r',time()); ?></pubDate>
    <lastBuildDate><?php print date('r',time()); ?></lastBuildDate>
    <docs>http://cyber.law.harvard.edu/rss/rss.html</docs>
    <generator>Sudpresse - 1.5</generator>
    <managingEditor>webmaster@sudpresse.be (Webmaster Sudpresse)</managingEditor>
    <webMaster>webmaster@sudpresse.be (Webmaster Sudpresse)</webMaster>    
    <ttl>3</ttl>
    <image>
      <title><?php print $domain; ?>.be</title>
      <url>http://www.sudinfo.be/sites/all/themes/custom_sp/images/logos/logo_<?php print $domain; ?>.gif</url>
      <link><?php print $link; ?></link>
      <width>91</width>
      <height>29</height>
    </image>
    <?php print $channel_elements; ?>
    <?php print $items; ?>
  </channel>
</rss>