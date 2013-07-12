<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wallydefaultpages_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => 1);
  }
  elseif ($module == "wallyctools" && $api == "wallyctools_redacblock") {
    return array("version" => 1);
  }
  elseif ($module == "wallyextra" && $api == "wallyextra_flowmix_content") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function wallydefaultpages_imagecache_default_presets() {
  $items = array(
    '120x120' => array(
      'presetname' => '120x120',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '120',
            'height' => '120',
          ),
        ),
      ),
    ),
    '241x150' => array(
      'presetname' => '241x150',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '241',
            'height' => '150',
          ),
        ),
      ),
    ),
    '300x300' => array(
      'presetname' => '300x300',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '300',
            'height' => '300',
          ),
        ),
      ),
    ),
    '366x228' => array(
      'presetname' => '366x228',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '366',
            'height' => '228',
          ),
        ),
      ),
    ),
    '70x70' => array(
      'presetname' => '70x70',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '70',
            'height' => '70',
          ),
        ),
      ),
    ),
    '743x430' => array(
      'presetname' => '743x430',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '743',
            'height' => '430',
          ),
        ),
      ),
    ),
    'flowpublish_preset' => array(
      'presetname' => 'flowpublish_preset',
      'actions' => array(
        '0' => array(
          'weight' => '1',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '640',
            'height' => '400',
            'upscale' => TRUE,
          ),
        ),
      ),
    ),
    'flowpublish_thumb_preset' => array(
      'presetname' => 'flowpublish_thumb_preset',
      'actions' => array(
        '0' => array(
          'weight' => '1',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '300',
            'height' => '150',
            'upscale' => TRUE,
          ),
        ),
      ),
    ),
    'jn_large' => array(
      'presetname' => 'jn_large',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => '1024',
            'height' => '768',
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '1024',
            'height' => '768',
          ),
        ),
      ),
    ),
    'jn_medium' => array(
      'presetname' => 'jn_medium',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => '600',
            'height' => '300',
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '600',
            'height' => '300',
          ),
        ),
      ),
    ),
    'jn_thumb' => array(
      'presetname' => 'jn_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => '400',
            'height' => '300',
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '400',
            'height' => '300',
          ),
        ),
      ),
    ),
    'print_182x110' => array(
      'presetname' => 'print_182x110',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => 182,
            'height' => 110,
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_resize',
          'data' => array(
            'width' => 182,
            'height' => 110,
          ),
        ),
      ),
    ),
    'print_182x176' => array(
      'presetname' => 'print_182x176',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => 182,
            'height' => 176,
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_resize',
          'data' => array(
            'width' => 182,
            'height' => 176,
          ),
        ),
      ),
    ),
    'print_182x192' => array(
      'presetname' => 'print_182x192',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => 182,
            'height' => 192,
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_resize',
          'data' => array(
            'width' => 182,
            'height' => 192,
          ),
        ),
      ),
    ),
    'print_376x208' => array(
      'presetname' => 'print_376x208',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => 376,
            'height' => 208,
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_resize',
          'data' => array(
            'width' => 376,
            'height' => 208,
          ),
        ),
      ),
    ),
    'print_376x215' => array(
      'presetname' => 'print_376x215',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => 376,
            'height' => 215,
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_resize',
          'data' => array(
            'width' => 376,
            'height' => 215,
          ),
        ),
      ),
    ),
    'print_376x220' => array(
      'presetname' => 'print_376x220',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => 376,
            'height' => 220,
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_resize',
          'data' => array(
            'width' => 376,
            'height' => 220,
          ),
        ),
      ),
    ),
    'print_376x240' => array(
      'presetname' => 'print_376x240',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => 376,
            'height' => 240,
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_resize',
          'data' => array(
            'width' => 376,
            'height' => 240,
          ),
        ),
      ),
    ),
    'print_764x463' => array(
      'presetname' => 'print_764x463',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'wallycontenttypes',
          'action' => 'wallycontenttypes_wallycrop',
          'data' => array(
            'width' => 764,
            'height' => 463,
            'xoffset' => 'center_max',
            'yoffset' => 'center_max',
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_resize',
          'data' => array(
            'width' => 764,
            'height' => 463,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function wallydefaultpages_views_api() {
  return array(
    'api' => '3.0',
  );
}
