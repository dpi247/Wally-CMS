<?php
// $Id: media-coveritlive.tpl.php $

/**
 * @file media_coveritlive/themes/media-coveritlive.tpl.php
 *
 * Template file for theme('media_coveritlive', $media, $variables).
 */
?>
<iframe src="http://www.coveritlive.com/index2.php/option=com_altcaster/task=viewaltcast/altcast_code=<?php print $media; ?>/height=<?php print $height; ?>/width=<?php print $width; ?>" scrolling="no" height="<?php print $height; ?>" width="<?php print $width; ?>" frameBorder ="0" allowTransparency="true"  ><a href="http://www.coveritlive.com/mobile.php/option=com_mobile/task=viewaltcast/altcast_code=<?php print $media; ?>" >Event</a></iframe>

