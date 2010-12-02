<?
/**
 * Wally default template for rendering Packages Autors List 
 * 
 *
 */
 
if (!function_exists('wally_tpl_mainstory')) {
  function wally_tpl_mainstory($story, $node) {

    $maincol = "";
    $leftcol = "";
    
    // Barette
    $textarette = "<span class='barette'>".$story->field_textarette[0]["value"]."</span>";

    // Foretitle
    $foretitle = "<h3 class='subtitle'>".$textarette." ".$story->field_textforetitle[0]["value"]."</h3>";

    // Title
    $title = "<h2 class='title'>".$story->title."</h2>";

    // Subtitle
    $subtitle = "<h3 class='subtitle'>".$story->field_textsubtitle[0]["value"]."</h3>"; 

    // Body
    $body = "<span class='body'>".$story->field_textbody[0]["value"]."</span>"; 
      
    // Authors
    $authors = "<fieldset id='mainstoryauthors'><h3>Authors</h3>".theme("wallyct_personslist",$story->field_authors_nodes, $node)."</fieldset>";

    // Persons
    //$persons = "<fieldset id='mainstoryauthors'><h3>Persons</h3>".theme("wallyct_personslist",$story->field_persons_nodes, $node)."</fieldset>";
    $persons = "<fieldset id='mainstoryauthors'><h3>Persons</h3>".theme("wallyct_personslist_detail",$story->field_persons_nodes, $node)."</fieldset>";

    // copyright
    $copyright = "<span class='copyright'>".$story->field_copyright[0]["value"]."</span>";

    // Photo Gallery Slide Show
    $photo_gallery = "<div id='main_obj_photo_slide'>".theme("wallyct_photoobject_slideshow",$node->field_embededobjects_nodes, $node)."</div>";
  
    $maincol .= "<fieldset id='maincol' style='width:95%'>"; 
    $maincol .= $foretitle; 
    $maincol .= $title; 
    
    $maincol .= $photo_gallery;
    $maincol .= $subtitle; 
    $maincol .= $body; 
    
    $maincol .= "</fieldset>";

    $leftcol .= $authors; 

    $content = "<div style='width:70%;float:left'>".$maincol."</div><div style='width:30%;float:right'>".$leftcol."</div>";
    return $content; 
 
  }
}

dsm($mainstory);

?>

<div id="mainstory">
<fieldset id='mainstory'>
<?php print wally_tpl_mainstory($mainstory, $node); ?>
</fieldset>
</div>
