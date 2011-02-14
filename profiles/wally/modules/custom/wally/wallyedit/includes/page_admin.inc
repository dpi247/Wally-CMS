<?php

function wallyedit_page_admin(){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  $result=db_query("SELECT * from {ctools_object_cache} where obj='%s'","prenode");
  
  while($obj=db_fetch_object($result)){
    dsm($obj);
    $row['sid']=$obj->sid;
    $row['name']=$obj->name;
    $row['data']=unserialize($obj->data);
    $row['edit_action_url']=wydit_get_url_action('edit',$obj->name);
    
    $rows[]=$row;
  }
  $headers=array('sid','nid','title','action(s)');
  return theme('wallyedit_admin',$rows,$headers);
}
