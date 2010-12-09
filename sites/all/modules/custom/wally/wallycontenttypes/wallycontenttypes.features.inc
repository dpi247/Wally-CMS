<?php

/**
 * Implementation of hook_content_taxonomy_mapping().
 */
function wallycontenttypes_content_taxonomy_mapping() {
  return array(
    'field_authors-authors' => array(
      'field_name' => 'field_authors',
      'vocabulary' => 'authors',
    ),
    'field_documenttype-documenttype' => array(
      'field_name' => 'field_documenttype',
      'vocabulary' => 'documenttype',
    ),
    'field_entities-entities' => array(
      'field_name' => 'field_entities',
      'vocabulary' => 'entities',
    ),
    'field_entity-entities' => array(
      'field_name' => 'field_entity',
      'vocabulary' => 'entities',
    ),
    'field_free_tags-keywords' => array(
      'field_name' => 'field_free_tags',
      'vocabulary' => 'keywords',
    ),
    'field_locations-locations' => array(
      'field_name' => 'field_locations',
      'vocabulary' => 'locations',
    ),
    'field_locationtaxonomy-locations' => array(
      'field_name' => 'field_locationtaxonomy',
      'vocabulary' => 'locations',
    ),
    'field_packageauthors-authors' => array(
      'field_name' => 'field_packageauthors',
      'vocabulary' => 'authors',
    ),
    'field_persons-persons' => array(
      'field_name' => 'field_persons',
      'vocabulary' => 'persons',
    ),
    'field_persontaxonomy-persons' => array(
      'field_name' => 'field_persontaxonomy',
      'vocabulary' => 'persons',
    ),
    'field_rating-rating' => array(
      'field_name' => 'field_rating',
      'vocabulary' => 'rating',
    ),
    'field_tags-tags' => array(
      'field_name' => 'field_tags',
      'vocabulary' => 'tags',
    ),
  );
}

/**
 * Implementation of hook_node_info().
 */
function wallycontenttypes_node_info() {
  $items = array(
    'wally_articlepackage' => array(
      'name' => t('wally_ArticlePackage'),
      'module' => 'features',
      'description' => t('This is an Article Package. Will always contain a Min Story text, with some embeded objects and links. '),
      'has_title' => '1',
      'title_label' => t('Package Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_audioobject' => array(
      'name' => t('wally_AudioObject'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_digitalobject' => array(
      'name' => t('wally_DigitalObject'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_entitytype' => array(
      'name' => t('wally_EntityType'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Entity Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_gallerypackage' => array(
      'name' => t('wally_GalleryPackage'),
      'module' => 'features',
      'description' => t('Multimedia Gallery Package'),
      'has_title' => '1',
      'title_label' => t('Package Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_linkslistobject' => array(
      'name' => t('wally_linksListObject'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_linktype' => array(
      'name' => t('Wally_LinkType'),
      'module' => 'features',
      'description' => t('Wally Link type - Link object (internal / external ref)'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_locationtype' => array(
      'name' => t('wally_LocationType'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_persontype' => array(
      'name' => t('wally_PersonType'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Person Name'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_photoobject' => array(
      'name' => t('wally_PhotoObject'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Photo Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_pollobject' => array(
      'name' => t('wally_PollObject'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_pollpackage' => array(
      'name' => t('wally_PollPackage'),
      'module' => 'features',
      'description' => t('Poll package'),
      'has_title' => '1',
      'title_label' => t('Package Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_textobject' => array(
      'name' => t('wally_TextObject'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_videoobject' => array(
      'name' => t('wally_VideoObject'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
