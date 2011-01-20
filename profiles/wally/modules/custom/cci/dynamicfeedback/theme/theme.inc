<?php
function theme_dynamicfeedback_page_render($array_of_informations){
  
  $content='<div>'.$array_of_informations['title'].'</div>';
  $content.='<div>'.$array_of_informations['comment_count'].'</div>';
  return $content;  

}