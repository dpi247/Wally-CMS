<?php
function theme_wallyedit_admin($rows=array(),$headers){

drupal_set_title("Wallyedit Admin");
dsm($rows);

foreach($rows as $row){
  $collumn[]=array('data'=>$row['sid']);
  $collumn[]=array('data'=>$row['name']);
  $collumn[]=array('data'=>$row['data'][$row['data']['type']]['#node']->type);
  $collumn[]=array('data'=>l('edit',$row['edit_action_url']));
  dsm($row['edit_action']);
    
  $collumns[]=$collumn;
}
$output=theme('table',$headers, $collumns);
return $output;
}
function theme_content_display_overview_form($form){

}
function theme_wallyedit_display_overview_form($form){
}
function theme_wallyedit_page_node_edition($form,$prenode){
  

  drupal_add_css(drupal_get_path('module', 'wallyedit') . '/css/wallyedit.css');
  $output='<div id="wallyedit_messages">';
  $output.='</div>';
  
  $output.='<div id="wallyedit_container">';
  $output.=$form;
  $output.='</div>';

return $output;

}
function theme_wydit_selector_theme($form){
  
  $header=array();
  $header[] = t('Order');
  $header[] = t('Title');
  $header[] = t('Action(s)');
  foreach($form as $key=>$element){
    if(is_integer($key)){
          $row = array();
      
    $row[] = array('data' =>$key);
    $row[] = array('data' => drupal_render($element['title']));
    $row[] = array('data' => drupal_render($element['edit_action']).' '.drupal_render($element['delete_action']));
                
    $rows[] = $row;
    
    }
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form['add_existing_action']);
  $output .= drupal_render($form["add_new_action"]);
  return  $output;
}
function theme_wallyedit_form_pagenodeedition_form($form,$prenode){
  
  $output.='  <div id="wallyedit_package_primaryinfos" class="wallyedit_level0">';
    $output.=drupal_render($form);
  $output.='  </div>';
  
  $output.='  <div id="wallyedit_mainobject" class="wallyedit_level1">';
    $output.='    <h3>Main Object</h3>';
    //$output.=drupal_render();
  $output.='  </div>';
  
  $output.='  <div id="wallyedit_embededobjects" class="wallyedit_level1">';
    //$output.=drupal_render($form['package_embededobjects']);
  $output.='  </div>';
  
  $output.='  <div id="wallyedit_linkedobjects" class="wallyedit_level1">';
   // $output.=drupal_render($form['package_linkedobjects']);
  $output.='  </div>';
  
  $output.='  <div id="wallyedit_mainobject_secondaryinfos" class="wallyedit_level0">';
   // $output.=drupal_render($form['package_secondary']);
  $output.='  </div>';
  
  $output.=drupal_render($form);
  

return $output;
}