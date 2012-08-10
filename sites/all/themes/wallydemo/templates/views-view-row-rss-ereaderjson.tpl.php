<?php
// $Id: views-view-row-rss.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss.tpl.php
 * Default view template to display a item in an Flow feed.
 *
 * @ingroup views_templates
 */

print_r($row_index);

//var_dump($node);
$row_view_index=$variables["view"]->row_index;
$view_offset = $variables["view"]->pager["offset"];
$row_index=$row_view_index+$view_offset;

$vars = array('node' => &$node);

$field_mainstory_nodes = $node->field_mainstory_nodes[0];
wallydemo_preprocess_node_build_embedded_links($vars);
//custom_ereader_preprocess_node_build_embedded_videos(&$node);


$json = array(); 
$json['nid']=$node->nid;
$json['comment']=$node->comment;
$json['title']= $field_mainstory_nodes->title;
$json['pubDate']=mktime($node->field_publicationdate[0]['value']);
$json['updateDate']=intval($node->changed);
$json['type']=$node->type;
$json['chapo']=$field_mainstory_nodes->field_textchapo[0]['value'];
$json['body']=$field_mainstory_nodes->field_textbody[0]['value'];
$json['foretitle']=$field_mainstory_nodes->field_textforetitle[0]['value'];
$json['subTitle']=$field_mainstory_nodes->field_textsubtitle[0]['value'];
$json['barette']=$field_mainstory_nodes->field_textbarette[0]['value'];
$json['byline']=$field_mainstory_nodes->field_byline[0]['value'];
$json['copyright']=$field_mainstory_nodes->field_copyright[0]['value'];
$aliases = wallytoolbox_get_all_aliases('node/'.$node->nid);
$json['url']= $aliases[0];
$json['mainDestination']=$node->field_destinations[0]["tid"];
$json['relatedObjects'] = array();
foreach($node->field_embededobjects_nodes as $one){
		switch($one->type){
    	case "wally_photoobject":
				$embed_one = wallydemo_get_photo_infos_and_display($one);
				$photoobject= array(
				//$json['relatedObjects'][] = array(
				'nid'=> $embed_one["nid"],
				'type'=> 'wally_photoobject',
				'caption'=> $embed_one["caption"],
				'credit'=> $embed_one["credit"]	);	
				$photoobject['crop']['medium']= array(					 				 				 				
					'height'=>'300',
					'width'=>'400',
					'url'=> $embed_one['medium']
				);
				$photoobject['crop']['large']= array(
					'height'=>'780',
					'width'=>'1024',
					'url'=> $embed_one['large']
				);
				$photoobject['crop']['thumb']= array(
					'height'=>'79',
					'width'=>'102',
					'url'=> $embed_one['thumb']
				);
				$json['relatedObjects'][] = $photoobject; 									
    		break;
    	case "wally_videoobject":
				$embed_one = wallydemo_get_video_infos_and_display($one);
				$videoobject= array(
				//$json['relatedObjects'][] = array(
				'nid'=> $embed_one["nid"],
				'type'=> 'wally_videoobject',
				'titre'=> $embed_one["title"],
				'summary'=> $embed_one["summary"],
				'url'=> $embed_one["link"],		
				'thumbnail'=> $embed_one["thumbnail"],
				'emcode'=> $embed_one["emcode"],
				'credit'=> $embed_one["credit"]		 
				 );
				 $json['relatedObjects'][] = $videoobject; 
				
    		break;
    		
    	case "wally_audioobject":
				$embed_one = wallydemo_get_audio_infos_and_display($one);
				$json['relatedObjects'][] = array(
				'nid'=> $embed_one["nid"],
				'type'=> 'wally_audioobject',
				'filemime'=> $embed_one["filemime"],
				'titre'=> $embed_one["title"],
				'summary'=> $embed_one["summary"],
				'url'=> $embed_one["link"],		
				'thumbnail'=> $embed_one["thumbnail"]		 
				 );
				
    		break; 
    		
    	case "wally_digitalobject":
				$embed_one = wallydemo_get_digitalobject_infos_and_display($one);
				$dummy = array(
				'nid'=> $embed_one["nid"],
				'type'=> 'wally_digitalobject',
				'linkType'=> $embed_one["linkType"],
				'label'=> $embed_one["title"],
				'url'=> $embed_one["url"]		
				 );

				 if($embed_one["thumbnail"]){
					 $dummy["thumnail"]["url"]= $embed_one["thumbnail"];
					 $dummy["thumnail"]["mime"]= $embed_one["mime"];
					 $dummy["thumnail"]["link"]= $embed_one["thumbnail_img"];
				 }
				 $json['relatedObjects'][] = $dummy; 
				
    		break;   	         
    	}    
}
foreach($node->embed_links as $two){
	switch($two["group_type"]){
    	case "video":
				//$json['embed_links'][] = array(
				$json['relatedObjects'][] = array(
				'nid'=> $two["nid"],
				'titre'=> $two["title"],	
				'type'=> 'videoobject',
				'thumb'=>$two["thumb"],
				//'content'=>$two["content"],
				'module'=>$two["module"],
				'provider'=>$two["provider"]	
				 );			
		break;   	  
        case "extref":
        	$embed_link= node_load(array('nid'=>$two["nid"]));
        	$embed_pack= node_load(array('nid'=>$embed_link->field_internal_link[0]["nid"]));
        	wallycontenttypes_packagepopulate($embed_pack);
        	//$embed_textobject= node_load(array('nid'=>$embed_link->field_internal_link[0]["nid"]));
        	//var_dump($embed_pack);
        	
        	   // $json['relatedObjects'][] = array( 
        	    $extref= array(
				'nid'=> $embed_pack->nid,
				'titre'=> $embed_pack->title,       	    
				'type'=> $embed_pack->type,
        	    'title'=> $embed_pack->field_mainstory_nodes[0]->title,
        	    'chapo'=> $embed_pack->field_mainstory_nodes[0]->field_textchapo[0]['value'],
        	    'body'=> $embed_pack->field_mainstory_nodes[0]->field_textbody[0]['value']
   	    		 );

   	    		 
		foreach($embed_pack->field_embededobjects_nodes as $one){
				switch($one->type){
		    	case "wally_photoobject":
						$embed_one = wallydemo_get_photo_infos_and_display($one);
						$photoobject= array(
						//$json['relatedObjects'][] = array(
						'nid'=> $embed_one["nid"],
						'type'=> 'wally_photoobject',
						'caption'=> $embed_one["caption"],
						'credit'=> $embed_one["credit"]	);	
						$photoobject['crop']['medium']= array(					 				 				 				
							'height'=>'300',
							'width'=>'400',
							'url'=> $embed_one['medium']
						);
						$photoobject['crop']['large']= array(
							'height'=>'780',
							'width'=>'1024',
							'url'=> $embed_one['large']
						);
						$photoobject['crop']['thumb']= array(
							'height'=>'79',
							'width'=>'102',
							'url'=> $embed_one['thumb']
						);
						$extref['relatedObjects'][] = $photoobject; 									
		    		break;
		    	case "wally_videoobject":
						$embed_one = wallydemo_get_video_infos_and_display($one);
						//$json['relatedObjects'][] = array(
							$videoobject= array(
						'nid'=> $embed_one["nid"],
						'type'=> 'wally_videoobject',
						'titre'=> $embed_one["title"],
						'summary'=> $embed_one["summary"],
						'url'=> $embed_one["link"],		
						'thumbnail'=> $embed_one["thumbnail"],
						'emcode'=> $embed_one["emcode"]	,
						'credit'=> $embed_one["credit"]		 
						 );
						$extref['relatedObjects'][] = $videoobject;
		    		break;
		    		
		    	case "wally_audioobject":
						$embed_one = wallydemo_get_audio_infos_and_display($one);
							$audioobject= array(
						//$json['relatedObjects'][] = array(
						'nid'=> $embed_one["nid"],
						'type'=> 'wally_audioobject',
						'filemime'=> $embed_one["filemime"],
						'titre'=> $embed_one["title"],
						'summary'=> $embed_one["summary"],
						'url'=> $embed_one["link"],		
						'thumbnail'=> $embed_one["thumbnail"]		 
						 );
						 $extref['relatedObjects'][] = $audioobject;
						
		    		break; 
		    		
		    	case "wally_digitalobject":
						$embed_one = wallydemo_get_digitalobject_infos_and_display($one);
						$dummy = array(
						'nid'=> $embed_one["nid"],
						'type'=> 'wally_digitalobject',
						'linkType'=> $embed_one["linkType"],
						'label'=> $embed_one["title"],
						'url'=> $embed_one["url"]		
						 );
		
						 if($embed_one["thumbnail"]){
							 $dummy["thumnail"]["url"]= $embed_one["thumbnail"];
							 $dummy["thumnail"]["mime"]= $embed_one["mime"];
							 $dummy["thumnail"]["link"]= $embed_one["thumbnail_img"];
						 }
						$extref['relatedObjects'][] = $dummy; 				
		    		break;   	         
		    	}    
		} 
		
$json['relatedObjects'][] =	$extref;    		 
   	    		   	    		 
      	break; 
	}
}
// field_linkedobjects
foreach($node->field_linkedobjects_nodes as $liensObjects){
	if($liensObjects->type=='wally_linkslistobject'){
		$ess =  array(
		'nid'=> $liensObjects->nid,
		'type'=> "wally_linkslistobject",
		'titre'=> $liensObjects->title
		);		
		foreach ($liensObjects->field_links_list_nodes as $lienObj ){
		if ($lienObj->field_externalreference['0']['value']){
			$ess2 = $lienObj->field_externalreference['0']['value'];
		} else {
			$ess2= $lienObj->field_internal_link['0']['nid'];					
		}
			$ess["links"][] =  array(
				'nid'=> $lienObj->nid,
				'type'=> 'wally_linktype',
				'path'=>$lienObj->path,
				'title'=> $lienObj->field_link_item['0']['title'],
				'url'=>$ess2
				);
				
		}
		 $json['relatedObjects'][] = $ess; 
	}
}

print drupal_to_js($json);

?>



