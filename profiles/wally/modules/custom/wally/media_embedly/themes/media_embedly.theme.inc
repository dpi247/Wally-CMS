<?php
// $Id: media_embedly.theme.inc,v 1.0 2011/01/10 ODM $

/**
 * @file
 * Theme functions for Media: Kewego.
 */

function template_preprocess_media_embedly_default(&$variables) {
  // Merge in our default variables.
  $variables = array_merge($variables, $variables['variables']);
}
