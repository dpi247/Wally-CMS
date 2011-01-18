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
       foreach ($nodes as $n) {
       if ($n->type == "wally_photoobject") {
        //$file_path = "/".$n->field_photofile[0]["filepath"];        
        $file_img = theme('imagecache', 'slider_preset', $n->field_photofile[0]["filename"], 'Slide '.$nbr);  
        $result .= "<a href='#' title='Title'>";
        $result .= $file_img; 
        //$result .= "<img src='".$file_path."' width='300' height='200' alt='Slide ".$nbr."'>";
        $result .= "</a>";
        $nbr++;
      }
    }
      
      ?>
      <li><a href="sites/all/themes/wallynews/images/steph/danneels.jpg" rel="prettyPhoto[pp_gal]" title="Danneels s'explique"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_danneels.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/dewever.jpg" rel="prettyPhoto[pp_gal]" title="De Wever est désolé"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dewever.jpg"/></a></li>
      <li><a href="http://www.youtube.com/watch?v=xrTpZIz6RvE" rel="prettyPhoto[pp_gal]" title="Vidéo"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dewever.jpg" alt="Vidéo" /></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/dirupo.jpg" rel="prettyPhoto[pp_gal]" title="Di Rupo a une sale tête"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dirupo.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/ecolo.jpg" rel="prettyPhoto[pp_gal]" title="Ecolo et Groen"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_ecolo.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/leterme.jpg" rel="prettyPhoto[pp_gal]" title="Yves Leterme"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_leterme.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/letermereve.jpg" rel="prettyPhoto[pp_gal]" title="Yves Leterme a de l'espoir"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_letermereve.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/vandelanotte.jpg" rel="prettyPhoto[pp_gal]" title="Van De Lanotte arrive au palais royal"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_vandelanotte.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/vanrompuy.jpg" rel="prettyPhoto[pp_gal]" title="Van Rompuy"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_vanrompuy.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/danneels.jpg" rel="prettyPhoto[pp_gal]" title="Danneels s'explique"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_danneels.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/dewever.jpg" rel="prettyPhoto[pp_gal]" title="De Wever est désolé"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dewever.jpg"/></a></li>
      <li><a href="http://www.youtube.com/watch?v=xrTpZIz6RvE" rel="prettyPhoto[pp_gal]" title="Vidéo"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dewever.jpg" alt="Vidéo" /></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/dirupo.jpg" rel="prettyPhoto[pp_gal]" title="Di Rupo a une sale tête"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dirupo.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/ecolo.jpg" rel="prettyPhoto[pp_gal]" title="Ecolo et Groen"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_ecolo.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/leterme.jpg" rel="prettyPhoto[pp_gal]" title="Yves Leterme"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_leterme.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/letermereve.jpg" rel="prettyPhoto[pp_gal]" title="Yves Leterme a de l'espoir"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_letermereve.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/vandelanotte.jpg" rel="prettyPhoto[pp_gal]" title="Van De Lanotte arrive au palais royal"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_vandelanotte.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/vanrompuy.jpg" rel="prettyPhoto[pp_gal]" title="Van Rompuy"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_vanrompuy.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/danneels.jpg" rel="prettyPhoto[pp_gal]" title="Danneels s'explique"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_danneels.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/dewever.jpg" rel="prettyPhoto[pp_gal]" title="De Wever est désolé"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dewever.jpg"/></a></li>
      <li><a href="http://www.youtube.com/watch?v=xrTpZIz6RvE" rel="prettyPhoto[pp_gal]" title="Vidéo"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dewever.jpg" alt="Vidéo" /></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/dirupo.jpg" rel="prettyPhoto[pp_gal]" title="Di Rupo a une sale tête"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_dirupo.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/ecolo.jpg" rel="prettyPhoto[pp_gal]" title="Ecolo et Groen"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_ecolo.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/leterme.jpg" rel="prettyPhoto[pp_gal]" title="Yves Leterme"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_leterme.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/letermereve.jpg" rel="prettyPhoto[pp_gal]" title="Yves Leterme a de l'espoir"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_letermereve.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/vandelanotte.jpg" rel="prettyPhoto[pp_gal]" title="Van De Lanotte arrive au palais royal"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_vandelanotte.jpg"/></a></li>
      <li><a href="sites/all/themes/wallynews/images/steph/vanrompuy.jpg" rel="prettyPhoto[pp_gal]" title="Van Rompuy"><img src="sites/all/themes/wallynews/images/steph/thumbnails/t_vanrompuy.jpg"/></a></li>
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
