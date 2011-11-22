<?php
// $Id: media-dailymotion-default.tpl.php,v 1.0 2011/01/07 ODM $

/**
 * @file media_dailymotion/themes/media-dailymotion-default.tpl.php
 *
 * Template file for Media: dailymotion's theme('media_dailymotion_default_external').
 *
 * This will display dailymotion's default embedded video.
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
<div id="media-dailymotion-default-external-<?php print $options['video_id']; ?>">
<iframe frameborder="0" width="<?php print $options['width']; ?>" height="<?php print $options['height']; ?>" src="http://www.dailymotion.com/embed/video/<?php print $options['video_id']; ?>"></iframe> 
</div>