<?php
// $Id: views-view-row-rss.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss.tpl.php
 * Default view template to display a item in an RSS feed.
 *
 * @ingroup views_templates
 */
?>
<item>
<title><?php print $title; ?></title>
<link>
<?php print $link; ?>
</link>
<description>
&lt;img src="<?php print $url;?>" width="120" align="left" hspace="4" vspace="4" /&gt;
<?php print $description;?>
</description>


<media:description><?php print $description;?></media:description>
<media:thumbnail
	url="<?php print $url;?>" height='360' width='480' />
<media:content
	url="<?php print $url;?>" type="<?php print $type;?>"
	fileSize="<?php print $filesize;?>" />
<media:title><?php print $title; ?></media:title>

</item>
