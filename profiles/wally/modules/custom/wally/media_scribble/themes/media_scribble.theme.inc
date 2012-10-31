<?php
// $Id: media_scribble.theme.inc $

/**
 * @file media_scribble/themes/media_scribble.theme.inc
 *
 * Theme preprocess functions.
 */

/**
 * Template preprocess function for theme('media_scribble', $media, $variables).
 */
function template_preprocess_media_scribble(&$variables) {
  $variables = array_merge($variables, $variables['variables']);
}
