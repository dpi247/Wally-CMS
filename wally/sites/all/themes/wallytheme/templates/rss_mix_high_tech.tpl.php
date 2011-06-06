<?php

 function _custom_soirmag_high_tech_gen($item){
  $html = "<li>
                <a href=\"".$item['url']."\">";
                 if ($item['image_path'] != ""){
                 	$item['img'] = theme('imagecache', 'sm_bloc_high_tech_thumb', $item['image_path']); 
                  $html .=$item['img'];
                 }
                 $html .="<span class=\"overlay\">&nbsp;</span>
                </a>
              </li>";
 	return $html;
 	
 }

$item_html = "";
foreach ($feed as $row){
	$item['title']=$row['Package']['PackageTitle']['value'];
	$item['url']=$row['Package']['ExternalURI']['value'];
	$item['image_path']=$row['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'];
  $item_html .= _custom_soirmag_high_tech_gen($item);
}

$html_start = "<div id=\"bloc-blog\" class=\"brique\">
            <h2>High-tech</h2>
            <ul>";
$html_end = "</ul>
          </div>";

 $html_return = $html_start.$item_html.$html_end;
 
 print $html_return;
