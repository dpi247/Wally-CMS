<?
/**
 * Wally default template for rendering Packages Autors List 
 */
 dsm($node);
	$mainstory = $node->field_mainstory_nodes[0];

    // Foretitle
    $foretitle = $textbarette." ".$mainstory->field_textforetitle[0]["value"];

    // Title
    $title = $mainstory->title;

    // Subtitle
    $subtitle = $mainstory->field_textsubtitle[0]["value"];
    
?>
<h3><?php print $foretitle; ?></h3>             
<h2><a title="" rel="bookmark" href=""><?php print $title; ?></a></h2>             
<h3><?php print $subtitle; ?></h3>             
             
<div class="date">Publié le <?php print date('d M Y', $mainstory->created) ?> // <a rel="category tag" title="View all posts in Featured" href="http://bignews.blogohblog.net/category/featured/">Featured</a>, <a rel="category tag" title="View all posts in Technology" href="http://bignews.blogohblog.net/category/technology/">Technology</a></span></div>        

<div class="content">

<!-- <p><a href="http://bignews.blogohblog.net/wp-content/uploads/2010/11/First_Xbox_buyer.jpg"><img width="300" height="200" alt="" src="http://bignews.blogohblog.net/wp-content/uploads/2010/11/First_Xbox_buyer-300x200.jpg" title="First_Xbox_buyer" class="alignleft size-medium wp-image-75"></a>  -->

<?php
	if (isset($node->field_embededobjects_nodes)) {
		print theme("wallyct_embededobjects", $node->field_embededobjects_nodes, $node);
	}

	if (isset($node->field_mainstory_nodes)) {
		print theme("wallyct_mainstory", $node->field_mainstory_nodes[0], $node); 
    }
?>

</div>

<div class="tags">
  <a rel="tag" href="http://bignews.blogohblog.net/tag/gaming/">Gaming</a> 
  <a rel="tag" href="http://bignews.blogohblog.net/tag/tech/">Tech</a>
</div>         
<div class="clear"></div>
