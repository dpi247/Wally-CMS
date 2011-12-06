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
<?php
// Old skool
/*
<div id="media-kewego-default-external-<?php print $video_id; ?>">
  <object id="<?php print $video_id; ?>" type="application/x-shockwave-flash" data="http://www.kewego.com/swf/p3/epix.swf" width="<?php print $options['width']; ?>" height="<?php print $options['height']; ?>">
    <param name="flashVars" value="language_code=<?php print $language; ?>&playerKey=037fg546ekd7&skinKey=&sig=<?php print $video_id; ?>&autostart=<?php print $options['autoplay']; ?>&videoformat=<?php print $options['videoformat']; ?>" />
    <param name="movie" value="http://www.kewego.com/swf/p3/epix.swf" />
    <param name="allowFullScreen" value="<?php print $options['fullscreen']; ?>" />
    <param name="allowscriptaccess" value="always" />
  </object>
</div>
*/
?>

<object width="<?php print $options['width'];?>" height="<?php print $options['height'];?>" type="application/x-shockwave-flash" id="<?php print $video_id;?>" data="http://sll.kewego.com/swf/p3/epix.swf">
  <param value="language_code=<?php print $options['language'];?>&amp;playerKey=<?php print $options['player_key'];?>&amp;skinKey=<?php print $options['skin_key'];?>&amp;sig=<?php print $video_id;?>&amp;autostart=<?php print $options['autostart'];?>&amp;advertise=<?php print $options['advertise'];?>" name="flashVars"><param value="http://sll.kewego.com/swf/p3/epix.swf" name="movie">
  <param value="<?php print $options['fullscreen'];?>" name="allowFullScreen">
  <param value="<?php print $options['allow_script_access'];?>" name="allowscriptaccess">
  <param value="<?php print $options['wmode'];?>" name="wmode">
  <video width="<?php print $options['width'];?>" height="<?php print $options['height'];?>" preload="<?php print $options['preload'];?>" poster="http://api.kewego.com/video/getHTML5Thumbnail/?playerKey=<?php print $options['player_key'];?>&amp;sig=<?php print $video_id;?>" controls="controls">&nbsp;</video>

  <script src="//sll.kewego.com/embed/assets/kplayer-standalone.js"></script>
  <script defer="defer">kitd.html5loader("flash_epix_<?php print $video_id;?>");</script>
</object>
