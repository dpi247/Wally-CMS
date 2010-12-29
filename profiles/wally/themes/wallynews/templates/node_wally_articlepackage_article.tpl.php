<?
/**
 * Wally default template for rendering Packages Autors List 
 */
	$mainstory = $node->field_mainstory_nodes[0];

  // Foretitle
  $foretitle = $textbarette." ".$mainstory->field_textforetitle[0]["value"];

  // Title
  $title = $mainstory->title;

  // Subtitle
  $subtitle = $mainstory->field_textsubtitle[0]["value"];
    
?>
<?php
	if (isset($node->field_mainstory_nodes)) {
		print theme("wallyct_mainstory", $mainstory, $node); 
  }
?>

</div>

<div class="tags">
  <a rel="tag" href="http://bignews.blogohblog.net/tag/gaming/">Gaming</a> 
  <a rel="tag" href="http://bignews.blogohblog.net/tag/tech/">Tech</a>
</div>         
<div class="clear"></div>

<?php
  print theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node);
?>
