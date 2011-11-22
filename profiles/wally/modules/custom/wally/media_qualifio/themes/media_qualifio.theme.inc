<?php
// $Id: media_qualifio.theme.inc $

/**
 * @file media_qualifio/themes/media_qualifio.theme.inc
 *
 * Theme preprocess functions.
 */

/**
 * Template preprocess function for theme('media_qualifio', $media, $variables).
 */
function template_preprocess_media_qualifio(&$variables) {
  $variables = array_merge($variables, $variables['variables']);
}

