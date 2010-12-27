<?
/**
 * Wally default template for rendering Packages Autors List 
 */
     
    // Barette
    $textbarette = "<span class='barette'>".$mainstory->field_textbarette[0]["value"]."</span>";
    
    // Foretitle
    $foretitle = $textbarette." ".$mainstory->field_textforetitle[0]["value"];

    // Title
    $title = $mainstory->title;

    // Subtitle
    $subtitle = $mainstory->field_textsubtitle[0]["value"];
    
    // Body
    $body = $mainstory->field_textbody[0]["value"];
      
    // Authors
    $authors = "<fieldset id='mainstoryauthors'><h3>Authors</h3>".theme("wallyct_personslist",$mainstory->field_authors_nodes, $mainstory)."</fieldset>";

    // Persons
    //$persons = "<fieldset id='mainstoryauthors'><h3>Persons</h3>".theme("wallyct_personslist",$mainstory->field_persons_nodes, $mainstory)."</fieldset>";
    $persons = "<fieldset id='mainstoryauthors'><h3>Persons</h3>".theme("wallyct_personslist_detail",$mainstory->field_persons_nodes, $mainstory)."</fieldset>";

    // copyright
    $copyright = "<span class='copyright'>".$mainstory->field_copyright[0]["value"]."</span>";

    // social network icon
    $social = theme("wallyct_tofacebook", $node);
    
    // Destinations lists
    $destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");

    // Linsks lists Blocks
    $linklists = theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node ); 
  
?>

<?php print $body; ?>
                        
<?php print $social; ?>
