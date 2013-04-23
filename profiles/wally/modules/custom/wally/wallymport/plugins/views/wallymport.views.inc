<?php
// $Id: signup.views.inc,v 1.4.2.7 2009/07/22 21:47:47 dww Exp $


/**
 * @file
 * Provides support for Views integration.
 *
 * This exposes signup-related data and declares handlers (fields, filters)
 * and plugins (argument validators).
 */



/*
 * Implementation of hook_views_data().
 */

function wallymport_views_data() {
  $data['wallymport_logs']['table']['base'] = array(
    'field' => 'lid',
    'title' => t('Wallymport logs'),
    'help' => t('Logs of wallymport.'),
  );
  
  // Define the base group of the signup table.  Fields that don't
  // have a group defined will go into this field by default.
  $data['wallymport_logs']['table']['group'] = t('Wallymport');
 
  $data['wallymport_logs']['lid'] = array(
    'title' => t('Log ID'),
    'help'  => t('The unique ID of the log'), 
    // Information for displaying the field
    'field' => array(
      'handler'        => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting status as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help'    => t('ID Filter'),
      'label'   => "Statut d'intervention",
    ),
    // Information for sorting on status
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help'    => t('ID Sort'),
    ),
  );
  $data['wallymport_logs']['importer_name'] = array(
    'title' => t('Importer name'),
    'help'  => t('The unique name of the importer'),
    'field' => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help'    => t("Importe name Sort"),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'help'    => t("Importer name Filter"),
      'label'   => t("Importer name Filter"),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['wallymport_logs']['date'] = array(
    'title' => t('Date'),
    'help'  => t("La date de création de l'intervention"),
    'field' => array(
      'handler'        => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'help'    => t("Date of the import"),
      'label'   => t("Date of the import"),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'help'    => t("Sort by date"),
    ),
  );
  $data['wallymport_logs']['location'] = array(
    'title' => t('File location'),
    'help'  => t('Where the file is located'),
    'field' => array(
      'handler'        => 'wallymport_handler_field_location',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help'    => t("file location Sort"),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'help'    => t("File location Filter"),
      'label'   => t("File location Filter"),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['wallymport_logs']['filename'] = array(
    'real field' => 'location',
    'title' => t('File Name'),
    'help'  => t('The file name'),
    'field' => array(
      'handler'        => 'wallymport_handler_field_filename',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help'    => t("File Name Sort"),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'help'    => t("File Name Filter"),
      'label'   => t("File Name Filter"),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['wallymport_logs']['success'] = array(
    'real field' => 'log',
    'title' => t('Success'),
    'help'  => t('Indicate if the process succeed'),
    'field' => array(
      'handler'        => 'wallymport_handler_field_success',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help'    => t("Success Sort"),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'help'    => t("Success Filter"),
      'label'   => t("Success Filter"),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['wallymport_logs']['message'] = array(
    'real field' => 'log',
    'title' => t('Message'),
    'help'  => t('Error message'),
    'field' => array(
      'handler'        => 'wallymport_handler_field_message',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help'    => t("Message Sort"),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'help'    => t("Message Filter"),
      'label'   => t("Message Filter"),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  return $data;
}
/*
 * Implementation of hook_views_handlers().
 */
function wallymport_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'wallymport') .'/plugins/views/handlers',
    ),
    'handlers' => array(
      'wallymport_handler_field_success' => array(
        'parent' => 'views_handler_field_numeric',
      ),
  	  'wallymport_handler_field_message' => array(
         'parent' => 'views_handler_field_numeric',
      ),
  	  'wallymport_handler_field_filename' => array(
        'parent' => 'views_handler_field_numeric',
      ),
  	  'wallymport_handler_field_location' => array(
        'parent' => 'views_handler_field_numeric',
      ),
    ),
  );
}
