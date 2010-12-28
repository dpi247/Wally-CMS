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
    'field_packagelayout-documenttype' => array(
      'field_name' => 'field_packagelayout',
      'vocabulary' => 'documenttype',
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
      'name' => t('P Article'),
      'module' => 'features',
      'description' => t('Article Package - Will always contain a Main object (text object), with some unrequired embedded objects and links lists.'),
      'has_title' => '1',
      'title_label' => t('Package Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_audioobject' => array(
      'name' => t('O Audio'),
      'module' => 'features',
      'description' => t('Audio - Define any supported audio file. '),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_digitalobject' => array(
      'name' => t('O Digital Document'),
      'module' => 'features',
      'description' => t('Digital Document - Define any supported digital document file.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_entitytype' => array(
      'name' => t('T Entity'),
      'module' => 'features',
      'description' => t('Entity - Organisations or Entities. Defines something that has a distinct, separate existence, although it need not be a material existence. '),
      'has_title' => '1',
      'title_label' => t('Entity Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_gallerypackage' => array(
      'name' => t('P Gallery'),
      'module' => 'features',
      'description' => t('Multimedia Gallery Package - Will always contain a Main object (photo, video, audio, ...), with some unrequired embedded objects and links lists.'),
      'has_title' => '1',
      'title_label' => t('Package Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_linkslistobject' => array(
      'name' => t('O Links List'),
      'module' => 'features',
      'description' => t('Links List - Define a list of links (see link type). Links could be internal, external ID or any URI.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_linktype' => array(
      'name' => t('T Link'),
      'module' => 'features',
      'description' => t('Link - Link to object or URI  (internal / external ref)'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_locationtype' => array(
      'name' => t('T Location'),
      'module' => 'features',
      'description' => t('Location - Refer to a geographical location.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_persontype' => array(
      'name' => t('T Person'),
      'module' => 'features',
      'description' => t('Person - defined as any individual self-conscious or rational being.'),
      'has_title' => '1',
      'title_label' => t('Person Name'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_photoobject' => array(
      'name' => t('O Photo'),
      'module' => 'features',
      'description' => t('Photo - Define a Photo file (with Main picture, croping information, etc).'),
      'has_title' => '1',
      'title_label' => t('Photo Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_pollobject' => array(
      'name' => t('O Poll'),
      'module' => 'features',
      'description' => t('Poll - Define a Poll (a poll could have multiple questions and multiple responses). '),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_pollpackage' => array(
      'name' => t('P Poll'),
      'module' => 'features',
      'description' => t('Poll Package - Will always contain a Main poll, with some unrequired embedded objects and links lists.'),
      'has_title' => '1',
      'title_label' => t('Package Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_textobject' => array(
      'name' => t('O Text'),
      'module' => 'features',
      'description' => t('Text - Define a text object.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'wally_videoobject' => array(
      'name' => t('O Video'),
      'module' => 'features',
      'description' => t('Video - Define any supported video file.'),
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
