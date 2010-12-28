<?
/**
 * Wally default template for rendering Packages Autors List 
 */
     
    // Barette
    if ($mainstory->field_textbarette[0]["value"]) {
      $textbarette = "<span class='barette'>".$mainstory->field_textbarette[0]["value"]."</span>";
    } else $textbarette = ""; 
    
    // Foretitle
    if ($mainstory->field_textforetitle[0]["value"]) {
      if ($textbarette) {
        $foretitle = $textbarette." ".$mainstory->field_textforetitle[0]["value"];
      } else {
        $foretitle = $mainstory->field_textforetitle[0]["value"];
      }
    } else $foretitle = ""; 

    // Title
    $title = $mainstory->title;

    // Subtitle
    if ($mainstory->field_textsubtitle[0]["value"]) {
      $subtitle = $mainstory->field_textsubtitle[0]["value"];
    } else $subtitle = ""; 
    
    // Body
    $body = $mainstory->field_textbody[0]["value"];
      
    // Authors
    if ($mainstory->field_authors_nodes) {
      $authors = "<fieldset id='mainstoryauthors'><h3>Authors</h3>".theme("wallyct_personslist",$mainstory->field_authors_nodes, $mainstory)."</fieldset>";
    } else $authors = ""; 

    // Persons
    //$persons = "<fieldset id='mainstoryauthors'><h3>Persons</h3>".theme("wallyct_personslist",$mainstory->field_persons_nodes, $mainstory)."</fieldset>";
    if ($mainstory->field_persons_nodes) {
      $persons = "<fieldset id='mainstoryauthors'><h3>Persons</h3>".theme("wallyct_personslist_detail",$mainstory->field_persons_nodes, $mainstory)."</fieldset>";
    } else $persons = ""; 

    // copyright
    if ($mainstory->field_copyright[0]["value"]) {
      $copyright = "<span class='copyright'>".$mainstory->field_copyright[0]["value"]."</span>";
    } else $copyright = ""; 

    // Photo Gallery Slide Show
    if ($node->field_embededobjects_nodes) {
     $photo_gallery = theme("wallyct_photoobject_slider",$node->field_embededobjects_nodes, $node);
    } else $photo_gallery =""; 

    // social network icon
    $social = theme("wallyct_tofacebook", $node);
    
    // Destinations lists
    $destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");

    // Linsks lists Blocks
    if ($node->field_linkedobjects_nodes) {
      $linklists = theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node ); 
    } else $linklists = "";
  
?>
<h3><?php print $foretitle; ?></h3>             
<h2><a title="" rel="bookmark" href=""><?php print $title; ?></a></h2>             
<h3><?php if($subtitle) print $subtitle; ?></h3>             
             
<div class="date">Publié le <?php print date('d M Y', $mainstory->created) ?>  // <?php print $destination_term; ?></div>

<div class="content">
  <?php  print $photo_gallery; ?>
  <?php print $body; ?>
  <?php print $linklists; ?>
</div>
<div class="clear"></div>
<?php print $social; ?>
<div class="clear"></div>
