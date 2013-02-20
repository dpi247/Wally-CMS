<templates customer="<?php print wally_variable_get('cpsprint_customer', 'DPI247')?>" pub="<?php print wally_variable_get('cpsprint_pub', 'The Unfold')?>" edition="Main" exportdate="<?php print date('Y-m-d');?>" pubdate="<?php print date('Ymd');?>">
  <?php if ($content['133x185']) { ?>
      <?php print $content['133x185'];?>
  <?php }?>
  <?php if ($content['133x90A']) { ?>
      <?php print $content['133x90A'];?>
  <?php }?>
  <?php if ($content['133x90B']) { ?>
      <?php print $content['133x90B'];?>
  <?php }?>
  <?php if ($content['133x185B']) { ?>
  	  <?php print $content['133x185B'];?>
  <?php }?>
  <?php if ($content['133x90C']) { ?>
      <?php print $content['133x90C'];?>
  <?php }?>
  <?php if ($content['133x90D']) { ?>
      <?php print $content['133x90D'];?>
  <?php }?>
 </templates>