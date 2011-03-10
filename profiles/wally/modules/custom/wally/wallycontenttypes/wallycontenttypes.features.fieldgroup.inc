<?php

/**
 * Implementation of hook_fieldgroup_default_groups().
 */
function wallycontenttypes_fieldgroup_default_groups() {
  $groups = array();

  // Exported group: group_authoring
  $groups['wally_articlepackage-group_authoring'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_articlepackage',
    'group_name' => 'group_authoring',
    'label' => 'Authoring',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '0',
    'fields' => array(
      '0' => 'field_authors',
      '1' => 'field_packageauthors',
      '2' => 'field_embargodatetime',
      '3' => 'field_unpublishdate',
      '4' => 'field_publicationdate',
      '5' => 'field_freeaccess',
    ),
  );

  // Exported group: group_content
  $groups['wally_articlepackage-group_content'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_articlepackage',
    'group_name' => 'group_content',
    'label' => 'Content',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '1',
    'fields' => array(
      '0' => 'field_mainstory',
      '1' => 'field_embededobjects',
      '2' => 'field_linkedobjects',
      '3' => 'field_packagelayout',
    ),
  );

  // Exported group: group_destinations
  $groups['wally_articlepackage-group_destinations'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_articlepackage',
    'group_name' => 'group_destinations',
    'label' => 'Destinations',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => 'Tells where the package will be published. ',
      ),
      'display' => array(
        'description' => 'Tells where the package will be published. ',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_destinations',
      '1' => 'field_editions',
      '2' => 'field_channels',
    ),
  );

  // Exported group: group_identification
  $groups['wally_articlepackage-group_identification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_articlepackage',
    'group_name' => 'group_identification',
    'label' => 'Package Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => 'IDs; Internal, external, product, etc.',
      ),
      'display' => array(
        'description' => 'IDs; Internal, external, product, etc.',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-3',
    'fields' => array(
      '0' => 'field_packageid',
      '1' => 'field_externalreference',
      '2' => 'field_objectdescription',
      '3' => 'field_productid',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_articlepackage-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_articlepackage',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & Geolocalisation',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsible',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-1',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_entities',
      '3' => 'field_persons',
      '4' => 'field_tags',
      '5' => 'field_free_tags',
    ),
  );

  // Exported group: group_audio
  $groups['wally_audioobject-group_audio'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_audioobject',
    'group_name' => 'group_audio',
    'label' => 'Audio',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-4',
    'fields' => array(
      '0' => 'field_summary',
      '1' => 'field_audiofile',
      '2' => 'field_audio3rdparty',
      '3' => 'field_link',
      '4' => 'field_thumbnail',
      '5' => 'field_rating',
    ),
  );

  // Exported group: group_audioidentification
  $groups['wally_audioobject-group_audioidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_audioobject',
    'group_name' => 'group_audioidentification',
    'label' => ' Audio Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_externalreference',
      '1' => 'field_objectid',
      '2' => 'field_objectdescription',
    ),
  );

  // Exported group: group_authoring
  $groups['wally_audioobject-group_authoring'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_audioobject',
    'group_name' => 'group_authoring',
    'label' => 'Authoring',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-1',
    'fields' => array(
      '0' => 'field_authors',
      '1' => 'field_copyright',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_audioobject-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_audioobject',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & TME',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-3',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_tags',
      '3' => 'field_free_tags',
      '4' => 'field_entities',
      '5' => 'field_persons',
    ),
  );

  // Exported group: group_digitalidentification
  $groups['wally_digitalobject-group_digitalidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_digitalobject',
    'group_name' => 'group_digitalidentification',
    'label' => 'Digital Object Identification ',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '-2',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_externalreference',
      '1' => 'field_objectid',
      '2' => 'field_objectdescription',
    ),
  );

  // Exported group: group_digitalobject
  $groups['wally_digitalobject-group_digitalobject'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_digitalobject',
    'group_name' => 'group_digitalobject',
    'label' => 'Digital Object',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '-4',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-4',
    'fields' => array(
      '0' => 'field_summary',
      '1' => 'field_objectfile',
      '2' => 'field_object3rdparty',
      '3' => 'field_thumbnail',
      '4' => 'field_rating',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_digitalobject-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_digitalobject',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & TME',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '-3',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-3',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_tags',
      '3' => 'field_free_tags',
      '4' => 'field_entity',
      '5' => 'field_persons',
      '6' => 'field_copyright',
    ),
  );

  // Exported group: group_authoring
  $groups['wally_gallerypackage-group_authoring'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_gallerypackage',
    'group_name' => 'group_authoring',
    'label' => 'Authoring',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '0',
    'fields' => array(
      '0' => 'field_authors',
      '1' => 'field_packageauthors',
      '2' => 'field_publicationdate',
      '3' => 'field_embargodatetime',
      '4' => 'field_unpublishdate',
      '5' => 'field_freeaccess',
    ),
  );

  // Exported group: group_content
  $groups['wally_gallerypackage-group_content'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_gallerypackage',
    'group_name' => 'group_content',
    'label' => 'Content',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '1',
    'fields' => array(
      '0' => 'field_mainobject',
      '1' => 'field_embededobjects',
      '2' => 'field_packagelayout',
    ),
  );

  // Exported group: group_destinations
  $groups['wally_gallerypackage-group_destinations'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_gallerypackage',
    'group_name' => 'group_destinations',
    'label' => 'Destinations',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_destinations',
      '1' => 'field_editions',
      '2' => 'field_channels',
    ),
  );

  // Exported group: group_packageidentification
  $groups['wally_gallerypackage-group_packageidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_gallerypackage',
    'group_name' => 'group_packageidentification',
    'label' => 'Package Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-3',
    'fields' => array(
      '0' => 'field_packageid',
      '1' => 'field_externalreference',
      '2' => 'field_objectdescription',
      '3' => 'field_productid',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_gallerypackage-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_gallerypackage',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & Geolocalisation',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-1',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_entities',
      '3' => 'field_persons',
      '4' => 'field_tags',
      '5' => 'field_free_tags',
    ),
  );

  // Exported group: group_linkslist
  $groups['wally_linkslistobject-group_linkslist'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_linkslistobject',
    'group_name' => 'group_linkslist',
    'label' => 'Links List',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_summary',
      '1' => 'field_links_list',
    ),
  );

  // Exported group: group_linkslistidentification
  $groups['wally_linkslistobject-group_linkslistidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_linkslistobject',
    'group_name' => 'group_linkslistidentification',
    'label' => 'group_LinksList Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-1',
    'fields' => array(
      '0' => 'field_externalreference',
      '1' => 'field_objectid',
      '2' => 'field_objectdescription',
    ),
  );

  // Exported group: group_link
  $groups['wally_linktype-group_link'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_linktype',
    'group_name' => 'group_link',
    'label' => 'Link',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-4',
    'fields' => array(
      '0' => 'field_link_item',
      '1' => 'field_summary',
      '2' => 'field_internal_link',
    ),
  );

  // Exported group: group_linkidentification
  $groups['wally_linktype-group_linkidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_linktype',
    'group_name' => 'group_linkidentification',
    'label' => 'Link Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-3',
    'fields' => array(
      '0' => 'field_externalreference',
      '1' => 'field_objectid',
      '2' => 'field_objectdescription',
    ),
  );

  // Exported group: group_locationindentification
  $groups['wally_locationtype-group_locationindentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_locationtype',
    'group_name' => 'group_locationindentification',
    'label' => 'Location Indentification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '0',
    'fields' => array(
      '0' => 'field_externalreference',
    ),
  );

  // Exported group: group_identity
  $groups['wally_persontype-group_identity'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_persontype',
    'group_name' => 'group_identity',
    'label' => 'Identity',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-1',
    'fields' => array(
      '0' => 'field_personfirstname',
      '1' => 'field_personlastname',
      '2' => 'field_personnickname',
    ),
  );

  // Exported group: group_personnalinfo
  $groups['wally_persontype-group_personnalinfo'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_persontype',
    'group_name' => 'group_personnalinfo',
    'label' => 'PersonnalInfo',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '1',
    'fields' => array(
      '0' => 'field_personbirthdate',
      '1' => 'field_personsex',
      '2' => 'field_personemail',
      '3' => 'field_personwebsite',
      '4' => 'field_phonenumber',
    ),
  );

  // Exported group: group_authoring
  $groups['wally_photoobject-group_authoring'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_photoobject',
    'group_name' => 'group_authoring',
    'label' => 'Authoring',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '1',
    'fields' => array(
      '0' => 'field_authors',
      '1' => 'field_copyright',
    ),
  );

  // Exported group: group_photo
  $groups['wally_photoobject-group_photo'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_photoobject',
    'group_name' => 'group_photo',
    'label' => 'Photo',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_summary',
      '1' => 'field_photofile',
      '2' => 'field_thumbnail',
      '3' => 'field_photo3rdparty',
      '4' => 'field_link',
      '5' => 'field_rating',
      '6' => 'field_croping',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_photoobject-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_photoobject',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & TME',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '0',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_tags',
      '3' => 'field_free_tags',
      '4' => 'field_entities',
      '5' => 'field_persons',
    ),
  );

  // Exported group: group_textidentification
  $groups['wally_photoobject-group_textidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_photoobject',
    'group_name' => 'group_textidentification',
    'label' => 'Photo Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '-1',
    'fields' => array(
      '0' => 'field_externalreference',
      '1' => 'field_objectid',
      '2' => 'field_objectdescription',
    ),
  );

  // Exported group: group_authoring
  $groups['wally_pollobject-group_authoring'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollobject',
    'group_name' => 'group_authoring',
    'label' => 'Authoring',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '1',
    'fields' => array(
      '0' => 'field_authors',
      '1' => 'field_copyright',
    ),
  );

  // Exported group: group_poll
  $groups['wally_pollobject-group_poll'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollobject',
    'group_name' => 'group_poll',
    'label' => 'Poll',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-1',
    'fields' => array(),
  );

  // Exported group: group_pollidentification
  $groups['wally_pollobject-group_pollidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollobject',
    'group_name' => 'group_pollidentification',
    'label' => 'Poll Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '2',
    'fields' => array(
      '0' => 'field_externalreference',
      '1' => 'field_objectid',
      '2' => 'field_objectdescription',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_pollobject-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollobject',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & TME',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '0',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_tags',
      '3' => 'field_free_tags',
      '4' => 'field_entities',
      '5' => 'field_persons',
    ),
  );

  // Exported group: group_authoring
  $groups['wally_pollpackage-group_authoring'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollpackage',
    'group_name' => 'group_authoring',
    'label' => 'Authoring',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '0',
    'fields' => array(
      '0' => 'field_authors',
      '1' => 'field_packageauthors',
      '2' => 'field_publicationdate',
      '3' => 'field_embargodatetime',
      '4' => 'field_unpublishdate',
      '5' => 'field_freeaccess',
    ),
  );

  // Exported group: group_content
  $groups['wally_pollpackage-group_content'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollpackage',
    'group_name' => 'group_content',
    'label' => 'Content',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '1',
    'fields' => array(
      '0' => 'field_mainpoll',
      '1' => 'field_embededobjects',
      '2' => 'field_packagelayout',
    ),
  );

  // Exported group: group_destinations
  $groups['wally_pollpackage-group_destinations'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollpackage',
    'group_name' => 'group_destinations',
    'label' => 'Destinations',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_destinations',
      '1' => 'field_editions',
      '2' => 'field_channels',
    ),
  );

  // Exported group: group_packageidentification
  $groups['wally_pollpackage-group_packageidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollpackage',
    'group_name' => 'group_packageidentification',
    'label' => 'Package Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-3',
    'fields' => array(
      '0' => 'field_packageid',
      '1' => 'field_externalreference',
      '2' => 'field_objectdescription',
      '3' => 'field_productid',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_pollpackage-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_pollpackage',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & Geolocalisation',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-1',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_entities',
      '3' => 'field_persons',
      '4' => 'field_tags',
      '5' => 'field_free_tags',
    ),
  );

  // Exported group: group_authoring
  $groups['wally_textobject-group_authoring'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textobject',
    'group_name' => 'group_authoring',
    'label' => 'Authoring',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '41',
    'fields' => array(
      '0' => 'field_authors',
    ),
  );

  // Exported group: group_extracts
  $groups['wally_textobject-group_extracts'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textobject',
    'group_name' => 'group_extracts',
    'label' => 'Extracts',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '38',
    'fields' => array(
      '0' => 'field_extractmedium',
      '1' => 'field_extractshort',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_textobject-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textobject',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & TME',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '40',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_entities',
      '3' => 'field_persons',
      '4' => 'field_tags',
      '5' => 'field_free_tags',
    ),
  );

  // Exported group: group_text
  $groups['wally_textobject-group_text'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textobject',
    'group_name' => 'group_text',
    'label' => 'Text',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '37',
    'fields' => array(
      '0' => 'field_textbarette',
      '1' => 'field_textforetitle',
      '2' => 'field_textsubtitle',
      '3' => 'field_copyright',
    ),
  );

  // Exported group: group_textidentification
  $groups['wally_textobject-group_textidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textobject',
    'group_name' => 'group_textidentification',
    'label' => 'Text Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset_collapsed',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '39',
    'fields' => array(
      '0' => 'field_externalreference',
      '1' => 'field_objectid',
      '2' => 'field_objecttitle',
      '3' => 'field_objectdescription',
    ),
  );

  // Exported group: group_authoring
  $groups['wally_videoobject-group_authoring'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_videoobject',
    'group_name' => 'group_authoring',
    'label' => 'Authoring',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-1',
    'fields' => array(
      '0' => 'field_authors',
      '1' => 'field_copyright',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_videoobject-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_videoobject',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy & TME',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-3',
    'fields' => array(
      '0' => 'field_markers',
      '1' => 'field_locations',
      '2' => 'field_tags',
      '3' => 'field_free_tags',
      '4' => 'field_entities',
      '5' => 'field_persons',
    ),
  );

  // Exported group: group_video
  $groups['wally_videoobject-group_video'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_videoobject',
    'group_name' => 'group_video',
    'label' => 'Video',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-4',
    'fields' => array(
      '0' => 'field_summary',
      '1' => 'field_videofile',
      '2' => 'field_thumbnail',
      '3' => 'field_video3rdparty',
      '4' => 'field_rating',
      '5' => 'field_link',
    ),
  );

  // Exported group: group_videoidentification
  $groups['wally_videoobject-group_videoidentification'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_videoobject',
    'group_name' => 'group_videoidentification',
    'label' => 'Video Identification',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'description' => '',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_externalreference',
      '1' => 'field_objectid',
      '2' => 'field_objectdescription',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t(' Audio Identification');
  t('Audio');
  t('Authoring');
  t('Content');
  t('Destinations');
  t('Digital Object');
  t('Digital Object Identification ');
  t('Extracts');
  t('Identity');
  t('Link');
  t('Link Identification');
  t('Links List');
  t('Location Indentification');
  t('Package Identification');
  t('PersonnalInfo');
  t('Photo');
  t('Photo Identification');
  t('Poll');
  t('Poll Identification');
  t('Taxonomy & Geolocalisation');
  t('Taxonomy & TME');
  t('Text');
  t('Text Identification');
  t('Video');
  t('Video Identification');
  t('group_LinksList Identification');

  return $groups;
}
