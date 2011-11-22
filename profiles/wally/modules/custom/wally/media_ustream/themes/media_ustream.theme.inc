<?php
// $Id: media_ustream.theme.inc,v 1.0 2011/01/10 ODM $

/**
 * @file
 * Theme functions for Media: ustream.
 */

function template_preprocess_media_ustream_default(&$variables) {
  // Merge in our default variables.
  $variables = array_merge($variables, $variables['variables']);
}
