<?
/**
 * Wally default template for rendering Packages Autors List 
 */
     
    // Barette
    $textarette = "<span class='barette'>".$mainstory->field_textarette[0]["value"]."</span>";
    
    // Foretitle
    $foretitle = $textarette." ".$mainstory->field_textforetitle[0]["value"];

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

    // Photo Gallery Slide Show
    $photo_gallery = theme("wallyct_photoobject_slider",$node->field_embededobjects_nodes, $node);

    // social network icon
    $social = theme("wallyct_tofacebook", $node);
    
    // Destinations lists
    $destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");

?>
<h3><?php print $foretitle; ?></h3>             
<h2><a title="" rel="bookmark" href=""><?php print $title; ?></a></h2>             
<h3><?php print $subtitle; ?></h3>             
             
<div class="date">Published on ### ## #####<span> // <?php print $destination_term; ?>

<div class="content">
   
<?php  print $photo_gallery; ?>

<!-- <p><a href="http://bignews.blogohblog.net/wp-content/uploads/2010/11/First_Xbox_buyer.jpg"><img width="300" height="200" alt="" src="http://bignews.blogohblog.net/wp-content/uploads/2010/11/First_Xbox_buyer-300x200.jpg" title="First_Xbox_buyer" class="alignleft size-medium wp-image-75"></a>  -->

<?php print $body; ?>
                        
<?php print $social; ?>

</div>

<div class="tags">
  <a rel="tag" href="http://bignews.blogohblog.net/tag/gaming/">Gaming</a> 
  <a rel="tag" href="http://bignews.blogohblog.net/tag/tech/">Tech</a>
</div>         
<div class="clear"></div>

