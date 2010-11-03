<?php
function wallogic_page_admin_list_block(){

  $query="Select wb.name,wb.view_id,wb.view_id,wb.view_vid,wb.comment, vd.display_title, vv.name as view_name from {wallogic_block} wb LEFT JOIN {views_view} vv ON vv.vid=wb.view_vid LEFT JOIN {views_display} AS vd ON vd.id=wb.view_id";
  $results=db_query($query);
  
  $blocks=array();
  while($result=db_fetch_object($results)){
    $blocks[]=$result;
  }
  
  $output="";
  $output.=theme('wallogic_block_list',$blocks);
  $output.=drupal_get_form('wallogic_admin_list_block_form');
  
  return  $output;
  
}

function wallogic_admin_list_block_form($form_state){
  $form=array('#tree'=>true);
  $form['block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create a new wallogic block'),
     '#description' => t('Description of a Wallogic Block'),
  );
 $form['block']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Wallogic system name'),
    '#required'=>true,
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t("The system name of a wallogic block. only alphanumeric, '-' and '_' characters are allowed"),
  );
 $form['block']['view'] = array(
    '#type' => 'select',
    '#options'=> wallogic_get_redac_view_options(),
   '#title' => t('view associated with the block'),
    '#required'=>true,
  );
 $form['block']['comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#required'=>true,
    '#size' => 60,
    '#maxlength' => 512,
    '#description' => t("Describe the block in few lines"),
  );
 $form['block']['is_active'] = array(
  '#type' => 'value',
  '#value'=>1

 );

$form['block']['submit'] = array(
  '#type' => 'submit',
  '#value' => t('Save'),
);
return $form;
}

function wallogic_admin_list_block_validate($form, &$form_state){
  
}
function wallogic_admin_list_block_form_submit($form, &$form_state){
  $values=$form_state['values']['block'];
  $view=split('\+',$values['view']);
  unset($values['view']);
  $values['view_vid']=$view[0];
  $values['view_id']=$view[1];
  drupal_write_record('wallogic_block',$values);
}

function theme_wallogic_block_list($blocks){
  dsm($blocks);
  $headers=array(t("Block's name"),t("View"),t("Comment"),t("Actions"));
  $rows=array();
  foreach($blocks as $block){
    $rows[]=array($block->name,$block->view_name.' '.$block->display_title.' ('.$block->view_id.')',$block->comment);
  }
  return theme_table($headers,$rows);

  
}

