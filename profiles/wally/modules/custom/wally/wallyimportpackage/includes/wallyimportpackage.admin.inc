<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * Settings form.
 */
function wallyimportpackage_page_admin_form($form_state) {
  $path = drupal_get_path('module', 'wallymport');
  
  $form['wallymport_productID'] = array(
    '#type' => 'textfield',
    '#title' => t('Product ID'),
    '#description' => t('Product ID.'),
    '#default_value' => variable_get('wallymport_productID', $_SERVER['HTTP_HOST']),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $vocab_list = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach($vocabularies as $key => $vocabulary) {
	  $vocab_list[$key] = $vocabulary->name;
  }

  $form['wallymport_destinationpath'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Destination Path"'),
    '#description' => t('Choose the vocabulary corresponding to the destination path. Import module will use this vocabulary to match the DestinationPath element.'),
    '#default_value' => variable_get('wallymport_destinationpath', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );
  
  $form['wallymport_documenttype'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Document Type"'),
    '#description' => t('Choose the vocabulary corresponding to the document type. Import module will use this vocabulary to match the PackageLayout element.'),
    '#default_value' => variable_get('wallymport_documenttype', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );
  $form['wallymport_author'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Authors"'),
    '#description' => t('Choose the vocabulary corresponding to the available Authors. Import module will use this vocabulary to match the AuthorID element.'),
    '#default_value' => variable_get('wallymport_author', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );  
  $form['wallymport_edition'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Editions"'),
    '#description' => t('Choose the vocabulary corresponding to the available Editions. Import module will use this vocabulary to match the EditionID element.'),
    '#default_value' => variable_get('wallymport_edition', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );  

  $form['wallymport_location'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "location"'),
    '#description' => t('Choose the vocabulary corresponding to the taxonomy "location". Import module will use this vocabulary to match the "location" element.'),
    '#default_value' => variable_get('wallymport_location', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );  

  $form['wallymport_person'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "person"'),
    '#description' => t('Choose the vocabulary corresponding to the taxonomy "person". Import module will use this vocabulary to match the "person" element.'),
    '#default_value' => variable_get('wallymport_person', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );  

  $form['wallymport_entity'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Entities"'),
    '#description' => t('Choose the vocabulary corresponding to the taxonomy "entities". Import module will use this vocabulary to match the "entity" element.'),
    '#default_value' => variable_get('wallymport_entity', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );  

  $form['wallymport_freetagtaxonomy'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Free Tags"'),
    '#description' => t('Choose the vocabulary corresponding to the free tagging taxonomy.'),
    '#default_value' => variable_get('wallymport_freetagtaxonomy', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );  

  $form['wallymport_concept'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Concept"'),
    '#description' => t('Choose the vocabulary corresponding to the concept tagging taxonomy.'),
    '#default_value' => variable_get('wallymport_concept', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );
  $form['wallymport_iptc'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "IPTC"'),
    '#description' => t('Choose the vocabulary corresponding to the IPTC tagging taxonomy.'),
    '#default_value' => variable_get('wallymport_iptc', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  ); 
  $form['wallymport_taxonomy_recusive'] = array(
    '#type' => 'select',
    '#title' => t('Is the Taxonomies recusive?'),
    '#description' => t('If TRUE, node will be associated with all the term in taxonomy PATH /term1/term2/term3. Else only to the last term (term3).'),
    '#default_value' => variable_get('wallymport_taxonomy_recusive', FALSE),
    '#options' => array("true"=>"true", "false"=>"false"),
    '#required' => TRUE,
  );  
  
  return system_settings_form($form);
}
