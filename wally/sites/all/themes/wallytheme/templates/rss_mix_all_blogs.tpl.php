<?php
/**
 * Available variable
 * $feed : array with all the feeds items
 * 
 */

function _wallytheme_all_blogs_gen($data){
	if ($data['mod'] == 1){
	 $html_item = "<div id=\"".$data['div_id']."\" class=\"clearfix purple\">
	           <h3><span><a href=\"".$data['blog_url']."\">".$data['blog_name']."</a></span></h3>
              <div class=\"author\"><span class=\"tier-01\">".$data['blogger_firstname']."</span><span class=\"tier-02\">".$data['blogger_lastname']."</span></div>
              <ul class=\"clearfix\">
                <li>";
          if ($data['image_path'] != ""){
          	  $data['img'] = theme('imagecache', 'sm_bloc_blogs_thumb', $data['image_path']); 
              $html_item .= "<a href=\"".$data['url']."\">".$data['img']."</a>";
          }                                 
              $html_item .= "<h4><a href=\"".$data['url']."\">".check_plain($data['title'])."</a></h4>
                </li>";
  } else
  if ($data['mod'] == 2){
  	$html_item = "<li>";
          if ($data['image_path'] != ""){
          	  $data['img'] = theme('imagecache', 'sm_bloc_blogs_thumb', $data['image_path']);       
              $html_item .= "<a href=\"".$data['url']."\">".$data['img']."</a>";
          }   
              $html_item .= "<h4><a href=\"".$data['url']."\">".check_plain($data['title'])."</a></h4>
             </li>";
  } else
  if ($data['mod'] == 3){
    $html_item = "<li class=\"lines\"><h4><a href=\"".$data['url']."\">".check_plain($data['title'])."</a></h4></li>";
  } else {
    $html_item = "<li class=\"lines\"><h4><a href=\"".$data['url']."\">".check_plain($data['title'])."</a></h4></li>
            </ul>
            </div>";
  	
  }
	
  return $html_item;
}

$html = "";
$cpt=1;

foreach ($feed as $row){
	$item = array();
	$item['title'] = $row['Package']['MainStory']['Title']['value'];
	$item['url'] = $row['Package']['ExternalURI']['value'];
	$item['image_path'] = $row['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'];
  if ($cpt<5){
  	$item['blogger_firstname'] = "Pierre";
  	$item['blogger_lastname'] = "De Vuyst";
  	$item['blog_name'] = "Rue Royale";
  	$item['blog_url'] = "http://blog.soirmag.lesoir.be/rue-royale/";
  	$item['div_id'] = "blog-01";  	 
  }else if ($cpt >= 5 && $cpt < 9){
    $item['blogger_firstname'] = "Francesca";
    $item['blogger_lastname'] = "Casori";
    $item['blog_name'] = "Vu à la télé&hellip; réalité";
    $item['blog_url'] = "http://blog.soirmag.lesoir.be/vu-a-la-tele-realite/";
    $item['div_id'] = "blog-02";    	
  }else{
    $item['blogger_firstname'] = "Letizia";
    $item['blogger_lastname'] = "Virone";
    $item['blog_url'] = "http://blog.soirmag.lesoir.be/serie-fais-moi-peur/";
    $item['blog_name'] = "Série, fais moi peur !"; 
    $item['div_id'] = "blog-03";   	
  }
  $item['mod'] = $cpt%4;
  $html_content .= _custom_soirmag_all_blogs_gen($item);
  $cpt++;

}

$html_start = "<div id=\"pourpre\">
        <h2>En direct des blogs</h2>";
$html_end = "</div>";
$html = $html_start.$html_content.$html_end;

print $html;

