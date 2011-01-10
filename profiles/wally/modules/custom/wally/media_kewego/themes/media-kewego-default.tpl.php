<?php
// $Id: media-kewego-default.tpl.php,v 1.0 2011/01/07 ODM $

/**
 * @file media_kewego/themes/media-kewego-default.tpl.php
 *
 * Template file for Media: Kewego's theme('media_kewego_default_external').
 *
 * This will display Kewego's default embedded video.
 *
 *  This is the fallback display, for when we don't have SWF Object or
 *  JW Flash Media Player.
 */
 
$video_id = $variables['variables']['video_id'];
$video_id = explode('.', $video_id);
$video_id = $video_id[0];
$width = media_kewego_variable_get('width') ? media_kewego_variable_get('width') : EMVIDEO_DEFAULT_VIDEO_WIDTH;
$height = media_kewego_variable_get('height') ? media_kewego_variable_get('height') : EMVIDEO_DEFAULT_VIDEO_HEIGHT;
$autostart = media_kewego_variable_get('autostart') == 1 ? 'true' : 'false';
$fullscreen = media_kewego_variable_get('fullscreen') == 1 ? 'true' : 'false';
$videoformat = media_kewego_variable_get('videoformat') ? media_kewego_variable_get('videoformat') : 'old';
$language = media_kewego_variable_get('language') ? media_kewego_variable_get('language') : 'en';
dsm($variables);
?>
<div id="media-kewego-default-external-<?php print $video_id; ?>">
  <object id="<?php print $video_id; ?>" type="application/x-shockwave-flash" data="http://www.kewego.com/swf/p3/epix.swf" width="<?php print $width; ?>" height="<?php print $height; ?>">
    <param name="flashVars" value="language_code=<?php print $language; ?>&playerKey=037fg546ekd7&skinKey=&sig=<?php print $video_id; ?>&autostart=<?php print $autostart; ?>&videoformat=<?php print $videoformat; ?>" />
    <param name="movie" value="http://www.kewego.com/swf/p3/epix.swf" />
    <param name="allowFullScreen" value="<?php print $fullscreen; ?>" />
    <param name="allowscriptaccess" value="always" />
  </object>
</div>
