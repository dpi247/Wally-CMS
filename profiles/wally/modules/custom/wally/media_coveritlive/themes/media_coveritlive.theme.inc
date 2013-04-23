<?php
// $Id: media_coveritlive.theme.inc $

/**
 * @file media_coveritlive/themes/media_coveritlive.theme.inc
 *
 * Theme preprocess functions.
 */

/**
 * Template preprocess function for theme('media_coveritlive', $media, $variables).
 */
function template_preprocess_media_coveritlive(&$variables) {
  $variables = array_merge($variables, $variables['variables']);
}

