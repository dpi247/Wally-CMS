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
  var $base_table = 'node';
  
  /**
   * Constructor
   */
  function Prenode($cache_name = NULL, $reload = FALSE, $profile_id = NULL){
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
        } else {
          drupal_set_message(t('WallyEdit: node not found'), 'error');
          watchdog('WallyEdit', t('Node not found (nid : @nid) while constructing Prenode', array('@nid' => $cache_name)));
        }
      } else {
       //Case of a prenode in edition mode
       if($cache = prenode_get($cache_name)) {
          $this->form_state = $cache;
          $this->setNode($node);
          $this->form_state['cache name'] = $cache_name;
        } else {
          $this->setNode($node);
          $this->getFormState();
        }
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
    if(isset($this->form_state) && $reset == FALSE) {
      return $this->form_state;
    } else {
      return $this->setFormState();
    }
  }

  function get_cache_name() {
    global $user;

    if (!isset($this->cache_name)) {
      $this->cache_name = md5(uniqid(rand(), TRUE));
    }

    return $this->cache_name;
  }
  
  function setFormState($reset = FALSE){
    $type_name = $this->node->type;
    $form_state = array('ajax' => TRUE);
    $form_state += array(
      're_render' => FALSE,
      'no_redirect' => TRUE,
    );
    
    $form_state = array(
      'cache name' => $this->get_cache_name(),
      'profile_id' => $this->profile_id,
    );
    
    if($cache = prenode_get($this->cache_name)) {
      $form_state = $cache;
      $form_state['type'] = $type_name;
    } else {
      global $user;
      $form_state['parents'] = array($type_name);
      $form_state['current_tab'] = NULL;
      $form_state['type'] = $type_name;
      
      module_load_include('inc', 'wallyedit', 'includes/page_form_display_tabs');
      
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

/**
 * @}
 */
