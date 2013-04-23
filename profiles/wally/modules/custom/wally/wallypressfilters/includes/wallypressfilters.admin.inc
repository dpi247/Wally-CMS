<?php 

/**
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
        '<a href = "/admin/wally/wallypressfilters/settings/edit/'.$filter->fid.'">'.t('Edit').'</a>, <a href = "/admin/wally/wallypressfilters/settings/delete/'.$filter->fid.'">'.t('Delete').'</a>',
      );
    }
    $content .= theme_table($header,$rows);
  } else {
    $content .= t('There is no filter. To add one click on "Create filter".');
  }
  
  return $content;
}

/**
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
    '#description'   => t('The tags who will be filtered. Enter one value per line, without the "&lt;", "&gt;".<br>
        You can also add a label, following schema "<i>tag|label</i>".'),
    '#default_value' => '',
    '#cols'          => 10,
    '#rows'          => 30,
    '#resizable'     => TRUE,
  );
  
  /*
  $form['wallypressfilters']['markers'] = array(
    '#type'          => 'textarea',
    '#title'         => t('List of the markers'),
    '#description'   => t('The markers who will be filtered. Enter one value per line, without the "&lt;", "&gt;".<br>
        You can also add a label, following schema "<i>marker|label</i>".'),
    '#default_value' => '',
    '#cols'          => 10,
    '#rows'          => 15,
    '#resizable'     => TRUE,
  );
  */

  if ($fid != NULL){
    $filter = wallypressfilters_getfilterbyid($fid);
    $form['wallypressfilters']['fid'] = array('#type' => 'value', '#value' => $filter->fid);
    $tags = wallypressfilters_gettagsfromfilter($filter->name);
    $form['wallypressfilters']['name']['#default_value'] = $filter->name;
    $form['wallypressfilters']['description']['#default_value'] = $filter->description;

    $tags_default_value = $markers_default_value = $attr_rel_tags_default_value = '';
    $tag_space = $marker_space = $attr_rel_tags_space = '';
    foreach ($tags as $tag) {
    //  switch ($tag->type) {
    //    case 'tag':
          $tags_default_value .= $tag_space.$tag->tag.'|'.$tag->description;
          $tag_space = "\n";
    //      break;
          
    /*    case 'marker':
          $markers_default_value .= $marker_space.$tag->tag.'|'.$tag->description;
          $marker_space = "\n";
          break;*/
    //  }
    }
    $form['wallypressfilters']['tags']['#default_value'] = $tags_default_value;
    //$form['wallypressfilters']['markers']['#default_value'] = $markers_default_value;
  }

  $form['sumbit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation of the wallypresfilters_form_addfilter_form
 */
function wallypressfilters_form_addfilter_form_validate($form, &$form_state){
  $form_state['values']['wallypressfilters']['name'] = strtolower(check_plain($form_state['values']['wallypressfilters']['name']));
  $form_state['values']['wallypressfilters']['description'] = check_plain($form_state['values']['wallypressfilters']['description']);
  
  // If it's edition
  if (isset($form_state['values']['wallypressfilters']['fid'])){
    $form_state['values']['update'] = array('fid');
  } else {
    // Check if the name is correct
    if (preg_match('/[^a-z0-9_]/', $form_state['values']['wallypressfilters']['name'])) {
      form_set_error('wallypressfilters][name', t('Filter name must be alphanumeric or underscores only.'));
    } 
    // Check if the name already exists
    elseif (wallypressfilters_getfilterbyname($form_state['values']['wallypressfilters']['name']) != NULL){
      form_set_error('wallypressfilters][name', t('A filter with this name already exists.'));
    }
    $form_state['values']['update'] = array();
  }
  
  $form_state['values']['wallypresstags'] = array();

  $weight = 0;
  $tags = explode("\r\n", $form_state['values']['wallypressfilters']['tags']);
  foreach ($tags as $tmp_tag) {
    list($tag, $description) = explode('|', $tmp_tag);

    // Check if the tag is correct
    if ($tag == '') {
      continue;
    }
    
    // Check is the tag exists two times
    if (array_key_exists($tag, $form_state['values']['wallypresstags'])) {
      form_set_error('wallypressfilters][tags', t('There can not be two identical tags.'));
    } else {
      $update = array();
      $form_state['values']['wallypresstags'][$tag] = array(
        'tag' => $tag,
        'description' => $description,
        'filter' => $form_state['values']['wallypressfilters']['name'],
        'type' => 'tag',
        'update' => $update,
        'weight' => $weight++,
      );
    }
  }
  /*
  $markers = explode("\r\n", $form_state['values']['wallypressfilters']['markers']);
  foreach ($markers as $tmp_marker) {
    list($marker, $description) = explode('|', $tmp_marker);
    
    // Check if the marker is correct
    if ($marker == '') {
      ;
    }
    // Check is the marker exists two times
    elseif (array_key_exists($marker, $form_state['values']['wallypresstags'])) {
      form_set_error('wallypressfilters][markers', t('There can not be two identical markers.'));
    } else {
      $update = array();
      $form_state['values']['wallypresstags'][$marker] = array(
        'tag' => $marker,
        'description' => $description ? $description : '',
        'filter' => $form_state['values']['wallypressfilters']['name'],
        'type' => 'marker',
        'update' => $update,
      );
    }
  }
  */
}

/**
 * Submit of the wallypresfilters_form_addfilter_form
 */
function wallypressfilters_form_addfilter_form_submit($form, &$form_state){
  // Save filter
  $result = drupal_write_record('wallypressfilters', $form_state['values']['wallypressfilters'], $form_state['values']['update']);
  $message = '';
  if ($result) {
    $message = t('The filter as been created/updated');
    
    foreach ($form_state['values']['wallypresstags'] as $tag){
      // Save tags
      if ($old_tags = wallypressfilters_gettagsbynameandfilter($tag['tag'], $form_state['values']['wallypressfilters']['name'])) {
        $update = array('tid');
        foreach ($old_tags as $old_tag) {
          $new_tag = array_merge((array)$old_tag, $tag);
          $result = drupal_write_record('wallypressfilters_tags', $new_tag, $update);
        }
        $action = 'updated';
      } else {
        $tag['settings'] = _wallypressfilters_settings_defaults();
        $result = drupal_write_record('wallypressfilters_tags', $tag);
        $action = 'created';
      }
      if ($result) {
        $message .= '<br>'.t('The !tag_type "@tag" has been !action', array('!tag_type' => $tag['type'], '@tag' => $tag['tag'], '!action' => $action));
      } else {
        $message .= '<br>'.t('The !tag_type "@tag" has not been !action', array('!tag_type' => $tag['type'], '@tag' => $tag['tag'], '!action' => $action));
      }
    }

    // Delete tags and markers
    foreach (wallypressfilters_gettagsfromfilter($form_state['values']['wallypressfilters']['name']) as $filter_tags) {
      if (!array_key_exists($filter_tags->tag, $form_state['values']['wallypresstags'])) {
        db_query("DELETE FROM {wallypressfilters_tags} WHERE filter = '%s' AND type = '%s' AND tag = '%s'", $filter_tags->filter, $filter_tags->type, $filter_tags->tag);
        $message .= '<br>'.t('The !tag_type "@tag" has been deleted', array('!tag_type' => $filter_tags->type, '@tag' => $filter_tags->tag));
      }
    }
  } else {
    $message = t('The filter as not been created/udpated');
  }
  drupal_set_message(t($message));
  drupal_goto('admin/wally/wallypressfilters/settings/general');
}

/**
 * Form to delete a filter
 */
function wallypressfilters_form_deletefilter_form($form_state, $fid){
  $filter = wallypressfilters_getfilterbyid($fid);
  $question = t('Are you sure you want to delete "@filter_name" filter?', array('@filter_name' => $filter->name));
  
  $add_form = array();
  $add_form['fid'] = array(
    '#type' => 'value',
    '#value' => $fid,
  );
  
  $form = confirm_form($add_form, $question, 'admin/wally/wallypressfilters/settings');
  $form['#redirect'] = 'admin/wally/wallypressfilters/settings';
  return $form;
}

/**
 * Submit of the wallypressfilters_form_deletefilter_form
 */
function wallypressfilters_form_deletefilter_form_submit($form, &$form_state){
  $filter = wallypressfilters_getfilterbyid($form_state['values']['fid']);
  $result = db_query("DELETE FROM {wallypressfilters} WHERE fid = %d", $filter->fid);
  $result2 = db_query("DELETE FROM {wallypressfilters_tags} WHERE filter = '%s'", $filter->name);
  if ($result & $result2){
    drupal_set_message(t('The filter as correctly been deleted'));
  } else {
    drupal_set_message(t('The filter couldn\'t be correctly deleted'));
  }
}
