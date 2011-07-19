<?php
// $Id: media_googledoc.theme.inc $

/**
 * @file media_googledoc/themes/media_googledoc.theme.inc
 *
 * Theme preprocess functions.
 */

/**
 * Template preprocess function for theme('media_googledoc', $media, $variables).
 */
function template_preprocess_media_googledoc(&$variables) {
  $variables = array_merge($variables, $variables['variables']);
}

