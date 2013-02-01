<templates customer="<?php print variable_get('cpsprint_customer', 'DPI247')?>" pub="<?php print variable_get('cpsprint_pub', 'The Unfold')?>" edition="Main" exportdate="<?php print date('Y-m-d');?>" pubdate="<?php print date('Ymd');?>">
  <template size="" name="UNE-1-TOP" pagetype="UNE">
    <frame type="4" fontsize="14" maxCapacity="22" minCapacity="18" width="146" height="20.5" textlines="1" cropwidth="" cropheight="" cropleft="" croptop="" id="6">
	  <![CDATA[<?php print variable_get('sendprint_date', t(date('j F Y')));?>]]>
	</frame>
	<frame type="4" fontsize="22" maxCapacity="41" minCapacity="35" width="400" height="27.25" textlines="1" cropwidth="" cropheight="" cropleft="" croptop="" id="7">
	  <![CDATA[<?php print variable_get('sendprint_name', 'The Unfold');?>]]>
	</frame>
	<frame type="4" fontsize="14" maxCapacity="18" minCapacity="12" width="125.75" height="20.5" textlines="1" cropwidth="" cropheight="" cropleft="" croptop="" id="8">
	  <![CDATA[<?php print variable_get('sendprint_front', 'UNE');?>]]>
	</frame>
	<frame type="4" fontsize="13" maxCapacity="95" minCapacity="87" width="270" height="37.5" textlines="2" cropwidth="" cropheight="" cropleft="" croptop="" id="10">
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