<?php 
print $node->field_body[0]['value'];

if (isset($node->content['webform']['#value'])){
  print $node->content['webform']['#value'];
} else {
  print wallydemo_displaypollresult($node);
}
?>