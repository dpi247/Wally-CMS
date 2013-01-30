<templates customer="<?php print variable_get('cpsprint_customer', 'DPI247')?>" pub="<?php print variable_get('cpsprint_pub', 'The Unfold')?>" edition="Main" exportdate="<?php print date('Y-m-d');?>" pubdate="<?php print date('Ymd');?>">
  <template size="" name="UNE-1-TOP" pagetype="UNE">
    <frame type="4" fontsize="17" maxCapacity="202" minCapacity="177" width="764" height="40" textlines="2">
      <![CDATA[<?php print variable_get('sendprint_name', 'The Unfold');?>]]>
    </frame>
  </template>
  <?php if ($content['65x180']) { ?>
      <?php print $content['65x180'];?>
  <?php }?>
  <?php if ($content['200x180']) { ?>
      <?php print $content['200x180'];?>
  <?php }?>
  <?php if ($content['270x130']) { ?>
      <?php print $content['270x130'];?>
  <?php }?>
</templates>