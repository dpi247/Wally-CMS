<?php
// $Id: media-ustream-default.tpl.php,v 1.0 2011/01/07 ODM $

/**
 * @file media_ustream/themes/media-ustream-default.tpl.php
 *
 * Template file for Media: ustream's theme('media_ustream_default_external').
 *
 * This will display ustream's default embedded video.
 *
 *  This is the fallback display, for when we don't have SWF Object or
 *  JW Flash Media Player.
 */
 
$options = $variables['variables'];
$expl_vid_id = explode('/', $options['video_id']);
foreach ($expl_vid_id as $id_part) {
  if (strstr($id_part, '.html')) {
    $expl_id_part = explode('.', $id_part);
    $video_id = $expl_id_part[0];
    break;
  }
  $video_id = $id_part;
}
?>
<div id="media-ustream-default-external-<?php print $video_id; ?>">
<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" width="480" height="296" id="<?php print $video_id; ?>" name="<?php print $video_id; ?>"><param name="flashvars" value="autoplay=false&vid=<?php print $video_id; ?>&v3=1" />
	<param name="allowfullscreen" value="true" />
	<param name="allowscriptaccess" value="always" />
	<param name="src" value="http://www.ustream.tv/flash/viewer.swf" />
	<embed flashvars="autoplay=false&vid=<?php print $video_id; ?>&v3=1" width="480" height="296" allowfullscreen="true" allowscriptaccess="always" id="<?php print $video_id; ?>" name="<?php print $video_id; ?>" src="http://www.ustream.tv/flash/viewer.swf" type="application/x-shockwave-flash" />
</object>
</div>
  	