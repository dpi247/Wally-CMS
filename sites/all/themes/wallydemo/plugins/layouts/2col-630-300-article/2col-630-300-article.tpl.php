<?php
  $path = base_path().path_to_theme();     
?>

<div class="top-content full-width">
  <?php if($content['top-content']) print $content['top-content'];  ?>
</div>
<div class="first-content w630-300">
  <div class="column col-01">
    <div id="article">
      <?php if($content['article']) print $content['article'];  ?>
    </div>
    <div id="middle-content">
      <?php if($content['middle-content']) print $content['middle-content'];  ?>
    </div>
    <!-- <a id="ancre_commentaires" name="ancre_commentaires" href="#ancre_commentaires"></a>
  <div id="commentaires">
    <div class="bloc-02">
			<?php // @todo: virer le H2 d'ici  ?>
      <h2>Ajouter un commentaire à cet article</h2>
	  <?php if($content['commentaires']) print $content['commentaires'];  ?>
    </div>
  </div>
   --> 
  </div>
  <div class="column col-02">
    <?php if($content['col-02']) print $content['col-02'];  ?>
  </div>
</div>
<div class="bottom-content full-width">
  <?php if($content['bottom-content']) print $content['bottom-content'];  ?>
</div>
