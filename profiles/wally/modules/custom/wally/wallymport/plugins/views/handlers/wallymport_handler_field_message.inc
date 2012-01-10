<?php
class wallymport_handler_field_message extends views_handler_field_numeric {
  
  
  function render($values) {
    $logs = unserialize($values->{$this->field_alias});
    
    return $logs['error_msg'];
  }
}