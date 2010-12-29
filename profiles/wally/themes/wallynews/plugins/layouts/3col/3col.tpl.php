<?php
  $path = base_path().path_to_theme();
?>

<div id="container">

  <?php if($content['header']) {
    print "<div class='clear'></div>";
    print "<div class='largebox'>";
    print $content['header'];
    print "</div>";
    }   
  ?>
  
  <div class="col1">
    <?php if($content['colone']) print $content['colone'];  ?>
  </div>

  <div class="col2">
    <?php if($content['coltwo']) print $content['coltwo'];  ?>
  </div>

  <div class="col3">
    <?php if($content['colthree']) print $content['colthree'];  ?>
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
