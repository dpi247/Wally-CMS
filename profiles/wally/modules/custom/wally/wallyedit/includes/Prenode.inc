<?php
/**
 * @file prnode.inc
 * Provides the prenode object type and associated methods.
 */

/**
 * @defgroup prenode_objects Objects that represent a node from his editing point of view.
 * @{
 * @todo add description
 */

/**
 * An object to contain all of the data to generate a view, plus the member
 * functions to build the view query, execute the query and render the output.
 */
class Prenode{
  var $cache_name;
  var $node;
  var $form_state;
  var $profile_id;
  
  /**
   * Constructor
   */
  function Prenode($cache_name = NULL, $reload = FALSE, $profile_id = NULL, $break = FALSE){
    if($profile_id == NULL){
      $profile_id = 0;
    } else {
      $this->profile_id = $profile_id;
    }
    $this->cache_name = $cache_name;

    if($cache_name && !empty($cache_name)) {
      $node = node_load($cache_name);
      if(!$node) {
        //this not a node, maybe a prenode ?
        
        if($cache = prenode_get($cache_name)) {
          //Case of a prenode in edition mode
          $this->form_state = $cache;
          $this->form_state['cache name'] = $cache_name;
          $this->form_state['locked'] = (isset($this->form_state['locked']) && !$break) ? $this->form_state['locked'] : FALSE;
          $this->getFormState($break);
        } else {
          drupal_set_message(t('WallyEdit: node not found'), 'error');
          watchdog('WallyEdit', t('Node not found (nid : @nid) while constructing Prenode', array('@nid' => $cache_name)));
        }
      } else {
        //Case of a prenode in edition mode
        if(!$break && $cache = prenode_get($cache_name)) {
          $this->form_state = $cache;
          $this->setNode($node);
          $this->form_state['cache name'] = $cache_name;
          $this->form_state['locked'] = $break ? FALSE : $this->form_state['locked'];
        } else {
          $this->setNode($node);
        }
        $this->getFormState($break);
      }
    }

    return $this;
  }
  
  function ReturnPopulated() {
    $prenode = $this->form_state[$this->form_state['type']]['#node'];
    foreach($this->form_state[$this->form_state['type']] as $field_name => $field_content){
      if(strpos($field_name, 'field_') === 0){
        foreach($field_content as $elem){
          if ($elem['#node']) {
            $prenode->{$field_name.'_nodes'}[] = $elem['#node'];
          }
        }
      }
    }
    return $prenode;
  }
  
  function setNode($node) {
    $this->node = $node;
  }
  
  function getFormState($reset = FALSE) {
    if(isset($this->form_state) && $reset == FALSE && isset($this->form_state['type']) && !empty($this->form_state['type'])) {
      return $this->form_state;
    } else {
      return $this->setFormState($reset);
    }
  }

  function get_cache_name() {
    global $user;

    if (!isset($this->cache_name)) {
      $this->cache_name = md5(uniqid(rand(), TRUE));
    }

    return $this->cache_name;
  }
  
  function setFormState($reset = FALSE) {
    if (!isset($this->node) && isset($this->form_state)) {
      $this->setNode($this->form_state[$this->form_state['type']]['#node']);
    }
    $type_name = $this->node->type;

    module_load_include('inc', 'wallyedit', 'wallyedit.forms');
    
    if(!$reset && $cache = prenode_get($this->cache_name)) {
      $form_state = $cache;
      $form_state['type'] = $type_name;
    } else {
      global $user;
      $form_state = array(
        'ajax' => TRUE,
        're_render' => FALSE,
        'no_redirect' => TRUE,
        'cache name' => $this->get_cache_name(),
        'profile_id' => $this->profile_id,
        'parents' => array($type_name),
        'current_tab' => NULL,
        'type' => $type_name,
        'locked' => FALSE,
      );

      wydit_populate_form_state($form_state[$type_name], $this->node, $this->profile_id);
    }

    wydit_clean_and_save_form_state($form_state);
    $this->form_state = $form_state;
    return $form_state;
  }
  
  function ConvertNodeToPrenode($node){
    $this->setNode($node);
    $this->setFormState();
  }
  
  function setCurrentProfile($current_profile) {
    $this->profile_id = $current_profile;
    $this->form_state['profile_id'] = $current_profile;
  }

  static function LoadPrenodeFromNid($nid){
    return prenode_get($nid);
  }
}

function prenode_load($nid, $args) {
  module_load_include('inc', 'wallyedit', 'includes/config_api');
  $profile_arg = isset($args[4]) ? $args[4] : wallyedit_get_default_profile();

  $prenode = new Prenode($nid, FALSE, $profile_arg, $_GET['break']);
  $prenode->setCurrentProfile($profile_arg);

  // Let modules alter the prenode after it has been loaded from the database
  wallyedit_invoke_prenodeapi($prenode->form_state, 'load');

  return $prenode;
}

function prenode_save(&$form_state) {
  global $user;
  $form_state['locked'] = array(
    'uid' => $user->uid,
    'updated' => time(),
  );

  // Let modules modify the prenode before it is saved to the database.
  wallyedit_invoke_prenodeapi($form_state, 'presave');

  module_load_include('inc', drupal_get_path('module', 'wallyedit').'/includes/index_prenodes');
  wallyedit_index_prenodes_set($form_state);

  if(function_exists('memcache_variable_set')) {
  	$day_in_memecache = 2;
  	$expire = $day_in_memecache*60*60*24;
  	$prenode_object = new stdClass;
  	$prenode_object->cache = $form_state;
    cache_set($form_state['cache name'], $prenode_object, 'prenode', $expire);
  } else {
  	module_load_include('inc', 'wallyedit', 'includes/object-cache');
  	wallyedit_object_cache_set($form_state['cache name'], $form_state);
  }
  // Let modules reacts after the prenode is saved to the database
  wallyedit_invoke_prenodeapi($form_state, 'save');
}

function prenode_delete($prenode) {
  // Let modules reacts after the prenode is saved to the database
  wallyedit_invoke_prenodeapi($prenode, 'delete');
  
  module_load_include('inc', drupal_get_path('module', 'wallyedit').'/includes/index_prenodes');
  wallyedit_index_prenodes_clear($prenode['cache name']);

  if(function_exists('memcache_variable_set')) {
  	cache_clear_all($prenode['cache name'], 'prenode');
  } else {
    module_load_include('inc', 'wallyedit', 'includes/object-cache');
  	wallyedit_object_cache_clear($prenode['cache name']);
  }
}

function prenode_delete_all() {
  // Let modules reacts after the prenode is saved to the database
  $fake_prenode = new stdClass();
  wallyedit_invoke_prenodeapi($fake_prenode, 'delete_all');

  if(function_exists('memcache_variable_set')) {
  	cache_clear_all('*', 'prenode');
  } else {
    module_load_include('inc', 'wallyedit', 'includes/object-cache');
  	wallyedit_object_cache_clear_all();
  }
  
  module_load_include('inc', 'wallyedit', 'includes/index_prenodes');
  wallyedit_index_prenodes_clear_all();
  
  drupal_set_message(t('All PreNodes have been cleared'));
}

function prenode_get($cache_name, $skip_cache = FALSE) {
  global $user;
  
  if(function_exists('memcache_variable_set')) {
  	$prenode = cache_get($cache_name, 'prenode');
  	$prenode = $prenode->data->cache;
  } else {
    module_load_include('inc', 'wallyedit', 'includes/object-cache');
  	$prenode = wallyedit_object_cache_get($cache_name, $skip_cache);
  }

  if ($prenode['locked'] && $prenode['locked']['uid'] == $user->uid) {
    $prenode['locked'] = FALSE;
  }
  
  return $prenode;
}

/**
 * @}
 */
