<?php
class wallymport_handler_field_filename extends views_handler_field_numeric {
  
  
  function render($values) {
    
    $path = explode('/', $values->{$this->field_alias});
    $filename = array_pop($path);
    return $filename;
  }
}