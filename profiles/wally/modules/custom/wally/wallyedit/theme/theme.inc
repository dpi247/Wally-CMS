<?php

function theme_wallyedit_admin($rows=array(),$headers){
  drupal_set_title("Wallyedit Admin");

  foreach($rows as $row){
    $collumn[]=array('data'=>$row['sid']);
    $collumn[]=array('data'=>$row['name']);
    $collumn[]=array('data'=>$row['data'][$row['data']['type']]['#node']->type);
    $collumn[]=array('data'=>l('edit',$row['edit_action_url']));
    $collumns[]=$collumn;
  }
  
  $output=theme('table',$headers, $collumns);
  return $output;
}

function theme_wallyedit_page_createmode($form,$prenode){
  drupal_add_css(drupal_get_path('module', 'wallyedit') . '/css/wallyedit.css');
  $output='<div id="wallyedit_messages">';
  $output.='</div>';
  
  $output.='<div id="wallyedit_container">';
  $output.=$form;
  $output.='</div>';

  return $output;
}

function theme_wallyedit_page_editmode($form,$prenode){
  drupal_add_css(drupal_get_path('module', 'wallyedit') . '/css/wallyedit.css');
  $output='<div id="wallyedit_messages">';
  $output.='</div>';
  
  $output.='<div id="wallyedit_container">';
  $output.=$form;
  $output.='</div>';

  return $output;
}

function theme_wallyedit_preview_id($cache_name, $dest = 'no_dest'){
  $output.='<div id="wallyedit_preview">';
  $output.= theme('wallyedit_preview_iframe', $cache_name, $dest);
  $output.='</div>';
  
  return $output;
}

function theme_wydit_selector_theme($form){
  $header=array();
  $header[] = t('Order');
  $header[] = t('Title');
  $header[] = t('Type');
  $header[] = t('Action(s)');
  
  foreach($form as $key=>$element){
    if(is_integer($key)){
      $row = array();
      
      /*
      $row[] = array('data' =>drupal_render($element['weight']));
      $row[] = array('data' => drupal_render($element['title']));
      $row[] = array('data' => drupal_render($element['type']));
      $row[] = array('data' => drupal_render($element['edit_action']).' '.drupal_render($element['delete_action']));
      */
      
      $row[] =drupal_render($element['weight_'.$key]);
      $row[] = drupal_render($element['title']);
      $row[] = drupal_render($element['type']);
      $row[] = drupal_render($element['edit_action']).' '.drupal_render($element['delete_action']);
          
      $rows[] = array('data'=>$row, 'class'=>'draggable');
    }
  }
  
  $table_id='table_order_'.$form['#parents'][count($form['#parents'])-1];

  drupal_add_tabledrag($table_id, 'order', 'sibiling', 'order_weightss', NULL, NULL,FALSE);    
  $output = theme('table', $header, $rows,array('id'=>$table_id));
  
  $output .= drupal_render($form['add_existing_action']);
  $output .= drupal_render($form["add_new_action"]);
  
  return  $output;
}
