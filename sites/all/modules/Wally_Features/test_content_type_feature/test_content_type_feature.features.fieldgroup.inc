<?php

/**
 * Implementation of hook_fieldgroup_default_groups().
 */
function test_content_type_feature_fieldgroup_default_groups() {
  $groups = array();

  // Exported group: group_datefields
  $groups['wally_textcontenttype-group_datefields'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textcontenttype',
    'group_name' => 'group_datefields',
    'label' => 'DateFileds',
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
      '0' => 'field_datetimefield',
      '1' => 'field_embargodatetimemulti',
      '2' => 'field_datetimewtofield',
      '3' => 'field_datetimewtofieldmulti',
    ),
  );

  // Exported group: group_filefields
  $groups['wally_textcontenttype-group_filefields'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textcontenttype',
    'group_name' => 'group_filefields',
    'label' => 'FilesFields',
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
      '0' => 'field_filefield',
      '1' => 'field_filefieldmulti',
      '2' => 'field_imagefield',
      '3' => 'field_imagefieldmulti',
    ),
  );

  // Exported group: group_locationfield
  $groups['wally_textcontenttype-group_locationfield'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textcontenttype',
    'group_name' => 'group_locationfield',
    'label' => 'LocationsFileds',
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
      '0' => 'field_locationfield',
      '1' => 'field_locationfieldmulti',
    ),
  );

  // Exported group: group_nodereference
  $groups['wally_textcontenttype-group_nodereference'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textcontenttype',
    'group_name' => 'group_nodereference',
    'label' => 'NodeReference',
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
      '0' => 'field_nodereffield',
      '1' => 'field_nodereffieldmulti',
    ),
  );

  // Exported group: group_pollfields
  $groups['wally_textcontenttype-group_pollfields'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textcontenttype',
    'group_name' => 'group_pollfields',
    'label' => 'PollFields',
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
      '0' => 'field_pollfield',
      '1' => 'field_pollfieldmulti',
    ),
  );

  // Exported group: group_simpletext
  $groups['wally_textcontenttype-group_simpletext'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textcontenttype',
    'group_name' => 'group_simpletext',
    'label' => 'SimpleText',
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
      '0' => 'field_simpletextfield',
      '1' => 'field_simpletextfieldmulti',
      '2' => 'field_textareafield',
      '3' => 'field_textareafieldmulti',
    ),
  );

  // Exported group: group_taxonomy
  $groups['wally_textcontenttype-group_taxonomy'] = array(
    'group_type' => 'standard',
    'type_name' => 'wally_textcontenttype',
    'group_name' => 'group_taxonomy',
    'label' => 'Taxonomy',
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
    'weight' => '3',
    'fields' => array(
      '0' => 'field_contenttaxonomyfield',
      '1' => 'field_contenttaxonomyfieldmulti',
      '2' => 'field_contenttaxonomyfieldtree',
      '3' => 'field_contenttaxonomyfieldtreem',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('DateFileds');
  t('FilesFields');
  t('LocationsFileds');
  t('NodeReference');
  t('PollFields');
  t('SimpleText');
  t('Taxonomy');

  return $groups;
}
