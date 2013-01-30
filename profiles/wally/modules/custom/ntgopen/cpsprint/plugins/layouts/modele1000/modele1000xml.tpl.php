<templates customer="<?php print variable_get('cpsprint_customer', 'DPI247')?>" pub="<?php print variable_get('cpsprint_pub', 'The Unfold')?>" edition="Main" exportdate="<?php print date('Y-m-d');?>" pubdate="<?php print date('Ymd');?>">
  <?php if ($content['270x375']) { ?>
      <?php print $content['270x375'];?>
  <?php }?> 
</templates>