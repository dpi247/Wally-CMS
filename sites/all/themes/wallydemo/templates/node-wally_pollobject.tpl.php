<?php 
module_load_include('inc', 'webform', 'includes/webform.submissions');
print $node->field_body[0]['value'];
if (!_webform_submission_user_limit_check($node)){
  print $node->content['webform']['#value'];
} else {
  print wallydemo_displaypollresult($node);
}
/*
if (isset($node->content['webform']['#value'])){
  print $node->content['webform']['#value'];
} else {
  print wallydemo_displaypollresult($node);
}*/
?>