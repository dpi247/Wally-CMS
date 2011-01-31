<?php
  $title = $node->title;
  $node_path = drupal_get_path_alias("/node/".$node->nid);
  $summary = $node->field_summary[0]["value"];
?>
    
<div class="content">

<div class="planchecontact">

      <h2><?php print $title; ?></h2>
      <div class="content">
      <?php print $summary; ?>
      </div>
      
      <div class="photos"> 
      <ul>
           
      <?php 
       foreach ($node->field_embededobjects_nodes as $n) {
       if ($n->type == "wally_photoobject") {
       	$file_img = theme('imagecache', 'theme_medium_article_preset', $n->field_photofile[0]["filename"]);  
       	$result .= "<li><a href='/".$n->field_photofile[0]["filepath"]."' rel='prettyPhoto[pp_gal]' title='".$n->title."'>";
        $result .= $file_img; 
        $result .= "</a></li>";
        $nbr++;
      }
    }
    print $result;
      
      ?>
      </ul>
      </div>
    
</div>
  
 <script type="text/javascript" charset="utf-8">
    $(document).ready(function(){
      $("a[rel^='prettyPhoto']").prettyPhoto();
    });
  </script>

<?php
  print theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node);
?>

</div>
<div class="clear"></div>
