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
  var $cache_name;
  var $node;
  var $form_state;
  var $base_table = 'node';
  
  /**
   * Constructor
   */
  function Prenode($cache_name = NULL){
    $this->cache_name=$cache_name;
    
    if($cache_name){
      $node=node_load($cache_name);
      if(!$node){
        //this not a node, maybe a prenode ?
        ctools_include('object-cache');
        
        
        if($cache = ctools_object_cache_get('prenode', $cache_name)){
        //Case of a prenode in creation mode
          $this->form_state=$cache;
          $this->form_state['cache name']= $cache_name;
        }
        else{
          drupal_set_message("WallyEdit: node not found","error");
          watchdog("error","WallyEdit: node not found");
        }
      }
      else{
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
 //     "action"=>'/node/'.arg(1).'/edit2/ajax',
    );
    
    if($cache = ctools_object_cache_get('prenode', $form_state['cache name'])){
      $form_state=$cache;
      $form_state['type']= $type_name;
      //$form_state[$type_name]['#node']=$this->node;
    } else {
      $form_state['type']= $type_name;
      $form_state[$type_name]['#node']=$this->node;
      $form_state[$type_name]['type']=$type_name;
  
      //$form_state[$type_name]['#form_state']=$this->node;
      
      $type=wydit_get_infos_type( $type_name);

      $fields = $type['fields'];
      foreach($fields as $field_name=>$infos){
        if($infos['display_settings']["wallyedit"]>1){
          $form_state[$type_name][$field_name]=array();
          if($infos['display_settings']["wallyedit"]==2){
            
            $type=wydit_get_inline_type($infos['referenceable_types']);
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
          /*
        foreach($this->node->{$field_name} as $key=>$value){
            $temp_node=node_load($value['nid']);
            
            $form_state[$field_name][$key]["#form_state"]=$temp_node;
            
          }
          */
        }
      }
    }
    $this->form_state=$form_state;
    return $form_state;
  }
  
  function ConvertNodeToPrenode($node){
    $this->setNode($node);
    $this->setFormState();
  }

  static function LoadPrenodeFromNid($nid){
    ctools_include('object-cache');
    return ctools_object_cache_get('prenode', $nid);
  }
}
/**
 * @}
 */
