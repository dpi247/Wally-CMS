<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
function wallycontenttypes_galleryphoto_ctools_content_types() {
    return array(
      'single' => TRUE,
      'title' => t('Package related Gallery Photo'),
      'icon' => 'icon_node.png',
      'description' => t('allows the display of a photo gallery.'),
      'category' => t('Packages'),
   'defaults' => array(),
    );
}

function wallycontenttypes_galleryphoto_content_type_edit_form(&$form, &$form_state) {

  $conf = $form_state['conf'];
      
  
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($conf['identifier']) ? $conf['identifier'] : '',
    '#title' => t('Node identifier'),
    '#description' => t('...'),
  );
  

  return $form;  
}

function wallycontenttypes_galleryphoto_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['identifier']=$form['identifier']['#value'];
}

function wallycontenttypes_galleryphoto_content_type_render($subtype, $conf, $panel_args, $context) {
   
  
  $block = new stdClass();
  
  
  

    $identifier = $conf['identifier'];
        
    $block->title = "Gallery photo Tag List.";
    
    $mynode = node_load($identifier);
    wallycontenttypes_packagepopulate($mynode,"");
    dsm($mynode); 
    $block->content = theme("wallyct_galleryphoto",$mynode); 
    
    


  return $block;
}
