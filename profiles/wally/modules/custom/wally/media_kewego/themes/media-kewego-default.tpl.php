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
 
$options = $variables['variables'];
?>
<div id="media-kewego-default-external-<?php print $options['video_id']; ?>">
  <object id="<?php print $options['video_id']; ?>" type="application/x-shockwave-flash" data="http://www.kewego.com/swf/p3/epix.swf" width="<?php print $options['width']; ?>" height="<?php print $options['height']; ?>">
    <param name="flashVars" value="language_code=<?php print $language; ?>&playerKey=037fg546ekd7&skinKey=&sig=<?php print $options['video_id']; ?>&autostart=<?php print $options['autoplay']; ?>&videoformat=<?php print $options['videoformat']; ?>" />
    <param name="movie" value="http://www.kewego.com/swf/p3/epix.swf" />
    <param name="allowFullScreen" value="<?php print $options['fullscreen']; ?>" />
    <param name="allowscriptaccess" value="always" />
  </object>
</div>
