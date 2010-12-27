<?php

    $image_base = base_path() . drupal_get_path('module', 'wallycontenttypes') . '/images/slider/';
    $nid = $node->nid; 

    $js = "
          $(document).ready(function() {
            $('.slides-".$nid."').slides({
              preload: true,
              play: 5000,
              pause: 2500,
              hoverPause: true
            });
          });
    ";
    
    drupal_add_css(drupal_get_path('module', 'wallycontenttypes') . '/css/slider/slider.css');
    drupal_add_js(drupal_get_path('module', 'wallycontenttypes') . '/scripts/slider/slides.min.jquery.js');
    drupal_add_js($js, 'inline');


    $result = "";
    $nbr = 1;
    
    foreach ($nodes as $n) {
      if ($n->type == "wally_photoobject") {
        //$file_path = "/".$n->field_photofile[0]["filepath"];
        $file_path = $node->field_embededobjects_nodes[0]->field_photofile[0]['filepath'];
        $explfilepath = explode('/', $file_path);        
        $file_img = theme('imagecache', 'slider_preset', $explfilepath[sizeof($explfilepath)-1], 'Slide '.$nbr);  
 				$result .= "<a href='#' title='Title'>";
        $result .= $file_img; 
        //$result .= "<img src='".$file_path."' width='300' height='200' alt='Slide ".$nbr."'>";
        $result .= "</a>";
        $nbr++;
      }
    }

?>
<div id="slide-bloc">
			<div class="slides slides-<?php print $nid; ?>">
				<div class="slides_container">
          <?php print $result; ?>
				</div>
				<a href="#" class="prev"><img src="<?php print $image_base; ?>prev.gif" width="29" height="24" alt="Arrow Prev"></a>
				<a href="#" class="next"><img src="<?php print $image_base; ?>next.gif" width="29" height="24" alt="Arrow Next"></a> 
			</div>
</div>
