<?php
class wallymport_handler_field_success extends views_handler_field_numeric {
  
  
  function render($values) {
    $logs = unserialize($values->{$this->field_alias});
    $success = 'TRUE';
    if ($logs['error']){
      $success = 'FALSE';
    }
    return $success;
  }
}