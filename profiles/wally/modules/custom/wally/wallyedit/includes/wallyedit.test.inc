<?php 

function wallyedit_test(){

 return drupal_get_form('wallyedit_test_form');

}



function wallyedit_test_form($form_state){
  $type_name='wally_articlepackage';
  
  $groups=fieldgroup_groups($type_name) ;
      $type=content_types(str_replace('-','_', $type_name));
  
  $fields=$type['fields'];
 
  
  //remove fields already included in a groups
  foreach($groups as $group){
    foreach($group["fields"] as $key=>$field){
      unset($fields[$key]);
    }
  }
  
  
  //SIMULATE FAKE ONGLETS STRUCTURE:
  $onglets=array(
    'main'=>array(
      "label"=>"MAin",
      "weight"=>2,
      "elements"=>$fields+array('group_destinations'=>$groups['group_destinations']),
    ),
    'meta'=>array(
      "label"=>"Meta",
      "weight"=>1,
      "elements"=>array('group_identification'=>$groups['group_identification']),
    ),
    'onglet1'=>array(
      "label"=>"MAin",
      "weight"=>1,
      "elements"=>array('group_content'=>$groups['group_content']),
    ),
    'onglet2'=>array(
      "label"=>"Divers",
      "weight"=>3,
      "elements"=>array('group_taxonomy'=>$groups['group_taxonomy'],'group_authoring'=>$groups['group_authoring']),
    ),
  );
  
  $form=array();
  foreach($onglets as $index_onglet=>$onglet){
    
    $rows[]=array(NULL, $onglet['label'],NULL,'edit');
    foreach($onglet["elements"] as $index_element=>$element){
      /*
      $form['rows'][$index_onglet]['items'][$index_element]=array(
        '#type' => 'markup',
        '#value' => 'some Value about: '.$element['field_name'].$element['group_name']."\n<br/>"
      );
      */
      $form['rows'][$index_onglet]['items'][$index_element]['weight']=array(
            "#type"=>'weight',
			'#default_value'=>$weight,
            '#title' => 'some Value about: '.$element['field_name'].$element['group_name']."\n<br/>",
            "#attributes"=>array('class'=>"my-elements-weight-lev1 my-elements-weight-lev1-". $index_onglet),
      );
      if(isset($element['group_name'])){
        //Gentlemans, We have a band here
        foreach($element["fields"] as $index_field=>$field){
          $form['rows'][$index_onglet]['items'][$index_element]['items'][$index_field]['weight']=array(
            "#type"=>'weight',
			'#default_value'=>$weight,
            "#attributes"=>array('class'=>"my-elements-weight-lev2 my-elements-weight--lev2-". $region),
          );
        }
      }
    }
  }
  
  return $form;
}

function theme_wallyedit_test_form($form){
  dsm($form);
 
  foreach($form['rows'] as $index_onglet=>$onglet ){
    $this_row=array();
    $this_row[]='value';
    $this_row[] = drupal_render($form['rows'][$id]['weight-'.$sid]);
    $table_rows[] = array('data'=>$this_row, 'class'=>'draggable');

    drupal_add_tabledrag('my-module-table', 'order', 'sibling', "my-elements-weight-lev1","my-elements-weight-lev1-". $index_onglet);

  }
  $header=array('','');
  $output = theme('table',$header,$table_rows,array('id'=>'my-module-table'));
 
  return $output;
}