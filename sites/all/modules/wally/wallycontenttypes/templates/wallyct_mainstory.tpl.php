<?
/**
 * Wally default template for rendering Packages Autors List 
 * 
 */
function wally_tpl_mainstory($story, $node) {

  $content = "";
  
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
  //$authors = theme("wallyct_authorslist",$story->field_authors, $node);

  // copyright
  $copyright = "<span class='copyright'>".$story->field_copyright[0]["value"]."</span>";
    
  $content .= $foretitle; 
  $content .= $title; 
  $content .= $subtitle; 
  $content .= $body; 
  
  return $content; 

}
dsm($mainstory); 
?>

<div id="mainstory">
<?php print wally_tpl_mainstory($mainstory, $node); ?>
</div>
