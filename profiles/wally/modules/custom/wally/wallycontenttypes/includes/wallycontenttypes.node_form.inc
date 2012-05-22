<?php 

function wallycontenttypes_ajax_editcrop($nid_type, $nid, $js = NULL) {
  if ($js) {
    wydit_ctools_include();
  } else {
    ctools_include('form');
  }

  $form_state = array(
    'ajax' => $js,
  );

  if ($nid_type == 'prenode') {
    module_load_include('inc', 'wallyedit', 'includes/Prenode');
    if ($prenode = prenode_get($nid, TRUE)) {
      $args = func_get_args();
      array_shift($args);
      array_shift($args);
      array_shift($args);
      if (!empty($args)) {
        $parents = $args;
        $node = $prenode;
        foreach ($parents as $parent) {
          $node = $node[$parent];
        }
        $node = $node['#node'];
        $form_state['parents'] = $parents;
      } else {
        $node = $prenode['wally_photoobject']['#node'];
      }
    }
  } else {
    $node = node_load($nid);
    $prenode = $node;
  }

  $imgcropings = array();
  $imgcropings = json_decode($node->field_croping[0]['value'], TRUE);
  $explimgcrops = array();
  $defaultvalues = array();

  if ($imgcropings && !empty($imgcropings) && $imgcropings!="") {
    foreach ($imgcropings as $presetname => $imgcroping) {
      $tempexplcrop = array();
      $tempexplcrop = $imgcroping;
      $tempexplcrop[0] = $tempexplcrop[0];
      $tempexplcrop[1] = $tempexplcrop[1];
      $tempexplcrop[2] = $tempexplcrop[2];
      $tempexplcrop[3] = $tempexplcrop[3];
      $tempexplcrop[4] = round($tempexplcrop[2]/$tempexplcrop[3],2);
      $explimgcrops[$presetname] = $tempexplcrop;
      $last_preset = $presetname;
    }
  } else {
    if (!empty($imgcropings)) drupal_set_message(t('Bad JSON encoding.'),"error");
  }
  $photofile = $node->field_photofile;
  $filepath = $photofile[0]['filepath'];
  list($defaultvalues, $usablecrops) = _wallycontenttypes_getusablecropsanddefaults($explimgcrops, $filepath);

  $form_state['nid'] = $nid;
  $form_state['prenode'] = $prenode;
  $form_state['nid_type'] = $nid_type;
  $form_state['filepath'] = $filepath;
  $form_state['usablecrops'] = $usablecrops;
  $form_state['defaultvalues'] = $defaultvalues;

  $output = ctools_build_form('wallycontenttypes_form_edit_crop_form', $form_state);

  // If javascript is active, we have to use a render array.
  $commands = array();
  if ($output === FALSE || !empty($form_state['complete'])) {
    drupal_get_messages();
    // Dismiss the modal.
    $commands[] = ctools_modal_command_dismiss();
  }
  else if (!empty($form_state['cancel'])) {
    drupal_get_messages();
    // If cancelling, return to the activity.
    $commands[] = ctools_modal_command_dismiss();
  }
  else {
    $module_path = drupal_get_path('module', 'wallycontenttypes');
    drupal_add_js($module_path.'/scripts/editcrop/jquery.Jcrop.js');
    drupal_add_css($module_path.'/css/editcrop/jquery.Jcrop.css');
    drupal_add_css($module_path.'/css/editcrop/editcrop.css');
    drupal_add_js($module_path.'/scripts/editcrop/edit_crop.js');
    drupal_add_js(array('js_usablecrops' => $usablecrops), 'setting');

    $commands = ctools_modal_form_render($form_state, $output);
  }

  ctools_ajax_render($commands);
}

/**
 * Edition of the cropings of a photo.
 * 
 * @param string $nid
 *  ID of the node containing the photo being edited.
 * 
 * @see http://deepliquid.com/blog/archives/category/jcrop
 *  Jcrop homepage.
 * 
 * @return
 *  HTML code of the image and the form containing the new values.
 */
function wallycontenttypes_form_edit_crop_form(&$form_state) {
  if (empty($form_state['input'])) {
    $form_state['complete'] = TRUE;
  }
  $defaultvalues = $form_state['defaultvalues'];

  $form = array(
    '#attributes' => array('class' => 'crops'),
    '#prefix' => '<div class="crop_wrap">',
    '#suffix' => '</div>',
  );

  $first = TRUE;
  foreach ($defaultvalues as $presetname => $defaultvalue) {
    $form[$presetname.'_serialCoord'] = array(
      '#type' => 'hidden',
      '#id' => $presetname.'_serialCoord',
      '#title' => $presetname.'_serialCoord',
      '#default_value' => implode($defaultvalue,','),
    );

    $form[$presetname] = array(
      '#type' => 'submit',
      '#id' => $presetname,
      '#title' => $presetname,
      '#value' => $presetname,
    );

    if ($first) {
      $form[$presetname.'_serialCoord']['#prefix'] = '
        <label for="edit-crop">'.t('Move and resize crops').' : <span id="crop-current-name">'.t('Choose a preset to edit.').'</span>
        </label>
        <div id="cropimage"><img src="'.base_path().$form_state['filepath'].'" id="cropbox" /></div>
        <div class="crops">';
      $first = FALSE;
    }
    if (count(element_children($form)) == 2*(count($defaultvalues))) {
      $form[$presetname]['#suffix'] = '</div>';
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => t('Save'),
    '#value' => t('Save'),
    '#prefix' => '<div class="crop_submit buttons">',
    '#suffix' => '</div>',
  );

  return $form;
}

function wallycontenttypes_form_edit_crop_form_submit (&$form, &$form_state) {
  $prenode = $form_state['prenode'];
  $usablecrops = $form_state['usablecrops'];
  $values = $form_state['values'];

  if ($form_state['nid_type'] == 'prenode') {
    if (isset($form_state['parents'])) {
      $parents = $form_state['parents'];
      $node = &$prenode;
      foreach ($parents as $parent) {
        $node = &$node[$parent];
      }
      $node = &$node['#node'];
    } else {
      $node = &$prenode['wally_photoobject']['#node'];
    }
  } else {
    $node = &$prenode;
  }

  $crops = array();
  $modif = FALSE;
  foreach ($usablecrops as $presetname => $usablecrop) {
    $temp_crops = explode(',',$values[$presetname.'_serialCoord']);
    if (isset($temp_crops[4]) && $temp_crops[4] == '1') {
      // Le crop a été modifié pour ce preset
      $modif = TRUE;
    }
    $crops[$presetname] = $temp_crops;
    $crops[$presetname]['presetid'] = $usablecrop['presetid'];
  }
  
  if ($modif) {
    imagecache_image_flush($node->field_photofile[0]['filepath']);
    // Also flush varnish cache
    if (module_exists('warnish')) {
      $imagecache_paths = array();
      foreach (imagecache_presets() as $preset) {
        $imagecache_paths[] = imagecache_create_path($preset['presetname'], $node->field_photofile[0]['filepath']);
      }
      warnish_purge_page($imagecache_paths);
    }
  }

  $node->field_croping[0]['value'] = drupal_to_js($crops);
  if ($form_state['nid_type'] == 'prenode') {
    module_load_include('inc', 'wallyedit', 'wallyedit.forms');
    wydit_save_form_state($prenode);
  } else {
    $former_changed = $node->changed;
    cckdestinations_force_dnc($node);
    node_save($node);
    db_query("UPDATE {node} set changed = %d WHERE nid = %d", $former_changed, $node->nid);
  }
}

/**
 * Recursively set the disabled attribute of a CCK field
 * and all its dependent FAPI elements. For a content_taxonomy_tree
 * cck field, we must disbaled all the childrens: checkboxes or radios.
 * 
 * @param $element
 *   The form element to proceed.
 * 
 * @return
 *   Nothing
 */
function _wallycontenttypes_mysnippetfixdisabled(&$elements) {
  if ($elements["#type"]!="content_taxonomy_tree") {
    foreach (element_children($elements) as $key) {
      if (isset($elements[$key]) && $elements[$key]) {
        _wallycontenttypes_mysnippetfixdisabled($elements[$key]);
      }
    }
    if (!isset($elements['#attributes'])) {
      $elements['#attributes'] = array();
    }
    $elements['#attributes']['disabled'] = 'disabled';
  } else {
    foreach (element_children($elements['value']['#elements']) as $key) {
      _wallycontenttypes_mysnippetdisabledtaxonomytree($elements['value']['#elements'][$key]);
    }
  }
}

/**
 * Recursivly search for each children element of a  content_taxonomy_tree
 * field. To disable all childrens. 
 * 
 * @param $element 
 *   Form element to proceed. Must be a content_taxonomy_tree element.
 *
 * @return
 *   Nothing
 */
function _wallycontenttypes_mysnippetdisabledtaxonomytree(&$elements) {
  foreach (element_children($elements) as $k) {
    $elements[$k]['#attributes']['disabled'] = 'disabled';
    if ($k=='children') {
      foreach (element_children($elements[$k]) as $key) {
        _wallycontenttypes_mysnippetdisabledtaxonomytree($elements[$k][$key]);
      }
    }
  }
}

/**
 * Update form_state values with the correct croping values
 */
function _wallycontenttypes_update_crop_state_value($form, &$form_state) {
  $values = &$form_state['values'];

  // @todo Trouver un moyen de vérifier qu'on est dans le process de submit
  // pour éviter de faire un node load à la construction du form
  if (isset($form_state['cache name'])) {
    module_load_include('inc', 'wallyedit', 'includes/Prenode');
    if ($prenode = prenode_get($form_state['cache name'])) {
      $photo_prenode = wydit_get_target_from_parents($prenode, $form_state['parents']);
      wydit_add_elem_at_target($values, $photo_prenode['#node']->field_croping[0]['value'], array_merge($form_state['parents'], array('field_croping', 0, 'value')));
    }
  } else {
    if ($node = node_load($values['nid'])) {
      if ($form['#tree']) {
        $crop_value = &$values['wally_photoobject']['field_croping'][0]['value'];
      } else {
        $crop_value = &$values['field_croping'][0]['value'];
      }
      $crop_value = $node->field_croping[0]['value'];
    }
  }
}

/**
 * Return the default values of a croping, needed by wallycontenttypes_editcrop.
 * 
 * @param $crop
 *   The croping informations to save.
 * 
 * @return
 *   The default values for this croping.
 */
function _wallycontenttypes_getdefaultvaluesforthecroping($crop, $img) {
  $cropdefault = array();
  for ($i=0; $i<4; $i++) {
    $cropdefault[$i] = $crop[$i];
  }
  return $cropdefault;
}

/**
 * Return the crops in a usable form for Jcrop, needed by wallycontenttypes_editcrop.
 * 
 * @param array $explimgcrops
 *   The croping informations extracted from the node.
 * @param string $photo_path
 *   Drupal path of the photo being edited.
 * 
 * @return
 *   The usables crops for Jcrop.
 */
function _wallycontenttypes_getusablecropsanddefaults($explimgcrops, $photo_path) {
  $usablecrops = array();
  $presets = imagecache_presets();
  $img = imageapi_image_open($photo_path);

  foreach ($presets as $preset) {
    if ($preset['actions']) {
      $datas = array();
      foreach ($preset['actions'] as $action) {
        if ($action['action']=='wallycontenttypes_wallycrop') {
          if (!isset($action['data']['presetid']) || empty($action['data']['presetid'])) {
            $action['data']['presetid'] = $preset['presetid'];
            imagecache_action_save($action);
          }
          $datas = $action['data'];
        }
      }
      if (!empty($datas)) {
        $temppresetcrop = array();
        
        $ratio = round($datas['width']/$datas['height'],2);
        
        if (!is_numeric($datas['xoffset'])) {
        	if ($datas['xoffset'] == 'center_max') {
        	  if ($img->info['width'] / $ratio <= $img->info['height']) {
        	    $datas['width'] = $img->info['width'];
        	    $datas['xoffset'] = 0;
        	  } else {
        	    $datas['width'] = $img->info['height'] * $ratio;
        	    $datas['xoffset'] = 'center';
        	    $datas['xoffset'] =  _imagecache_keyword_filter($datas['xoffset'], $img->info['width'], $datas['width']);
        	  }
        	} else {
        	  $datas['xoffset'] =  _imagecache_keyword_filter($datas['xoffset'], $img->info['width'], $datas['width']);
        	}
	      }
	      $temppresetcrop[0] = $datas['xoffset'];
	      
	      if (!is_numeric($datas['yoffset'])) {
        	if ($datas['yoffset'] == 'center_max') {
        	  if ($img->info['height'] * $ratio <= $img->info['width']) {
        	    $datas['height'] = $img->info['height'];
        	    $datas['yoffset'] = 0;
        	  } else {
        	    $datas['height'] = $img->info['width'] / $ratio;
        	    $datas['yoffset'] = 'center';
        	    $datas['yoffset'] =  _imagecache_keyword_filter($datas['yoffset'], $img->info['height'], $datas['height']);
        	  }
        	} else {
        	  $datas['yoffset'] =  _imagecache_keyword_filter($datas['yoffset'], $img->info['height'], $datas['height']);
        	}
	      }
	      $temppresetcrop[1] = $datas['yoffset'];

        $temppresetcrop[2] = $datas['width'];
        $temppresetcrop[3] = $datas['height'];
        $temppresetcrop[4] = $ratio;
        
        $tempusablecrop = array();

        if ($explimgcrops && !empty($explimgcrops) && $explimgcrops!="") {
          foreach ($explimgcrops as $presetname => $explimgcrop) {
            if ($presetname == $preset['presetname'] && abs($temppresetcrop[4]-$explimgcrop[4])/$temppresetcrop[4]<0.1) {
              $defaultvalues[$presetname] = _wallycontenttypes_getdefaultvaluesforthecroping($explimgcrop, $img);
              $tempusablecrop = $explimgcrop;
              $tempusablecrop[2] += $tempusablecrop[0];
              $tempusablecrop[3] += $tempusablecrop[1];
              $tempusablecrop['presetid'] = $preset['presetid'];
              break;
            }
          }
        }

        if (empty($tempusablecrop)) {
          $defaultvalues[$preset['presetname']] = _wallycontenttypes_getdefaultvaluesforthecroping($temppresetcrop, $img);
          $tempusablecrop = $temppresetcrop;
          $tempusablecrop[2] += $tempusablecrop[0];
          $tempusablecrop[3] += $tempusablecrop[1];
          $tempusablecrop['presetid'] = $preset['presetid'];
          drupal_set_message('No croping info found for the wallycrop action of the '.$preset['presetname'].' preset, default values loaded.',"notice");
        }
        $usablecrops[$preset['presetname']] = $tempusablecrop;
      } else {
        //drupal_set_message('The preset '.$preset['presetname'].' doesn\'t use the wallycrop action.',"notice");
      }
    }
  }

  return array($defaultvalues,$usablecrops);
}

/**
 * Check if the node need to be publish or unpublish
 * @param $node
 * The node
 * @return Nothing.
 */
function _wallycontenttypes_checkautopublish(&$node){
  if (db_table_exists('wallycontenttypes_temp_nodetopublish')){
    if ($node->field_autopublish[0]['value'] == 'Autopublish'){
      if (strtotime($node->field_embargodatetime[0]['value']) <= time() && ($node->field_unpublishdate[0]['value'] == NULL || strtotime($node->field_unpublishdate[0]['value']) >= time())){
        $node->status = 1;
      } else {
        $node->status = 0;
      }
    } else {
      $node->status = 0;
    }
  }
}

/**
 * Check if the node need to be publish or unpublish
 * @param $node
 * The node
 * @return Nothing.
 */
function _wallycontenttypes_insertautopublish($node) {
  if (db_table_exists('wallycontenttypes_temp_nodetopublish')) {
    db_query("DELETE FROM {wallycontenttypes_temp_nodetopublish} WHERE nid = %d", $node->nid);
    if ($node->field_autopublish[0]['value'] == 'Autopublish') {
      $node_temp = new stdClass();
      $node_temp->nid = $node->nid;
      $node_temp->status = $node->status;
      $node_temp->field_embargodatetime_value = $node->field_embargodatetime[0]['value'];
      $node_temp->field_unpublishdate_value = $node->field_unpublishdate[0]['value'];
      drupal_write_record('wallycontenttypes_temp_nodetopublish', $node_temp);
    }
  }
}

function _wallycontenttypes_deletefromtempautopublish($node) {
  db_query("DELETE FROM {wallycontenttypes_temp_nodetopublish} WHERE nid = %d", $node->nid);
}

/**
 * Add taxonomy term from textobject to articlepackage when textobject is updated
 * 
 * Packagenode is the node who reference the current node (It can be ArticlePackage, GalleryPackage or PollPackage
 * Because we dont want the package node to update its taxonomies fwhen we saved it from
 * here, we set a speciel field: "no_taxonomy_update" to TRUE, see _wallycontenttypes_addtaxonomywheninserted(); 
 * 
 * @param $node
 *   Node currently processed(TextObject, AudioObject, DigitalObject,PhotoObject, Videoobject, PollObject)
 * 
 * @see _wallycontenttypes_addtaxonomywheninserted()
 * 
 * @return
 *   Nothing
 */
function _wallycontenttypes_addtaxonomywhenupdated($node){
  $rows =_wallycontenttypes_getpackagesid($node);
  foreach($rows as $row){
    $packagenode=node_load($row['nid']);
    // Because we dont want node-save api to update
    // taxonomies from main object. We set this
    // field to true. 
    $packagenode->no_taxonomy_update = TRUE; 
    $packagenode->taxonomy = $node->taxonomy;
    $packagenode->field_markers = $node->field_markers;
    $packagenode->field_locations = $node->field_locations;
    $packagenode->field_entities = $node->field_entities;
    $packagenode->field_persons = $node->field_persons;
    $packagenode->field_free_tags = $node->field_free_tags;
    $packagenode->field_concepts = $node->field_concepts;
    $packagenode->field_iptc = $node->field_iptc;
    cckdestinations_force_dnc($node);
    $r = node_save($packagenode);
  }
}

/**
 * Add taxonomy term of textobject to articlepackage when articlepackage is inserted or updated
 * 
 * objectnode is the node who is referenced by the current node
 * (It can be TextObject, AudioObject, DigitalObject,PhotoObject, Videoobject or PollObject)
 * 
 * Notice: When a mainobject is updated (we also save package node) we set the
 * "no_taxonomy_update field" to TRUE, so we dont want to update taxonomies from
 * the main object node. See _wallycontenttypes_addtaxonomywhenupdated();
 * 
 * @param $node
 *   Node currently processed(ArticlePackage, GalleryPackage or PollPackage)
 *
 * @see _wallycontenttypes_addtaxonomywhenupdated
 * 
 * @return
 *   Nothing
*/
 function _wallycontenttypes_addtaxonomywheninserted(&$node, $revision = NULL, $reset = NULL){
// When a mainobject is updated we set the
// no_taxonomy_update field to TRUE
// so we dont want to update taxonomies from
// the main object node. 

  if (!$node->no_taxonomy_update) { 
    switch ($node->type){
      case 'wally_articlepackage':
        $objectnode = node_load($node->field_mainstory[0]['nid'], $revision, $reset);
      break;
      case 'wally_gallerypackage':
        $objectnode = node_load($node->field_mainobject[0]['nid'], $revision, $reset);
      break;
      case 'wally_pollpackage':
        $objectnode = node_load($node->field_mainpoll[0]['nid'], $revision, $reset);
      break;
    }
    if ($objectnode){
      $node->taxonomy = $objectnode->taxonomy;
      $node->field_markers = $objectnode->field_markers;
      $node->field_locations = $objectnode->field_locations;
      $node->field_entities = $objectnode->field_entities;
      $node->field_persons = $objectnode->field_persons;
      $node->field_free_tags = $objectnode->field_free_tags;
      $node->field_concepts = $objectnode->field_concepts;
      $node->field_iptc = $objectnode->field_iptc;
    }
  }
}

/**
 * Update a prenode (package) taxonomies with his objects
 */
function _wallycontenttypes_update_package_prenode_taxonomy_recursive(&$prenode, $current_profile) {
  $pack_type = $prenode['type'];
  module_load_include('inc', 'wallyedit', 'includes/config_api');
  $fields = wyditadmin_get_existing_fields($pack_type);
  
  switch ($pack_type){
    case 'wally_articlepackage':
      $main_object_type = 'field_mainstory';
      break;
    case 'wally_gallerypackage':
      $main_object_type = 'field_mainobject';
      break;
    case 'wally_pollpackage':
      $main_object_type = 'field_mainpoll';
      break;
  }

  $main_object = isset($prenode[$main_object_type][0]['#node']) ? $prenode[$main_object_type][0]['#node'] : FALSE;
  $prenode_node = &$prenode['#node'];

  if ($main_object){
    _wallycontenttypes_set_taxonomies_from_object_to_prenode($prenode_node, $main_object);
  }

  $pack_types = array('wally_articlepackage', 'wally_gallerypackage', 'wally_pollpackage');
  foreach ($fields as $field) {
    if (isset($field['referenceable_types']) && is_array($field['referenceable_types']) && array_intersect($pack_types, $field['referenceable_types']) && isset($prenode[$field['name']]) && is_array($prenode[$field['name']])) {
      foreach ($prenode[$field['name']] as &$sub_prenode) {
        if (in_array($sub_prenode['type'], $pack_types)) {
          _wallycontenttypes_update_package_prenode_taxonomy_recursive($sub_prenode, $current_profile);
        }
      }
    }
  }
}

function _wallycontenttypes_set_taxonomies_from_object_to_prenode(&$prenode_node, $object) {
  $prenode_node->taxonomy = isset($prenode_node->taxonomy) ? array_merge_recursive($prenode_node->taxonomy, $object->taxonomy) : $object->taxonomy;
  $prenode_node->field_markers = isset($object->field_markers) ? $object->field_markers : $prenode_node->field_markers;
  $prenode_node->field_locations = isset($object->field_locations) ? $object->field_locations : $prenode_node->field_locations;
  $prenode_node->field_entities = isset($prenode_node->field_entities) ? array_merge_recursive($prenode_node->field_entities, $object->field_entities) : $object->field_entities;
  $prenode_node->field_persons = isset($object->field_persons) ? $object->field_persons : $prenode_node->field_persons;
  $prenode_node->field_free_tags = isset($object->field_free_tags) ? $object->field_free_tags : $prenode_node->field_free_tags;
  $prenode_node->field_concepts = isset($object->field_concepts) ? $object->field_concepts : $prenode_node->field_concepts;
  $prenode_node->field_iptc = isset($object->field_iptc) ? $object->field_iptc : $prenode_node->field_iptc;
}
