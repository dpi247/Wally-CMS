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


function theme_ccimapping_hierarchies($hierarchies){
$output='<?xml version="1.0" encoding="UTF-8" standalone="no"?>'."\n";
$output.='<exportproductinformation newsroom = "CciNewsRoom" publicationchannel = "sudpresse.be" subtype = "Online">'."\n";
$output.=' <pubdatestring name = "%d-%m-%Y"/>'."\n";
$output.='  <product>Wally News</product>'."\n";
$output.='  <baseinfo>'."\n";
$output.='    <leafname name="Target"/>'."\n";
$output.='    <mainpriorities>'."\n";

$rank_limit = variable_get('cckdestinations_rank_limit', 10);
$count=0;
  while($count<$rank_limit){
    $count++;
    $output.='      <mainpriority name = "Rank '.$count.'" value = "'.$count.'"/>'."\n";
  }
  
$output.='    </mainpriorities>'."\n";
$output.='  </baseinfo>'."\n";
$output.='  <budgettemplate>'."\n";
foreach ($hierarchies as $key=>$hierarchie){
  $output.='    <level1 name="'.$hierarchie['term_name'].'">'."\n";
  foreach ($hierarchie['redacblocks'] as $redack_machine_name=>$redacblock){
    $output.='      <leaf name="'.$redack_machine_name.'"/>'."\n";
  }
  $output.='    </level1>'."\n";
}

$output.='  </budgettemplate>'."\n";
$output.='</exportproductinformation>'."\n";
return $output;
}