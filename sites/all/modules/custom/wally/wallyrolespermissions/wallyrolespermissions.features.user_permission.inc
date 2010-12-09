<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function wallyrolespermissions_user_default_permissions() {
  $permissions = array();

  // Exported permission: Use PHP input for field settings (dangerous - grant with care)
  $permissions['Use PHP input for field settings (dangerous - grant with care)'] = array(
    'name' => 'Use PHP input for field settings (dangerous - grant with care)',
    'roles' => array(),
  );

  // Exported permission: access administration menu
  $permissions['access administration menu'] = array(
    'name' => 'access administration menu',
    'roles' => array(),
  );

  // Exported permission: access administration pages
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(),
  );

  // Exported permission: access all views
  $permissions['access all views'] = array(
    'name' => 'access all views',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: access comments
  $permissions['access comments'] = array(
    'name' => 'access comments',
    'roles' => array(
      '0' => 'authenticated user',
      '1' => 'editor user',
    ),
  );

  // Exported permission: access content
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
      '2' => 'editor user',
    ),
  );

  // Exported permission: access site reports
  $permissions['access site reports'] = array(
    'name' => 'access site reports',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: access statistics
  $permissions['access statistics'] = array(
    'name' => 'access statistics',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: access user profiles
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(),
  );

  // Exported permission: admin pollfield field_poll
  $permissions['admin pollfield field_poll'] = array(
    'name' => 'admin pollfield field_poll',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: administer actions
  $permissions['administer actions'] = array(
    'name' => 'administer actions',
    'roles' => array(),
  );

  // Exported permission: administer blocks
  $permissions['administer blocks'] = array(
    'name' => 'administer blocks',
    'roles' => array(),
  );

  // Exported permission: administer comments
  $permissions['administer comments'] = array(
    'name' => 'administer comments',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: administer content types
  $permissions['administer content types'] = array(
    'name' => 'administer content types',
    'roles' => array(),
  );

  // Exported permission: administer date tools
  $permissions['administer date tools'] = array(
    'name' => 'administer date tools',
    'roles' => array(),
  );

  // Exported permission: administer features
  $permissions['administer features'] = array(
    'name' => 'administer features',
    'roles' => array(),
  );

  // Exported permission: administer files
  $permissions['administer files'] = array(
    'name' => 'administer files',
    'roles' => array(),
  );

  // Exported permission: administer filters
  $permissions['administer filters'] = array(
    'name' => 'administer filters',
    'roles' => array(),
  );

  // Exported permission: administer imageapi
  $permissions['administer imageapi'] = array(
    'name' => 'administer imageapi',
    'roles' => array(),
  );

  // Exported permission: administer imagecache
  $permissions['administer imagecache'] = array(
    'name' => 'administer imagecache',
    'roles' => array(),
  );

  // Exported permission: administer languages
  $permissions['administer languages'] = array(
    'name' => 'administer languages',
    'roles' => array(),
  );

  // Exported permission: administer menu
  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(),
  );

  // Exported permission: administer nodes
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: administer permissions
  $permissions['administer permissions'] = array(
    'name' => 'administer permissions',
    'roles' => array(),
  );

  // Exported permission: administer search
  $permissions['administer search'] = array(
    'name' => 'administer search',
    'roles' => array(),
  );

  // Exported permission: administer site configuration
  $permissions['administer site configuration'] = array(
    'name' => 'administer site configuration',
    'roles' => array(),
  );

  // Exported permission: administer taxonomy
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: administer url aliases
  $permissions['administer url aliases'] = array(
    'name' => 'administer url aliases',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: administer users
  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(),
  );

  // Exported permission: administer views
  $permissions['administer views'] = array(
    'name' => 'administer views',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: change own username
  $permissions['change own username'] = array(
    'name' => 'change own username',
    'roles' => array(),
  );

  // Exported permission: create url aliases
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_articlepackage content
  $permissions['create wally_articlepackage content'] = array(
    'name' => 'create wally_articlepackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_audioobject content
  $permissions['create wally_audioobject content'] = array(
    'name' => 'create wally_audioobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_digitalobject content
  $permissions['create wally_digitalobject content'] = array(
    'name' => 'create wally_digitalobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_entitytype content
  $permissions['create wally_entitytype content'] = array(
    'name' => 'create wally_entitytype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_gallerypackage content
  $permissions['create wally_gallerypackage content'] = array(
    'name' => 'create wally_gallerypackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_linkslistobject content
  $permissions['create wally_linkslistobject content'] = array(
    'name' => 'create wally_linkslistobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_linktype content
  $permissions['create wally_linktype content'] = array(
    'name' => 'create wally_linktype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_locationtype content
  $permissions['create wally_locationtype content'] = array(
    'name' => 'create wally_locationtype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_persontype content
  $permissions['create wally_persontype content'] = array(
    'name' => 'create wally_persontype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_photoobject content
  $permissions['create wally_photoobject content'] = array(
    'name' => 'create wally_photoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_pollobject content
  $permissions['create wally_pollobject content'] = array(
    'name' => 'create wally_pollobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_pollpackage content
  $permissions['create wally_pollpackage content'] = array(
    'name' => 'create wally_pollpackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: create wally_textobject content
  $permissions['create wally_textobject content'] = array(
    'name' => 'create wally_textobject content',
    'roles' => array(),
  );

  // Exported permission: create wally_videoobject content
  $permissions['create wally_videoobject content'] = array(
    'name' => 'create wally_videoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_articlepackage content
  $permissions['delete any wally_articlepackage content'] = array(
    'name' => 'delete any wally_articlepackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_audioobject content
  $permissions['delete any wally_audioobject content'] = array(
    'name' => 'delete any wally_audioobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_digitalobject content
  $permissions['delete any wally_digitalobject content'] = array(
    'name' => 'delete any wally_digitalobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_entitytype content
  $permissions['delete any wally_entitytype content'] = array(
    'name' => 'delete any wally_entitytype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_gallerypackage content
  $permissions['delete any wally_gallerypackage content'] = array(
    'name' => 'delete any wally_gallerypackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_linkslistobject content
  $permissions['delete any wally_linkslistobject content'] = array(
    'name' => 'delete any wally_linkslistobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_linktype content
  $permissions['delete any wally_linktype content'] = array(
    'name' => 'delete any wally_linktype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_locationtype content
  $permissions['delete any wally_locationtype content'] = array(
    'name' => 'delete any wally_locationtype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_persontype content
  $permissions['delete any wally_persontype content'] = array(
    'name' => 'delete any wally_persontype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_photoobject content
  $permissions['delete any wally_photoobject content'] = array(
    'name' => 'delete any wally_photoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_pollobject content
  $permissions['delete any wally_pollobject content'] = array(
    'name' => 'delete any wally_pollobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_pollpackage content
  $permissions['delete any wally_pollpackage content'] = array(
    'name' => 'delete any wally_pollpackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_textobject content
  $permissions['delete any wally_textobject content'] = array(
    'name' => 'delete any wally_textobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete any wally_videoobject content
  $permissions['delete any wally_videoobject content'] = array(
    'name' => 'delete any wally_videoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_articlepackage content
  $permissions['delete own wally_articlepackage content'] = array(
    'name' => 'delete own wally_articlepackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_audioobject content
  $permissions['delete own wally_audioobject content'] = array(
    'name' => 'delete own wally_audioobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_digitalobject content
  $permissions['delete own wally_digitalobject content'] = array(
    'name' => 'delete own wally_digitalobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_entitytype content
  $permissions['delete own wally_entitytype content'] = array(
    'name' => 'delete own wally_entitytype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_gallerypackage content
  $permissions['delete own wally_gallerypackage content'] = array(
    'name' => 'delete own wally_gallerypackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_linkslistobject content
  $permissions['delete own wally_linkslistobject content'] = array(
    'name' => 'delete own wally_linkslistobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_linktype content
  $permissions['delete own wally_linktype content'] = array(
    'name' => 'delete own wally_linktype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_locationtype content
  $permissions['delete own wally_locationtype content'] = array(
    'name' => 'delete own wally_locationtype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_persontype content
  $permissions['delete own wally_persontype content'] = array(
    'name' => 'delete own wally_persontype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_photoobject content
  $permissions['delete own wally_photoobject content'] = array(
    'name' => 'delete own wally_photoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_pollobject content
  $permissions['delete own wally_pollobject content'] = array(
    'name' => 'delete own wally_pollobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_pollpackage content
  $permissions['delete own wally_pollpackage content'] = array(
    'name' => 'delete own wally_pollpackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_textobject content
  $permissions['delete own wally_textobject content'] = array(
    'name' => 'delete own wally_textobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete own wally_videoobject content
  $permissions['delete own wally_videoobject content'] = array(
    'name' => 'delete own wally_videoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: delete revisions
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: display drupal links
  $permissions['display drupal links'] = array(
    'name' => 'display drupal links',
    'roles' => array(),
  );

  // Exported permission: edit any wally_articlepackage content
  $permissions['edit any wally_articlepackage content'] = array(
    'name' => 'edit any wally_articlepackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_audioobject content
  $permissions['edit any wally_audioobject content'] = array(
    'name' => 'edit any wally_audioobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_digitalobject content
  $permissions['edit any wally_digitalobject content'] = array(
    'name' => 'edit any wally_digitalobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_entitytype content
  $permissions['edit any wally_entitytype content'] = array(
    'name' => 'edit any wally_entitytype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_gallerypackage content
  $permissions['edit any wally_gallerypackage content'] = array(
    'name' => 'edit any wally_gallerypackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_linkslistobject content
  $permissions['edit any wally_linkslistobject content'] = array(
    'name' => 'edit any wally_linkslistobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_linktype content
  $permissions['edit any wally_linktype content'] = array(
    'name' => 'edit any wally_linktype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_locationtype content
  $permissions['edit any wally_locationtype content'] = array(
    'name' => 'edit any wally_locationtype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_persontype content
  $permissions['edit any wally_persontype content'] = array(
    'name' => 'edit any wally_persontype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_photoobject content
  $permissions['edit any wally_photoobject content'] = array(
    'name' => 'edit any wally_photoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_pollobject content
  $permissions['edit any wally_pollobject content'] = array(
    'name' => 'edit any wally_pollobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_pollpackage content
  $permissions['edit any wally_pollpackage content'] = array(
    'name' => 'edit any wally_pollpackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_textobject content
  $permissions['edit any wally_textobject content'] = array(
    'name' => 'edit any wally_textobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit any wally_videoobject content
  $permissions['edit any wally_videoobject content'] = array(
    'name' => 'edit any wally_videoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_articlepackage content
  $permissions['edit own wally_articlepackage content'] = array(
    'name' => 'edit own wally_articlepackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_audioobject content
  $permissions['edit own wally_audioobject content'] = array(
    'name' => 'edit own wally_audioobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_digitalobject content
  $permissions['edit own wally_digitalobject content'] = array(
    'name' => 'edit own wally_digitalobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_entitytype content
  $permissions['edit own wally_entitytype content'] = array(
    'name' => 'edit own wally_entitytype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_gallerypackage content
  $permissions['edit own wally_gallerypackage content'] = array(
    'name' => 'edit own wally_gallerypackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_linkslistobject content
  $permissions['edit own wally_linkslistobject content'] = array(
    'name' => 'edit own wally_linkslistobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_linktype content
  $permissions['edit own wally_linktype content'] = array(
    'name' => 'edit own wally_linktype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_locationtype content
  $permissions['edit own wally_locationtype content'] = array(
    'name' => 'edit own wally_locationtype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_persontype content
  $permissions['edit own wally_persontype content'] = array(
    'name' => 'edit own wally_persontype content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_photoobject content
  $permissions['edit own wally_photoobject content'] = array(
    'name' => 'edit own wally_photoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_pollobject content
  $permissions['edit own wally_pollobject content'] = array(
    'name' => 'edit own wally_pollobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_pollpackage content
  $permissions['edit own wally_pollpackage content'] = array(
    'name' => 'edit own wally_pollpackage content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_textobject content
  $permissions['edit own wally_textobject content'] = array(
    'name' => 'edit own wally_textobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: edit own wally_videoobject content
  $permissions['edit own wally_videoobject content'] = array(
    'name' => 'edit own wally_videoobject content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: flush imagecache
  $permissions['flush imagecache'] = array(
    'name' => 'flush imagecache',
    'roles' => array(),
  );

  // Exported permission: manage features
  $permissions['manage features'] = array(
    'name' => 'manage features',
    'roles' => array(),
  );

  // Exported permission: post comments
  $permissions['post comments'] = array(
    'name' => 'post comments',
    'roles' => array(
      '0' => 'authenticated user',
      '1' => 'editor user',
    ),
  );

  // Exported permission: post comments without approval
  $permissions['post comments without approval'] = array(
    'name' => 'post comments without approval',
    'roles' => array(
      '0' => 'authenticated user',
      '1' => 'editor user',
    ),
  );

  // Exported permission: revert revisions
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(),
  );

  // Exported permission: search content
  $permissions['search content'] = array(
    'name' => 'search content',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: select different theme
  $permissions['select different theme'] = array(
    'name' => 'select different theme',
    'roles' => array(),
  );

  // Exported permission: submit latitude/longitude
  $permissions['submit latitude/longitude'] = array(
    'name' => 'submit latitude/longitude',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: translate interface
  $permissions['translate interface'] = array(
    'name' => 'translate interface',
    'roles' => array(),
  );

  // Exported permission: upload files
  $permissions['upload files'] = array(
    'name' => 'upload files',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: use PHP for block visibility
  $permissions['use PHP for block visibility'] = array(
    'name' => 'use PHP for block visibility',
    'roles' => array(),
  );

  // Exported permission: use advanced search
  $permissions['use advanced search'] = array(
    'name' => 'use advanced search',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: use views exporter
  $permissions['use views exporter'] = array(
    'name' => 'use views exporter',
    'roles' => array(),
  );

  // Exported permission: view date repeats
  $permissions['view date repeats'] = array(
    'name' => 'view date repeats',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: view location directory
  $permissions['view location directory'] = array(
    'name' => 'view location directory',
    'roles' => array(),
  );

  // Exported permission: view node location table
  $permissions['view node location table'] = array(
    'name' => 'view node location table',
    'roles' => array(),
  );

  // Exported permission: view pollfield results field_poll
  $permissions['view pollfield results field_poll'] = array(
    'name' => 'view pollfield results field_poll',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: view post access counter
  $permissions['view post access counter'] = array(
    'name' => 'view post access counter',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: view revisions
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(),
  );

  // Exported permission: view uploaded files
  $permissions['view uploaded files'] = array(
    'name' => 'view uploaded files',
    'roles' => array(
      '0' => 'editor user',
    ),
  );

  // Exported permission: view user location table
  $permissions['view user location table'] = array(
    'name' => 'view user location table',
    'roles' => array(),
  );

  return $permissions;
}
