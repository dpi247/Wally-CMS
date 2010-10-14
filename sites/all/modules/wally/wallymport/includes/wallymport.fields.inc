<?php 

function wimprt_process_package_articlepackagetype(&$node,$dom_element,&$log){
  
  //Map XML Attributes 
  $node->type='wally_articlepackage';
  $node->uid=1;
  $node->created_at= strtotime($dom_element->getAttributeNode("CreationDate")->value);
  $node->language= $dom_element->getAttributeNode("Language")->value;
  $node->created= $dom_element->getAttributeNode("PackageID")->value;
  $node->created_at= $dom_element->getAttributeNode("PublicationDate")->value;
  $node->created_at= $dom_element->getAttributeNode("LastUpdateDate")->value;
  $node->created_at= $dom_element->getAttributeNode("EmbargoDate")->value;
  
  //Map XML element
  wimprt_set_field_mainstory($node,$dom_element->getElementsByTagName("MainStory")->item(0),$log);
}

//***************************




function wimprt_set_field_mainstory(&$node,$dom,&$log){
  $object=wimprt_process_object($dom,$log,'TextObject');
  $node->MainStory['nid'][0]=$object->nid;
}

function wimprt_process_object_textobject (&$node,$dom_element,&$log){
  //Map XML Attributes 
  $node->type='wally_textobject';
  $node->uid=1;
  $node->created= $dom_element->getAttributeNode("DocumentType")->value;
  $node->created_at= $dom_element->getAttributeNode("ObjectID")->value;
  $node->created_at= $dom_element->getAttributeNode("ObjectDescription")->value;
  $node->created_at= $dom_element->getAttributeNode("PublicationDate")->value;
  $node->created_at= $dom_element->getAttributeNode("LastUpdateDate")->value;
  
  //Map Element 
  // wimprt_process_object_default will call wimprt_process_object_locations,...
  wimprt_process_object_default(&$node,$dom_element,&$log);
}


function wimprt_set_field_locations(&$node,$dom,&$log){
  
  $result=wimprt_process_list(&$node,$dom,'type',$log);
  $name= $dom->nodeName;
  $node->{$name}[0]['value']=$dom->nodeValue;
}