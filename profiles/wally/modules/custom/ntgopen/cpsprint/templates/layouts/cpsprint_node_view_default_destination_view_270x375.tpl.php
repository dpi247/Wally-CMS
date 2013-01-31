<template size="1000" name="INT-1000" articlename="<?php print $node->nid;?>">
  <frame type="4" fontsize="22" maxCapacity="95" minCapacity="87" width="764" height="30.75" textlines="1" id="1">
    <![CDATA[<?php if ($mainstory->field_textbarette[0]['value'] != NULL){print $mainstory->field_textbarette[0]['value'].'. ';}print $mainstory->title;?>]]>
  </frame>
  
  <frame type="4" fontsize="10" maxCapacity="177" minCapacity="161" width="749.1278195" height="13" textlines="1" id="2">
    <?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
      <![CDATA[<?php print $photoobject->field_summary[0]['value'];?>]]>
    <?php }?>
  </frame>
  <frame type="4" fontsize="12.5" maxCapacity="4504" minCapacity="3752" width="182" height="503" textlines="" id="3">
    <![CDATA[<?php print $mainstory->field_textbody[0]['value'];?>]]>
  </frame>
  
  
  <frame type="4" fontsize="16" maxCapacity="122" minCapacity="102" width="764" height="22.75" textlines="1" id="4">
    <?php if ($mainstory->field_textchapo[0]['value'] != NULL){?>
      <![CDATA[<?php print $mainstory->field_textchapo[0]['value'];?>]]>
    <?php }?>
  </frame>
  
  <?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
    $preset = 'print_764x463';
    imagecache_generate_image($preset, $photoobject->field_photofile[0]['filepath']);
    ?>
    <frame type="2" fontsize="" maxCapacity="" minCapacity="" width="764" height="463" textlines="" id="5">
      <![CDATA[
        <photoinfo>
    		<filepath><?php print $photoobject->field_photofile[0]['filename'];?></filepath>
    		<internal_filepath><?php print imagecache_create_path($preset, $photoobject->field_photofile[0]['filepath']);?></internal_filepath>
    	  </photoinfo>
    	]]>
    </frame>
  <?php }?>
</template>