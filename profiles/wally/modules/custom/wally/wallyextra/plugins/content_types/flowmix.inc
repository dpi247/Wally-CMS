<?php
/*
 * Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'no title override' => FALSE,
  //'defaults' => _wallyextra_flowmix_defaultformvalues(),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
//  'edit form' => '',
  'add form' => array(
    'wallyflowmix_contenttype_summary_form'=>t('fff'),
    'wallyflowmix_contenttype_basicsetttings_form'=>t('RZA'),
    'wallyflowmix_contenttype_pluginselector_form'=>t('OLD DIRTY B'),
    'wallyflowmix_contenttype_pluginspecific_form'=>t('C.R.E.A.M'),
    'wallyflowmix_contenttype_finalstep_form'=>t('WU'),
  ),
    'edit form' => array(
    'wallyflowmix_contenttype_summary_form'=>t('fff'),
    'wallyflowmix_contenttype_basicsetttings_form'=>t('RZA'),
    'wallyflowmix_contenttype_pluginselector_form'=>t('OLD DIRTY B'),
    'wallyflowmix_contenttype_pluginspecific_form'=>t('C.R.E.A.M'),
    'wallyflowmix_contenttype_finalstep_form'=>t('WU'),
  ),
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);
function _wallyextra_flowmix_defaultformvalues(){
return array();
}


