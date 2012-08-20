<?php 
/*dsm($subtype,'subtype');
dsm($context,'context');
dsm($feed,'feed');
dsm($options,'options');*/

	$bloc_title = $options['title'];
	$bloc_class = $options['flows'][0]->name;
	
 $content = "";
  foreach ($feed as $k=>$item) {
 
    	if(isset($item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'])){
        $item['image_path'] = $item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'];
	      $content .= "<li class=\"avecphoto\">";
	      if ($item['image_path'] != ""){
	        $item['img'] = theme('imagecache', 'divers_201x134', $item['image_path']); 
	        $content .="<a href='".$item['ExternalURI']['value']."' target=\"_blank\">
	               ".$item['img']."
	              </a>";
	      }else{
        $content .="<li>";
	      }
    	}else{
    	 $content .="<li>";
    	}
      $content .="<a href='".$item['ExternalURI']['value']."' target=\"_blank\">".wallydemo_check_plain($item['MainStory']['Title']['value'])."</a>
             </li>";
     }
 
?>

<div class="<?php print $bloc_class; ?> bloc-01">
  <h2><a target="_blank" href="<?php print $item['ExternalURI']['value']; ?>"><?php print $bloc_title; ?></a></h2>
  <ul class="inner-bloc">
    <?php print $content; ?>
  </ul>
</div>
