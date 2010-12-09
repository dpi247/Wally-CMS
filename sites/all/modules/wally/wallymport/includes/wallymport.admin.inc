<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * Settings form.
 */
function wallymport_page_admin_form($form_state) {
  $path = drupal_get_path('module', 'wallymport');
  $form['wallymport_source'] = array(
    '#type' => 'textfield',
    '#title' => t('Import source directory path'),
    '#description' => t('This must be an existing and writable path where input zipped data is received.'),
    '#default_value' => variable_get('wallymport_source', 'sites/default/files/import'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  $form['wallymport_definition'] = array(
    '#type' => 'textfield',
    '#title' => t('Import Data Structure Definition file'),
    '#description' => t('XSD (schema) file name, with the path.'),
    '#default_value' => variable_get('wallymport_definition', $path.'/definitions/packages.xsd'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
   $form['wallymport_temp'] = array(
    '#type' => 'textfield',
    '#title' => t('Import temporary directory path'),
    '#description' => t('This must be an existing and writable path where input zipped data will be unpacked.'),
    '#default_value' => variable_get('wallymport_temp', file_directory_temp()),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
    
  $form['wallymport_debug'] = array(
    '#type' => 'radios',
    '#title' => t('Import debug level'),
    '#description' => t('When creating your own import definition file, if you set this value to <em>No data creation</em>, no nodes, files, or locations will be written to the database. If you set this value to <em>Verbose</em> or higher, check html source of the import result for debug info.'),
    '#default_value' => variable_get('wallymport_debug', 0),
	'#options' => array(t('No debug'), t('Verbose'), t('No data creation')),
    '#required' => TRUE,
  );

  $vocab_list=array();
  $vocabularies=taxonomy_get_vocabularies();
  foreach($vocabularies as $key => $vocabulary) {
	  $vocab_list[$key]=$vocabulary->name;
  }

  $form['wallymport_destinationpath'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Destination Path"'),
    '#description' => t('Choose the vocabulary corresponding to the destination path. Import module will use this vocabulary to match the DestinationPath element.'),
    '#default_value' => variable_get('wallymport_destinationpath', 0),
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

  $form['wallymport_classifiedtagtaxonomy'] = array(
    '#type' => 'select',
    '#title' => t('Taxonomy "Classified terms"'),
    '#description' => t('Choose the vocabulary corresponding to the classified taxonomy root.'),
    '#default_value' => variable_get('wallymport_classifiedtagtaxonomy', 0),
    '#options' => $vocab_list,
    '#required' => TRUE,
  );  

  $form['wallymport_taxonomy_recusive'] = array(
    '#type' => 'select',
    '#title' => t('Is the Taxonomies recusive?'),
    '#description' => t('If TRUE, node will be associated with all the term in taxonomy PATH /term1/term2/term3. Else only to the last term (term3).'),
    '#default_value' => variable_get('wallymport_taxonomy_recusive', TRUE),
    '#options' => array("true"=>"true", "false"=>"false"),
    '#required' => TRUE,
  );  


 $form['wallymport_defaultuser'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal user used for imports.'),
    '#description' => t('This user will be used as author for creating content.'),
    '#default_value' => variable_get('wallymport_defaultuser', 1),
    '#size' => 3,
    '#maxlength' => 8,
    '#required' => TRUE,
  );
  
  return system_settings_form($form);
}

/**
 * Validate function for the settings form.
 */
function wallymport_page_admin_form_validate($form, &$form_state) {
  
  $path = $form_state['values']['wallymport_source'];
  $dir_exist=file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist) {
    form_set_error('wallymport_source', t('The path provided is not an existing writable directory.'));
  }
  
  $file = $form_state['values']['wallymport_definition'];
  $file_readable=is_readable($file);
  if (!$file_readable) {
    form_set_error('wallymport_definition', t('The filename provided is not an existing readable file.'));
  }

  
}





