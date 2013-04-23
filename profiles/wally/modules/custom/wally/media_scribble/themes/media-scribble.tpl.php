<?php
// $Id: media-coveritlive.tpl.php $

/**
 * @file media_coveritlive/themes/media-coveritlive.tpl.php
 *
 * Template file for theme('media_coveritlive', $media, $variables).
 */
?>

<iframe src='http://embed.scribblelive.com/Embed/v5.aspx?Id=<?php print $media?>&ThemeId=<?php print $theme_number?>' width='<?php print $width; ?>' height='<?php print $height; ?>' frameborder='0' style='border: 1px solid #000'></iframe>

