<?php 

/*
 * Page callback for the administration of filters
 */
function wallypressfilters_page_admin(){
  $content = '';
  
  $filters = wallypressfilters_getfilters();    
  
  if ($filters != NULL){
    $header = array(t('Name'), t('Description'), t('Operations'));
    foreach ($filters as $filter){
      $rows[] = array(
        $filter->name,
        $filter->description,
        '<a href = "/admin/wally/wallypressfilters/settings/edit/'.$filter->fid.'">'.t('Edit').'</a>',
      );
    }
    $content .= theme_table($header,$rows);
  } else {
    $content .= t('There is no filter. To add one click on "Create filter".');
  }
  
  return $content;
}
/*
 * Form to create/edit filter
 */
function wallypressfilters_form_addfilter_form($form_state, $fid = NULL){
  
  $form['wallypressfilters']['#tree'] = TRUE;
  $form['wallypressfilters']['name'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Name'),
    '#description' => t('The unique Name of the filter.'),
    '#size'        => 80,
    '#maxlength'   => 100,
    '#required'    => TRUE,
  );
  
  $form['wallypressfilters']['description'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Description'),
    '#description' => t('A description of the filter'),
    '#size'        => 80,
    '#maxlength'   => 1000,
  );
  $form['wallypressfilters']['tags'] = array(
    '#type'          => 'textarea',
    '#title'         => t('List of the tags'),
    '#description'   => t('The tags who will be filtered. Enter one value per line, without the "&lt;", "&gt;".'),
    '#default_value' => '',
    '#cols'          => 10,
    '#rows'          => 15,
    '#resizable'     => TRUE,
  );
  if ($fid != NULL){
    $filter = wallypressfilters_getfilterbyid($fid);
    $form['wallypressfilters']['fid'] = array('#type' => 'value', '#value' => $filter->fid);
    $tags = wallypressfilters_gettagsfromfilter($filter->name);
    $form['wallypressfilters']['name']['#default_value'] = $filter->name;
    $form['wallypressfilters']['description']['#default_value'] = $filter->description;
    
    $tags_default_value = '';
    $space = '';
    foreach ($tags as $tag){
      $tags_default_value .= $space.$tag->tag;
      $space = "\n";
    }
    $form['wallypressfilters']['tags']['#default_value'] = $tags_default_value;
  }
  $form['sumbit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}
/*
 * Validation of the wallypresfilters_form_addfilter_form
 */
function wallypressfilters_form_addfilter_form_validate($form, &$form_state){
  
  $form_state['values']['wallypressfilters']['name'] = strtolower(check_plain($form_state['values']['wallypressfilters']['name']));
  $form_state['values']['wallypressfilters']['description'] = check_plain($form_state['values']['wallypressfilters']['description']);
  
  //If it's edition
  if (isset($form_state['values']['wallypressfilters']['fid'])){
    $form_state['values']['update'] = array('fid');
  } else {
    //check if the name is correct
    if (preg_match('/[^a-z0-9_]/', $form_state['values']['wallypressfilters']['name'])) {
      form_set_error('wallypressfilters][name', t('Filter name must be alphanumeric or underscores only.'));
    } 
    //check if the name already exists
    elseif (wallypressfilters_getfilterbyname($form_state['values']['wallypressfilters']['name']) != NULL){
      form_set_error('wallypressfilters][name', t('A filter with this name already exists.'));
    }
    $form_state['values']['update'] = array();
  }
  $tags = explode("\r\n", $form_state['values']['wallypressfilters']['tags']);
  $form_state['values']['wallypresstags'] = array();
  foreach ($tags as $tag){
    //Check if the tag is correct
    if (preg_match('/[^a-z0-9_-]/', $tag)) {
      form_set_error('wallypressfilters][tags', t('Tag name must be alphanumeric or underscores only.'));
    } 
    //Ignore space
    elseif ($tag == ''){
      ;
    } 
    //Check is the tag exists two times
    elseif (array_key_exists($tag, $form_state['values']['wallypresstags'])){
      form_set_error('wallypressfilters][tags', t('There can not be two identical tags.'));
    } else {
      $update = array();
      //if it's edition
      if (wallypressfilters_gettagbynameandfilter($tag, $form_state['values']['wallypressfilters']['name'])){
        $update = array('tid');
      }
      $form_state['values']['wallypresstags'][$tag] = array(
        'tag' => $tag,
        'filter' => $form_state['values']['wallypressfilters']['name'],
        'html_tag' => 'span',
        'update' => $update,
      );
    }
  }
}
/**
* Submit of the wallypresfilters_form_addfilter_form
* */
function wallypressfilters_form_addfilter_form_submit($form, &$form_state){
  
  //save filter
  $result = drupal_write_record('wallypressfilters', $form_state['values']['wallypressfilters'], $form_state['values']['update']);
  $message = '';
  if ($result){
    $message = 'The filter as been created/updated';
    foreach ($form_state['values']['wallypresstags'] as $tag){
      //Save tags
      $result = drupal_write_record('wallypressfilters_tags', $tag, $tag['update']);
      if ($result){
        $message .= '<br>The tag "'.$tag['tag'].'" has been created/updated'; 
      } else {
        $message .= '<br>The tag "'.$tag['tag'].'" has not been created/updated';
      }
    }
    //Delete tags
    foreach (wallypressfilters_gettagsfromfilter($form_state['values']['wallypressfilters']['name']) as $filter_tags){
      if (!array_key_exists($filter_tags->tag, $form_state['values']['wallypresstags'])){
        db_query("DELETE FROM {wallypressfilters_tags} WHERE tid = %d", $filter_tags->tid);
        $message .= '<br>The tag '.$filter_tags->tag.' has been deleted';
      }
    }
  } else {
    $message = 'The filter as not been created/udpated';
  }
  drupal_set_message(t($message));
  drupal_goto('admin/wally/wallypressfilters/settings/general');
}