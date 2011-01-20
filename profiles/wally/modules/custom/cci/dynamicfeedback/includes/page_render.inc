<?php
function dynamicfeedback_page_render($package_id){

  $array_of_nodes=wallytoolbox_get_node_by_cckfield(array('field_packageid'=>$package_id));
  
  if(count($array_of_nodes)!=1){
   
    print '';
  }
  else{
    $node=$array_of_nodes[0];
  }
  
  $full_node=node_load($node->nid);
  
  $comment_count=$full_node->comment;
  $title=$full_node->title;
   
  $array=array('title'=>$title,'comment_count'=>$comment_count);
  
  print theme('dynamicfeedback_page_render',$array);
}