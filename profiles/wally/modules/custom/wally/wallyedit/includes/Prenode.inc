<?php
/**
 * @file prnode.inc
 * Provides the prenode object type and associated methods.
 */

/**
 * @defgroup prenode_objects Objects that represent a node from his editing point of view.
 * @{
 * Todo
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
    if($profile_id==NULL){
      $profile_id = 0;
    }
    else{
      $this->profile_id = $profile_id;
    }
    $this->cache_name = $cache_name;

    if($cache_name && !empty($cache_name)) {
      $node=node_load($cache_name);
      if(!$node) {
        //this not a node, maybe a prenode ?
        ctools_include('object-cache');
        
        if($cache = ctools_object_cache_get('prenode', $cache_name)){
        //Case of a prenode in creation mode
          $this->form_state=$cache;
          $this->form_state['cache name']= $cache_name;
        } else {
          drupal_set_message("WallyEdit: node not found","error");
          watchdog("error","WallyEdit: node not found");
        }
      }
      else {
        //Case of a prenode in edition mode
        $this->setNode($node);
        $this->getFormState();
      }
    }
    
    return $this;
  }
  
  function ReturnPopulated(){
    $prenode = $this->form_state[$this->form_state['type']]['#node'];
    foreach($this->form_state[$this->form_state['type']] as $field_name=>$field_content){
      if(strpos($field_name, 'field_')===0){
        foreach($field_content as $elem){
          if ($elem['#node'])
            $prenode->{$field_name.'_nodes'}[]=$elem['#node'];
        }
      }
    }
    return $prenode;
  }
  
  function setNode($node){
    $this->node=$node;
  }
  
  function getFormState($reset=false){
    if(isset($this->form_state) and $reset==false)
      return $this->form_state;
    else{
      return $this->setFormState();
    }
  }
  
  function setFormState($reset=false){
    ctools_include('object-cache');
    $type_name=$this->node->type;
    $form_state = array('ajax' => TRUE);
    $form_state += array(
      're_render' => FALSE,
      'no_redirect' => TRUE,
    );
    
    $form_state = array(
      'cache name' => wydit_get_cache_name($this->cache_name),
      'profile_id' => $this->profile_id,
    );
    
    if($cache = ctools_object_cache_get('prenode', $form_state['cache name'])){
      $form_state = $cache;
      $form_state['type'] = $type_name;
    } else {
      $form_state['type'] = $type_name;
      $form_state[$type_name]['#node'] = $this->node;
      $form_state[$type_name]['type'] = $type_name;
      
      $type=wydit_get_infos_type( $type_name);

      $fields = $type['fields'];
      module_load_include("inc",'wallyedit','includes/page_form_display_tabs');
      $flat_fields = wyditadmin_get_fields_flat($this->profile_id, $type_name);
      $existing_fields = wyditadmin_get_existing_fields($type_name);

      foreach($flat_fields as $field_name => $infos){
        $orig_field_infos = $existing_fields[$field_name];
        if($infos['display']>1){
          $form_state[$type_name][$field_name]=array();
          if($infos['display']==2){
            $type=wydit_get_inline_type($orig_field_infos['referenceable_types']);
            $main_node=new stdClass;
            $main_node->type=$type;
            $form_state[$type_name][$field_name][0]['#node']=$main_node;
            $form_state[$type_name][$field_name][0]['type']=$main_node->type;
          }

          if ($this->node->{$field_name}) {
            foreach($this->node->{$field_name} as $key=>$value){
              $temp_node=node_load($value['nid']);
              $form_state[$type_name][$field_name][$key]["#node"]=$temp_node;
              $form_state[$type_name][$field_name][$key]["type"]=$temp_node->type;
            }
          }
        }
      }
    }
    wydit_clean_and_save_form_state($form_state);
    $this->form_state=$form_state;
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
    ctools_include('object-cache');
    return ctools_object_cache_get('prenode', $nid);
  }
}
/**
 * @}
 */
