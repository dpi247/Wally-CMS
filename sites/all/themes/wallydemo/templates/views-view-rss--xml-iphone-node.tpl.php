<?php
// $Id: views-view-rss.tpl.php,v 1.3 2008/12/02 00:02:06 merlinofchaos Exp $
/**
 * @file views-view-rss.tpl.php
 * Default template for feed displays that use the RSS style.
 *
 * @ingroup views_templates
 */

// Ne marche pas, theme va chercher views-view-rss.tpl.php de views
//drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');

$tid = explode('/', $link);
$nbTmp = count($tid)-1;
$tid = $tid[$nbTmp];
$destName = taxonomy_get_term($tid);

drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
print '<?xml version="1.0" encoding="utf-8" ?>\n';

?>
<rss version="2.0" xmlns:media="http://search.yahoo.com/mrss">
	<channel>
		<title>sudinfo.be-iPhone: <?php print $destName->name; ?></title>
		<link><?php print $link; ?></link>
		<description><?php print $destName->name; ?></description> 
		<language>fr</language>
		<copyright>Rossel et Cie SA,  Sudpresse, Bruxelles, 2012</copyright>
		<pubDate>Thu, 27 Oct 2011 16:28:00 +0200</pubDate>
		<lastBuildDate>Thu, 27 Oct 2011 16:28:00 +0200</lastBuildDate>
		<docs>http://backend.userland.com/rss</docs>
		<generator>GPS-SELRSS v2.0</generator>
		<managingEditor>redaction@sudpresse.be</managingEditor>
		<webMaster>redaction@sudpresse.be</webMaster>
		<ttl>1</ttl>
        <image>
        <title>Sudpresse.be</title>
        <width>180</width>
        <height>39</height>
        <link>http://www.sudinfo.be</link>
        <url>http://www.sudinfo.be/sites/all/themes/wallydemo/images/logos/logo_sudinfo.gif</url> 
        </image> 
        <?php print $channel_elements; ?>
        <?php print $items; ?>
  	</channel>
</rss>