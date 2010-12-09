<?php
/**
 * Wally default template for rendering Presons Lists. 
 */

if (!function_exists('wallyct_photoobject_slideshow')) {
  function wallyct_photoobject_slideshow($nodes, $node) {
    
    drupal_add_css(drupal_get_path('module', 'wallycontenttypes') . '/css/galleryview/galleryview.css');
    drupal_add_js(drupal_get_path('module', 'wallycontenttypes') . '/scripts/galleryview/jquery.galleryview-2.1.1.js');
    drupal_add_js(drupal_get_path('module', 'wallycontenttypes') . '/scripts/galleryview/jquery.timers-1.2.js');
    drupal_add_js("
      $(document).ready(function() {
        $('#gallery-".$node->nid."').galleryView({
        panel_width: 400,
        panel_height: 200,
        pause_on_hover: true,
        frame_width: 40,
        frame_height: 25,
        show_captions: true,
        });
      });
    ", 'inline');

    $content = "";

    $content .= "<ul id='gallery-".$node->nid."'>\n";
        
    foreach ($nodes as $n) {
      
      if ($n->type == "wally_photoobject") {
        $file_path = "/".$n->field_photofile[0]["filepath"];
        $content .= "<li>\n";
        $content .= "<img src='".$file_path."' title='Pretty Picture' />\n";
        $content .= "<div class='panel-overlay'>\n";
        $content .= "<h3>".$n->title."</h3>\n";
        $content .= "<p>";
        $content .= $n->field_summary[0]["value"]."<br/>";
        $content .= "</div>";
        $content .= "</li>";
      }
    }
    $content .= "</ul>";
    return $content; 
  }
}


print wallyct_photoobject_slideshow($nodes, $node);

?>
