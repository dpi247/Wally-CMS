<?php
/**
 * Wally default template for rendering Presons Lists. 
 */

$path = base_path() . path_to_theme();

if (!function_exists('soirmag_photoobject_slideshow')) {
  function soirmag_photoobject_slideshow($nodes, $node) {
    $content = "";
    foreach ($nodes as $n) {
      if ($n->type == "wally_photoobject") {
        $file_path = "/".$n->field_photofile[0]["filepath"];
        $file_title = $n->title;
        $content .= "<li><a href='$file_path'><img src='$file_path' width='416' height='200' alt='$file_title' /></a></li>";
      }
    }
    return $content; 
  }
}
?>
						<div id="galeria">
							<ul>
              <?php print soirmag_photoobject_slideshow($nodes, $node); ?>
							</ul>
							<div class="galeria-tools clearfix">
								<a id="btn-galeria-prev"><img src="<?php print $path; ?>/mediastore/elements/icons/rewind.gif" width="16" height="16" alt="Image précédente" /></a>
								<span class="count-item">images X de X</span>
								<a id="btn-galeria-next"><img src="<?php print $path; ?>/mediastore/elements/icons/fastforward.gif" width="16" height="16" alt="Image suivante" /></a>
							</div>
							<div class="galeria-legend">
								jeioza jeoizajeoiazej aiozej ioejiozaejioazej aiozjeaioejazioejzaioej ioezaj  oizejizoaejzaioejoziez apoiejzaioejzao
							</div>
						</div>
            
