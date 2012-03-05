<?php 
if (isset($node->content['webform']['#value'])){
  print $node->content['webform']['#value'];
} else {
  print wallydemo_displaypollresult($node);
}
?>