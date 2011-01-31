<?php
// $Id: view.inc,v 1.167.2.7 2010/03/22 20:55:11 merlinofchaos Exp $
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

  var $node;
  var $form_state;
  var $base_table = 'node';

  /**
   * Constructor
   */
  function Prenode($nid){
    /*
    $prenode=PreNode::LoadPrenodeFromNid($nid);
    if($prenode){
    }
    else{ 
      $prenode=PreNode::ConvertNodeToPrenode(node_load($nid));
    }*/
    $node=node_load($nid);
    if(!$node){
      drupal_set_message("WallyEdit: node not found","error");    
      watchdog("error","WallyEdit: node not found");    
    }
    
    $this->node=$node;
    $this->getFormState();
    return $this;
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
      'cache name' => $this->node->nid,
     "action"=>'/node/6/edit2/ajax',
    );
    if($cache = ctools_object_cache_get('prenode', $this->node->nid)){
     $form_state=$cache;
     $form_state['type']= $type_name;
     $form_state['type']= $type_name;
     //$form_state[$type_name]['#node']=$this->node;
  
    }
    else{
      $form_state['type']= $type_name;
      $form_state[$type_name]['#node']=$this->node;
      $form_state[$type_name]['type']=$type_name;
  
      //$form_state[$type_name]['#form_state']=$this->node;
      
      $type=wydit_get_infos_type( $type_name);
      $fields = $type['fields'];
    
      foreach($fields as $field_name=>$infos){
        if($infos['display_settings']["wallyedit"]>1){
          foreach($this->node->{$field_name} as $key=>$value){
            $temp_node=node_load($value['nid']);
            $form_state[$type_name][$field_name][$key]["#node"]=$temp_node;
            $form_state[$type_name][$field_name][$key]["type"]=$temp_node->type;
          }
          /*
        foreach($this->node->{$field_name} as $key=>$value){
            $temp_node=node_load($value['nid']);
            
            $form_state[$field_name][$key]["#form_state"]=$temp_node;
            dsm($form_state[$type_name][$field_name]);
            
          }
          */
        }
      }
    }
    $this->form_state=$form_state;
    return $form_state;
  }
  
  static function LoadPrenodeFromNid($nid){
    ctools_include('object-cache');
    return ctools_object_cache_get('prenode', $nid);
  }
  static function ConvertNodeToPrenode($node){
    
    return $node;
  }
}
/**
 * @}
 */
