<?php

function dynamicfeedback_page_render($ext_ref) {
  $GLOBALS['conf']['cache'] = FALSE;
  $options = wally_variable_get('dynamicfeedback_display_options', array());

  if (is_numeric($ext_ref)) {
    // If it's a numeric value it's probably a nid
    $nid = $ext_ref;
  } else {
    $array_of_nodes = wallytoolbox_get_node_by_cckfield(array('field_externalreference' => $ext_ref));
    if ($array_of_nodes && count($array_of_nodes) != 1) {
      $nid = FALSE;
    } else {
      $node = $array_of_nodes[0];
      $nid = $node->nid;
    }
  }
  
  $datas = array();
  if ($nid) {
    if($full_node = node_load($nid)) {
      $datas['options'] = array();

      foreach ($options as $option) {
        if ($option) {
          $tmp_datas = module_invoke_all('dynamicfeedback_options', 'content', $option, $full_node);
          if (!empty($tmp_datas)) {
            $datas['options'][$option] = $tmp_datas;
          }
        }
      }
      
      $datas['title'] = $full_node->title;
      $datas['nid'] = $nid;
      $datas['base_url'] = $_SERVER['HTTP_HOST'];
    }
  }

  if (empty($datas)) {
    $datas = array('nid' => FALSE);
  }

  print theme('dynamicfeedback_page_render', $datas);
}
