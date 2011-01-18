<?php

dsm($node); 

  $title = $node->title;
  $node_path = drupal_get_path_alias("/node/".$node->nid);
  $summary = $node->field_summary[0]["value"];
                
  

?>
    
<div class="content">


<div class="planchecontact">

      <div id="text_gal">
         <h2><?php print $title; ?></h2>
         <?php print $summary; ?>
      </div>
      <div class="photos"> 
      <ul>
      
      <?php 
       foreach ($node->field_embededobjects_nodes as $n) {
       if ($n->type == "wally_photoobject") {
       	
       	dsm($n->field_photofile);
       	 

       	$file_img = theme('imagecache', 'slider_preset', $n->field_photofile[0]["filename"]);  
       	
       	
       	
       	$result .= "<li><a href='".$n->field_photofile[0]["filepath"]."' rel='prettyPhoto[pp_gal]' title='title'>";
        $result .= $file_img; 
        $result .= "</a></li>";

        $nbr++;
      }
    }
    print $result;
      
      ?>
<!--      <li><a href="sites/all/themes/wallynews/images/steph/vanrompuy.jpg" rel="prettyPhoto[pp_gal]" title="Van Rompuy"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_vanrompuy.jpg"/></a></li>  -->
      </ul>
      </div>
    
</div>
  
 <script type="text/javascript" charset="utf-8">
    $(document).ready(function(){
      $("a[rel^='prettyPhoto']").prettyPhoto();
      alert("yipie"); 
    });
  </script>



<?php
  print theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node);
?>


</div>
<div class="clear"></div>
