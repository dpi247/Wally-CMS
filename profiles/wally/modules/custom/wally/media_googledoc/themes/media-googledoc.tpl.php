<?php
// $Id: media-googledoc.tpl.php $

/**
 * @file media_googledoc/themes/media-googledoc.tpl.php
 *
 * Template file for theme('media_googledoc', $media, $variables).
 */
?>
<iframe src="https://spreadsheets.google.com/spreadsheet/embeddedform?formkey=<?php print $media; ?>" width="<?php print $width; ?>" height="<?php print $height; ?>" frameborder="0" marginheight="0" marginwidth="0">Loading...</iframe>
