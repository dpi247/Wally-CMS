<?php
// $Id: media-videortl-default.tpl.php,v 1.0 2011/01/07 ODM $

/**
 * @file media_videortl/themes/media-videortl-default.tpl.php
 *
 * Template file for Media: videortl's theme('media_videortl_default_external').
 *
 * This will display videortl's default embedded video.
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
<div id="media-videortl-default-external-<?php print $video_id; ?>">
<object id="<?php print $video_id; ?>" classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" width="<?php print $options['width']; ?>" height="<?php print $options['height']; ?>">
	<param name="movie" value="http://www.rtl.be/videos/GED_vp/00230000/237200/237206.swf?bEmbed=1&bHideInfo=0&VideoID=<?php print $video_id; ?>"></param>
	<param name="allowFullScreen" value="<?php print $options['fullscreen']; ?>"></param>
	<param name="allowscriptaccess" value="always"></param>
	<embed src="http://www.rtl.be/videos/GED_vp/00230000/237200/237206.swf?bEmbed=1&bHideInfo=0&VideoID=<?php print $video_id; ?>" type="application/x-shockwave-flash" allowscriptaccess="always" allowfullscreen="true" width="600" height="338"></embed>
</object>
</div>
  	
