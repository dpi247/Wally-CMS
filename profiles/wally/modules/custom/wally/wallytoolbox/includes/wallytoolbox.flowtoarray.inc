<?php 

function wallytoolbox_flow_to_array_init_array($feed_infos = array()) {
  $array_packages = array();
  $array_packages['Packages']['Package'] = array();
  $array_packages['Packages']['__log'] = array();
  $array_packages['Packages']['__log']['success'] = TRUE;
  $array_packages['Packages']['__log']['msg'] = '';
  $array_packages['Packages']['__settings'] = $feed_infos;
  return $array_packages;
}

/**
 * Initialise an array following the schema of a Wallymport articlepackage.
 * Only required fields are initialised.
 * 
 * @return
 *   The initialised array.
 */
function wallytoolbox_flow_to_array_new_package() {
  $array_item = array();
  $array_item['PackageID'] = '';
  $array_item['CreationDate'] = date('Y-m-d').'T'.date('H:i:sP');
  $array_item['EmbargoDate'] = date('Y-m-d').'T'.date('H:i:sP');
  $array_item['Language'] = '';
  $array_item['PackageTitle']['value'] = '';
  $array_item['ExternalReference']['value'] = '';
  $array_item['ProductID']['value'] = '';
  $array_item['Destinations'] = array();
  $array_item['Destinations']['Destination'] = array();
  $array_item['Editions'] = array();
  $array_item['Editions']['Edition']['EditionID'] = 'all';
  $array_item['Editions']['Edition']['value'] = 'true';
  $array_item['Channels'] = array();
  $array_item['Channels']['Channel']['Name'] = '';
  $array_item['Summary']['value'] = '';
  $array_item['ExternalURI']['value'] = 'http://';
  $array_item['__log'] = array();
  $array_item['__log']['success'] = TRUE;
  $array_item['__log']['msg'] = '';
  return $array_item;
}

/**
 * Initialise an array following the schema of a Wallymport textobject.
 * Only required fields are initialised (+ the taxonomy field).
 * 
 * @return
 *   The initialised array.
 */
function wallytoolbox_flow_to_array_new_textobject() {
  $textobject = array();
  $textobject['type'] = 'TextObjectType';
  $textobject['ObjectID'] = '';
  $textobject['CreationDate'] = date('Y-m-d').'T'.date('H:i:sP');
  $textobject['Title']['value'] = '';
  $textobject['ExternalReference']['value'] =  '';
  /*$textobject['Taxonomy'] = array();
  $textobject['Taxonomy']['Vocabulary'] = array();
  $textobject['Taxonomy']['Vocabulary']['VocabularyName'] = 'Default';
  $textobject['Taxonomy']['Vocabulary']['VocabularyType'] = 'free';
  $textobject['Taxonomy']['Vocabulary']['Term'] = array();
  $textobject['TextBody']['value'] = '';*/
  return $textobject;
}

/**
 * Initialise an array following the schema of a Wallymport photoobject.
 * 
 * @return
 *   The initialised array.
 */
function wallytoolbox_flow_to_array_new_photoobject() {
  $photoobject = array();
  $photoobject['type'] = 'PhotoObjectType';
  $photoobject['ObjectID'] = '';
  $photoobject['CreationDate'] = date('Y-m-d').'T'.date('H:i:sP');
  $photoobject['Title']['value'] = '';
  $photoobject['ExternalReference']['value'] = '';
  $photoobject['FileURI']['value'] = 'http://';
  $photoobject['FileSize']['value'] = '';
  return $photoobject;
}

/**
 * Initialise an array following the schema of a Wallymport videoobject.
 * 
 * @return
 *   The initialised array.
 */
function wallytoolbox_flow_to_array_new_videoobject() {
  $videoobject = array();
  $videoobject['type'] = 'VideoObjectType';
  $videoobject['ObjectID'] = '';
  $videoobject['CreationDate'] = date('Y-m-d').'T'.date('H:i:sP');
  $videoobject['Title']['value'] = '';
  $videoobject['ExternalReference']['value'] = '';
  $videoobject['FileURI']['value'] = 'http://';
  $videoobject['FileSize']['value'] = '';
  return $videoobject;
}

function wallytoolbox_flow_to_array_struct_term($struct_array) {
  $new_struct = array('Vocabulary' => array());
  $vocabularies = taxonomy_get_vocabularies();
  
  if (isset($struct_array['Markers']['Location']) && !empty($struct_array['Markers']['Location'])) {
    $loc_voc = $vocabularies[variable_get('wallymport_location', 0)];
    $temp_tax = array(
      'VocabularyName' => 'Markers',
      'VocabularyDescription' => $loc_voc->description,
      'VocabularyType' => 'categorized',
      'Term' => array(),
    );
    foreach ($struct_array['Markers']['Location'] as $marker) {
      $temp_term = array();
      $temp_term['type'] = 'LocationType';
      if (is_numeric($marker)) {
        if ($term = taxonomy_get_term($marker)) {
          $temp_term['Name'] = $term->name;
        }
      } else {
        $temp_term['Name'] = $marker['LocationName']['value'];
        $temp_term['Address'] = $marker['Address'];
        $temp_term['LocationGeo'] = $marker['LocationGeo'];
      }
      $temp_tax['Term'][] = $temp_term;
    }
    $new_struct['Vocabulary'][] = $temp_tax;
  }
  
  if (isset($struct_array['Locations']['Location']) && !empty($struct_array['Locations']['Location'])) {
    $temp_tax = array(
      'VocabularyName' => 'Locations',
      'VocabularyDescription' => $loc_voc->description,
      'VocabularyType' => 'categorized',
      'Term' => array(),
    );
    foreach ($struct_array['Locations']['Location'] as $location) {
      $temp_term = array();
      $temp_term['type'] = 'LocationType';
      if (is_numeric($location)) {
        if ($term = taxonomy_get_term($location)) {
          $temp_term['Name'] = $term->name;
        }
      } else {
        $temp_term['Name'] = $location['LocationName']['value'];
        $temp_term['Address'] = $location['Address'];
        $temp_term['LocationGeo'] = $location['LocationGeo'];
      }
      $temp_tax['Term'][] = $temp_term;
    }
    $new_struct['Vocabulary'][] = $temp_tax;
  }
  
  if (isset($struct_array['Entities']['Entity']) && !empty($struct_array['Entities']['Entity'])) {
    $ent_voc = $vocabularies[variable_get('wallymport_entity', 0)];
    $temp_tax = array(
      'VocabularyName' => 'Entities',
      'VocabularyDescription' => $ent_voc->description,
      'VocabularyType' => 'categorized',
      'Term' => array(),
    );
    foreach ($struct_array['Entities']['Entity'] as $entity) {
      $temp_term = array();
      $temp_term['type'] = 'EntityType';
      if (is_numeric($entity)) {
        if ($term = taxonomy_get_term($entity)) {
          $temp_term['Name'] = $term->name;
        }
      } else {
        $temp_term['Name'] = $entity['EntityTitle']['value'];
        $temp_term['EntityDescription'] = $entity['EntityDescription'];
        $temp_term['EntityServices'] = $entity['EntityServices'];
      }
      $temp_tax['Term'][] = $temp_term;
    }
    $new_struct['Vocabulary'][] = $temp_tax;
  }
  
  if (isset($struct_array['Persons']['Person']) && !empty($struct_array['Persons']['Person'])) {
    $per_voc = $vocabularies[variable_get('wallymport_person', 0)];
    $temp_tax = array(
      'VocabularyName' => 'Persons',
      'VocabularyDescription' => $per_voc->description,
      'VocabularyType' => 'categorized',
      'Term' => array(),
    );
    foreach ($struct_array['Persons']['Person'] as $person) {
      $temp_term = array();
      $temp_term['type'] = 'PersonType';
      if (is_numeric($person)) {
        if ($term = taxonomy_get_term($person)) {
          $temp_term['Name'] = $term->name;
        }
      } else {
        $temp_term['Name'] = $person['PersonName']['value'];
        $temp_term['PersonLastName'] = $person['PersonLastName'];
        $temp_term['PersonFirstName'] = $person['PersonFirstName'];
        $temp_term['PersonNickName'] = $person['PersonNickName'];
        $temp_term['PersonBirthDate'] = $person['PersonBirthDate'];
        $temp_term['PersonSex'] = $person['PersonSex'];
        $temp_term['PersonEmail'] = $person['PersonEmail'];
        $temp_term['PersonWebSite'] = $person['PersonWebSite'];
        $temp_term['PersonPhoneNumber'] = $person['PersonPhoneNumber'];
      }
      $temp_tax['Term'][] = $temp_term;
    }
    $new_struct['Vocabulary'][] = $temp_tax;
  }
  
  if (isset($struct_array['Authors']['Person']) && !empty($struct_array['Authors']['Person'])) {
    $temp_tax = array(
      'VocabularyName' => 'Authors',
      'VocabularyDescription' => $per_voc->description,
      'VocabularyType' => 'categorized',
      'Term' => array(),
    );
    foreach ($struct_array['Authors']['Person'] as $author) {
      $temp_term = array();
      $temp_term['type'] = 'PersonType';
      if (is_numeric($author)) {
        if ($term = taxonomy_get_term($author)) {
          $temp_term['Name'] = $term->name;
        }
      } else {
        $temp_term['Name'] = $author['PersonName']['value'];
        $temp_term['PersonLastName'] = $author['PersonLastName'];
        $temp_term['PersonFirstName'] = $author['PersonFirstName'];
        $temp_term['PersonNickName'] = $author['PersonNickName'];
        $temp_term['PersonBirthDate'] = $author['PersonBirthDate'];
        $temp_term['PersonSex'] = $author['PersonSex'];
        $temp_term['PersonEmail'] = $author['PersonEmail'];
        $temp_term['PersonWebSite'] = $author['PersonWebSite'];
        $temp_term['PersonPhoneNumber'] = $author['PersonPhoneNumber'];
      }
      $temp_tax['Term'][] = $temp_term;
    }
    $new_struct['Vocabulary'][] = $temp_tax;
  }
  
  // Free tags and categorized keywords
  if (isset($struct_array['Taxonomy']['Vocabulary']) && !empty($struct_array['Taxonomy']['Vocabulary'])) {
    $free_voc = variable_get('wallymport_freetagtaxonomy', 0);
    $conc_voc = variable_get('wallymport_concept', 0);
    $iptc_voc = variable_get('wallymport_iptc', 0);
    
    $free_temp_tax = array();
    $free_temp_tax['VocabularyName'] = 'Tags';
    $free_temp_tax['VocabularyDescription'] = $vocabularies[$free_voc]->description;
    $free_temp_tax['VocabularyType'] = 'free';
    $free_temp_tax['Term'] = array();
    
    $conc_temp_tax = array();
    $conc_temp_tax['VocabularyName'] = 'Concepts';
    $conc_temp_tax['VocabularyDescription'] = $vocabularies[$conc_voc]->description;
    $conc_temp_tax['VocabularyType'] = 'free';
    $conc_temp_tax['Term'] = array();
    
    $iptc_temp_tax = array();
    $iptc_temp_tax['VocabularyName'] = 'IPTC';
    $iptc_temp_tax['VocabularyDescription'] = $vocabularies[$iptc_voc]->description;
    $iptc_temp_tax['VocabularyType'] = 'categorized';
    $iptc_temp_tax['Term'] = array();
    
    foreach ($struct_array['Taxonomy']['Vocabulary'] as $vocabulary) {
      switch ($vocabulary['VocabularyType']) {
        case $free_voc :
          $free_temp_tax['Term'][$vocabulary['Term']['value']] = array(
            'Name' => $vocabulary['VocabularyName'],
          );
          break;
        case $conc_voc :
          $conc_temp_tax['Term'][$vocabulary['Term']['value']] = array(
            'Name' => $vocabulary['VocabularyName'],
          );
          break;
        case $iptc_voc :
          $iptc_temp_tax['Term'][$vocabulary['Term']['value']] = array(
            'Name' => wallytoolbox_taxonomy_get_path_by_tid_or_term($vocabulary['Term']['value']),
          );
          break;
      }
    }
    
    if (!empty($free_temp_tax['Term'])) {
      $new_struct['Vocabulary'][] = $free_temp_tax;
    }

    if (!empty($conc_temp_tax['Term'])) {
      $new_struct['Vocabulary'][] = $conc_temp_tax;
    }

    if (!empty($iptc_temp_tax['Term'])) {
      $new_struct['Vocabulary'][] = $iptc_temp_tax;
    }
  }
  
  if (empty($new_struct['Vocabulary'])) {
    unset($new_struct['Vocabulary']);
  }
  
  return $new_struct;
}

function wallytoolbox_node_to_array($node) {
  module_load_include('inc', 'wallytoolbox', 'plugins/flow/destinations');
  $transformed_node['Packages']['Package'][0] = wallytoolbox_destinations_flow_transform_node($node);
  
  switch ($node->type){
    case 'wally_articlepackage' : $transformed_node['Packages']['Package'][0]['type'] = 'ArticlePackageType';break;
    case 'wally_gallerypackage' : $transformed_node['Packages']['Package'][0]['type'] = 'GalleryPackageType';break;
    case 'wally_pollpackage'    : $transformed_node['Packages']['Package'][0]['type'] = 'PollPackageType';break;
  }
  switch($node->comment){
    case 0 : $transformed_node['Packages']['Package'][0]['CommentsAllowed']['value'] = 'Disabled';break;
    case 1 : $transformed_node['Packages']['Package'][0]['CommentsAllowed']['value'] = 'Read Only';break;
    default :
      $transformed_node['Packages']['Package'][0]['CommentsAllowed']['value'] = 'Read-Write';
  }
  $packagelayout = taxonomy_get_term($node->field_packagelayout[0]['value']);
  $transformed_node['Packages']['Package'][0]['PackageLayout']['value'] = $packagelayout->name;
  
  if (isset($transformed_node['Packages']['Package'][0]['EmbeddedContent']['EmbeddedObjects']['Object'])){
    foreach ($transformed_node['Packages']['Package'][0]['EmbeddedContent']['EmbeddedObjects']['Object'] as $object_key => $object_value){
      switch ($object_value['type']){
        case 'wally_audioobject' : $transformed_node['Packages']['Package'][0]['EmbeddedContent']['EmbeddedObjects']['Object'][$object_key]['type'] = 'AudioObjectType';break;
        case 'wally_photoobject' : $transformed_node['Packages']['Package'][0]['EmbeddedContent']['EmbeddedObjects']['Object'][$object_key]['type'] = 'PhotoObjectType';break;
        case 'wally_pollobject'  : $transformed_node['Packages']['Package'][0]['EmbeddedContent']['EmbeddedObjects']['Object'][$object_key]['type'] = 'PollObjectType';break;
        case 'wally_textobject'  : $transformed_node['Packages']['Package'][0]['EmbeddedContent']['EmbeddedObjects']['Object'][$object_key]['type'] = 'TextObjectType';break;
        case 'wally_videoobject' : $transformed_node['Packages']['Package'][0]['EmbeddedContent']['EmbeddedObjects']['Object'][$object_key]['type'] = 'VideoObjectType';break;
      }
    }
  }
  
  return $transformed_node;
}

/**
 * Search an item for a desired element.
 * 
 * @param $dom_item
 *   The item in which the search is performed.
 * @param $name
 *   The name of the searched element.
 * @param $namespace
 *   Value of the namespace if used.
 * @param $second_choice
 *   If the element given by $name isn\'t found, you have a second chance.
 * @param $feed_name
 *   The name of the feed as recorded in the DB. Needed as default
 *   external reference value.
 * 
 * @return
 *   The element value or ''.
 */
function _wallytoolbox_checkifelementexists($dom_item, $name, $namespace = 'none', $second_choice = '', $feed_name = '', $prefix = '') {
  $return = '';
  $prefix = ($prefix != '') ? $prefix : $feed_name;
  if ($namespace != 'none' && is_object($dom_item)) {
    $return = $dom_item->getElementsByTagNameNS($namespace, $name)->item(0) ? $dom_item->getElementsByTagNameNS($namespace, $name)->item(0)->firstChild->wholeText : '';
    if ($second_choice != '') {
      if ($return == '') {
        $return = $dom_item->getElementsByTagNameNS($namespace, $second_choice)->item(0) ? $dom_item->getElementsByTagNameNS($namespace, $second_choice)->item(0)->firstChild->wholeText : '';
      }
      $return = $prefix.':::'.$return;
    }
  } elseif (is_object($dom_item)) {
    $return = $dom_item->getElementsByTagName($name)->item(0) ? $dom_item->getElementsByTagName($name)->item(0)->firstChild->wholeText : '';
    if ($second_choice != '') {
      if ($return == '') {
        $return = $dom_item->getElementsByTagName($second_choice)->item(0) ? $dom_item->getElementsByTagName($second_choice)->item(0)->firstChild->wholeText : '';
      }
      $return = $prefix.':::'.$return;
    }
  }
  return $return;
}
