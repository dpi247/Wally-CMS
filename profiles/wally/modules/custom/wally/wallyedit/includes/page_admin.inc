<?php

function wallyedit_page_admin($limit = 25){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');
  
  $count_query = "SELECT count(*) from {ctools_object_cache} where sid = '".session_id()."' AND obj = 'prenode'";
  $result = pager_query("SELECT * from {ctools_object_cache} where sid = '".session_id()."' AND obj = 'prenode'", $limit, 0, $count_query);
  
  while($obj=db_fetch_object($result)){
    //$row['sid']=$obj->sid;
    $temp_array = unserialize($obj->data);
    $row['title'] = $temp_array[$temp_array['type']]['#node']->title;
    $row['name']=$obj->name;
    $row['data']= $temp_array[$temp_array['type']]['#node']->type;
    $row['action_urls']=l('edit', wydit_get_url_action('edit',$obj->name));
    $row['action_urls'].=' '.l('delete', wydit_get_url_action('delete',$obj->name));
    $rows[]=$row;
  }
  $headers=array('Title','Nid','Type','Action(s)');
  return theme('wallyedit_admin',$rows,$headers);
}

function wallyedit_page_admin_form(&$form_state){
  $form = array();
  $form['limit'] = array(
    '#type' => 'select',
    '#title' => t('Prenodes par page'),
    '#options' => array(25=>25, 50=>50, 75=>75, 100=>100), 
  );
  $form['refresh'] = array(
    '#type' => 'button',
    '#value' => t('Refresh'),
  );
  $limit = ($form_state['post']['limit'] > 0) ? $form_state['post']['limit'] : 25;
  $form['#suffix'] = wallyedit_page_admin($limit);
  return $form;
}

function wallyedit_prenode_delete_confirm(&$form_state, $prenode = NULL){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');
  if(!empty($prenode)){
    $form_state['cache name'] = $prenode->cache_name;
    return confirm_form($form,
            t('Are you sure you want to delete the prenode with the title :'.$prenode->form_state[$prenode->form_state['type']]['#node']->title.' and the id : '.$prenode->cache_name.' ?'),
            'admin/content/wallyedit', t('This action cannot be undone.'),
            t('Delete'), t('Cancel'));
  }
}

function wallyedit_prenode_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    ctools_object_cache_clear('prenode', $form_state['cache name']);
    drupal_set_message(t('The item has been deleted.'));
  }
  $form_state['redirect'] = 'admin/content/wallyedit';
  return;
}
