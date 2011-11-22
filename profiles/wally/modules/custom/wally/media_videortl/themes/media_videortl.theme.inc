<?php
// $Id: media_videortl.theme.inc,v 1.0 2011/01/10 ODM $

/**
 * @file
 * Theme functions for Media: videortl.
 */

function template_preprocess_media_videortl_default(&$variables) {
  // Merge in our default variables.
  $variables = array_merge($variables, $variables['variables']);
}
