<?php if (!empty($css_class)) { print check_plain($css_class); } ?>
<div id="pane<?php if (!empty($css_id)) { print "-".$css_id; } ?>">

<div class="row">
	<section id="content-top">
		<div id="content-top">
			<?php if($content['contenttop']) {} print $content['contenttop'];?>
		</div> <!-- /content-top -->
	</section>
</div> <!-- /row -->

<div class="row">
	<section id="main-content" class="article">
		<?php if($content['mainContent']) print $content['mainContent'];?>
	</section> <!-- /main-content -->
<?php 
   if($content['complementary'])  {
?>
	<aside role="complementary">
	<?php if($content['complementary']) print $content['complementary'];?>
	</aside> <!-- /complementary  -->
<?php 
  }
?>
</div> <!-- /row -->
<?php 
   if($content['subcontentleft'] || $content['subcontentright'])  {
?>
<div class="row">
	<section id="sub-content">
		<div id="sub-content-left">
			<?php if($content['subcontentleft']) print $content['subcontentleft'];?>
		</div> <!-- /subcontentleft -->
		<div id="sub-content-right">
			<?php if($content['subcontentright']) print $content['subcontentright'];?>
		</div> <!-- /sub-content-right -->
	</section>
</div> <!-- /row -->
<?php
	}
?>
<div class="row">
	<section id="content-bottom">
		<div id="content-bottom">
			<?php if($content['contentbottom']) {} print $content['contentbottom'];?>
		</div> <!-- /content-top -->
	</section>
</div> <!-- /row -->

</div> <!-- /pane<?php if (!empty($css_id)) { print "-".$css_id; } ?> -->