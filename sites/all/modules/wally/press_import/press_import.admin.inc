<?php
// $Id: press_import.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * Settings form.
 */
function press_import_admin($form_state) {

  $vocabularies=taxonomy_get_vocabularies();
  
  $path = drupal_get_path('module', 'press_import');
  $form['press_import_product'] = array(
    '#type' => 'textfield',
    '#title' => t('Product ID'),
    '#description' => t('Describes this Drupal site. XML data received must match this ID in order to be published.'),
    '#default_value' => variable_get('press_import_product', ''),
    '#size' => 10,
    '#maxlength' => 32,
    '#required' => TRUE,
  );
  $form['press_import_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Product Version'),
    '#description' => t('Corresponds to the Import Data structure definition version. XML data received must match this version in order to be published.'),
    '#default_value' => variable_get('press_import_version', '1.0'),
    '#size' => 10,
    '#maxlength' => 32,
    '#required' => TRUE,
  );
  $node_types=node_get_types();	// D7: node_type_get_types();
  foreach($node_types as $key => $node_type)
	$node_type_list[$key]=$node_type->name;
  
  
  
  $form['press_import_locationitem'] = array(
    '#type' => 'select',
    '#title' => t('Location Item node type'),
    '#description' => t('The content type of a Location Item.'),
    '#default_value' => variable_get('press_import_locationitem', 'wally_locationtype'),
    '#options' => $node_type_list,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $vocab_list=array();
  foreach($vocabularies as $key => $vocabulary) {
    if (array_key_exists(variable_get('press_import_locationitem', 'wally_locationtype'),$vocabulary->nodes))
	  $vocab_list[$key]=$vocabulary->name;
  }
  $form['press_import_taxonomy_location'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Locations"'),
    '#description' => t('Choose the vocabulary corresponding to Locations.'),
    '#default_value' => variable_get('press_import_taxonomy_location', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );
  
  
   $node_types=node_get_types();	// D7: node_type_get_types();
  foreach($node_types as $key => $node_type)
	$node_type_list[$key]=$node_type->name;
  
  
  
  $form['press_import_personitem'] = array(
    '#type' => 'select',
    '#title' => t('Person Item node type'),
    '#description' => t('The content type of a Person Item.'),
    '#default_value' => variable_get('press_import_personitem', 'wally_persontype'),
    '#options' => $node_type_list,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $vocab_list=array();
  foreach($vocabularies as $key => $vocabulary) {
    if (array_key_exists(variable_get('press_import_personitem', 'wally_persontype'),$vocabulary->nodes))
	  $vocab_list[$key]=$vocabulary->name;
  }
  $form['press_import_taxonomy_person'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Person"'),
    '#description' => t('Choose the vocabulary corresponding to Persons.'),
    '#default_value' => variable_get('press_import_taxonomy_person', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );
  
  
  
  
  
  
     $node_types=node_get_types();	// D7: node_type_get_types();
  foreach($node_types as $key => $node_type)
	$node_type_list[$key]=$node_type->name;
  
  
  
  $form['press_import_entityitem'] = array(
    '#type' => 'select',
    '#title' => t('Entity Item node type'),
    '#description' => t('The content type of a Entity Item.'),
    '#default_value' => variable_get('press_import_entityitem', 'wally_entitytype'),
    '#options' => $node_type_list,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $vocab_list=array();
  foreach($vocabularies as $key => $vocabulary) {
    if (array_key_exists(variable_get('press_import_entityitem', 'wally_entitytype'),$vocabulary->nodes))
	  $vocab_list[$key]=$vocabulary->name;
  }
  $form['press_import_taxonomy_entity'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Entity"'),
    '#description' => t('Choose the vocabulary corresponding to Entities.'),
    '#default_value' => variable_get('press_import_taxonomy_entity', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );
  
  
  
  
     $node_types=node_get_types();	// D7: node_type_get_types();
  foreach($node_types as $key => $node_type)
	$node_type_list[$key]=$node_type->name;
  
  
  
  $form['press_import_entityitem'] = array(
    '#type' => 'select',
    '#title' => t('Entity Item node type'),
    '#description' => t('The content type of a Entity Item.'),
    '#default_value' => variable_get('press_import_entityitem', 'wally_entitytype'),
    '#options' => $node_type_list,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $vocab_list=array();
  foreach($vocabularies as $key => $vocabulary) {
    if (array_key_exists(variable_get('press_import_entityitem', 'wally_entitytype'),$vocabulary->nodes))
	  $vocab_list[$key]=$vocabulary->name;
  }
  $form['press_import_taxonomy_entity'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Entity"'),
    '#description' => t('Choose the vocabulary corresponding to Entities.'),
    '#default_value' => variable_get('press_import_taxonomy_entity', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );
  
  
  
  
   
  
     $node_types=node_get_types();	// D7: node_type_get_types();
  foreach($node_types as $key => $node_type)
	$node_type_list[$key]=$node_type->name;
  
  
  
  $form['press_import_authoritem'] = array(
    '#type' => 'select',
    '#title' => t('Author Item node type'),
    '#description' => t('The content type of a Author Item.'),
    '#default_value' => variable_get('press_import_personitem', 'wally_persontype'),
    '#options' => $node_type_list,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $vocab_list=array();
  foreach($vocabularies as $key => $vocabulary) {
    if (array_key_exists(variable_get('press_import_authoritem', 'wally_persontype'),$vocabulary->nodes))
	  $vocab_list[$key]=$vocabulary->name;
  }
  $form['press_import_taxonomy_author'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Author"'),
    '#description' => t('Choose the vocabulary corresponding to Authors.'),
    '#default_value' => variable_get('press_import_taxonomy_author', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );
  
  
  
  
  
  
  
  
  
  
  $vocab_list=array(0 => t('-ROOT-'));
  foreach($vocabularies as $key => $vocabulary) {
	  $vocab_list[$key]=$vocabulary->name;
  }
  $form['press_import_taxonomy_tags'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Tags"'),
    '#description' => t('Choose the vocabulary corresponding to Tags. If none is chosen, new vocabularies will be creates as new taxonomies. Otherwise new vocabularies will be set as sub-items of Tags.'),
    '#default_value' => variable_get('press_import_taxonomy_tags', 0),
    '#options' => $vocab_list,
    '#required' => FALSE,
  );
  return system_settings_form($form);
}

/**
 * Validate function for the settings form.
 */
function press_import_admin_validate($form, &$form_state) {
  
}

