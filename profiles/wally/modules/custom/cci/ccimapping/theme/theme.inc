<?php

function theme_ccimapping_typeselector($type){
  $content=t('Choose a content type:');
  $content.='<ul>';
  foreach(content_types() as $content_type){
    $content.='<li>'.l($content_type['name'],'admin/cci/ccimapping/'.$type.'/editor/'.$content_type['type']).'</li>';
  }
  $content.='</ul>';
  return $content;
}
