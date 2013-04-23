<?php
class wallymport_handler_field_message extends views_handler_field_numeric {
  
  
  function render($values) {
    
    $logs = unserialize($values->{$this->field_alias});
    $extref = '';
    if (isset ($logs['log']['extref'])){
      $extref = ' External Rerefence : '.$logs['log']['extref'];
    }
    return $logs['error_msg'].$extref;
  }
}