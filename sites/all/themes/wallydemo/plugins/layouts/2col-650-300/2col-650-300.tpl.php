<?php
  $path = base_path().path_to_theme();
?>

<div>
  <div class="top-content full-width pages_service">
    <?php if($content['top-content']) print $content['top-content'];  ?>
  </div>
  <div class="first-content w650-300 pages_service">
    <div class="column col-01">
      <?php if($content['col-650-first']) print $content['col-650-first'];  ?>
    </div>
    <div class="column col-03">
      <?php if($content['col-300-first']) print $content['col-300-first'];  ?>
    </div>
  </div>
  <div class="middle-content full-width">
    <?php if($content['middle-content']) print $content['middle-content'];  ?>
  </div>
  <div class="second-content w650-300">
    <div class="column col-01">
      <?php if($content['col-650-second']) print $content['col-650-second'];  ?>
    </div>
    <div class="column col-03">
      <?php if($content['col-300-second']) print $content['col-300-second'];  ?>
    </div>
  </div>
</div>
<div class="bottom-content full-width">
  <?php if($content['bottom-content']) print $content['bottom-content'];  ?>
</div>
