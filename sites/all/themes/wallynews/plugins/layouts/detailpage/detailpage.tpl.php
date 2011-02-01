<?php
  $path = base_path().path_to_theme();
?>

<div id="detailpage-container">

  <?php if($content['header']) {
    print "<div class='clear'></div>";
    print "<div class='largebox'>";
    print $content['header'];
    print "</div>";
    }   
  ?>

  <div class="clear"></div>
  <div class="col_large">
    <?php if($content['colmain']) print $content['colmain'];  ?>
  </div>
  <div class="col3">
    <?php if($content['colrelated']) print $content['colrelated'];  ?>
  </div>
  
  <?php if($content['footer']) {
    print "<div class='clear'></div>";
    print "<div class='largebox'>";
    print $content['footer'];
    print "</div>";
    }   
  ?>

    <div class="clear"></div>
</div>
