<?php

/**
 * Implementation of hook_views_api().
 */
function cckdestinations_views_api() {
  return array(
    'api' => 2,
    'path' => drupal_get_path('module', 'cckdestinations') .'/views',
  );
}
