<?php
// $Id: media_dailymotion.theme.inc,v 1.0 2011/01/10 ODM $

/**
 * @file
 * Theme functions for Media: dailymotion.
 */

function template_preprocess_media_dailymotion_default(&$variables) {
  // Merge in our default variables.
  $variables = array_merge($variables, $variables['variables']);
}
