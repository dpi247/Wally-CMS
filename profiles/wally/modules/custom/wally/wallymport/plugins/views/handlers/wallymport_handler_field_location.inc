<?php
class wallymport_handler_field_location extends views_handler_field_numeric {
  
  
  function render($values) {
    
    $path = explode('/', $values->{$this->field_alias});
    $filename = array_pop($path);
    $file_path = str_replace($filename, '', $values->{$this->field_alias});
    return $file_path;
  }
}