<template size="0500" name="INT-0500" hpos="1" level="3" articlename="<?php print $node->nid;?>">
  <frame type="4" fontsize="22" maxCapacity="95" minCapacity="87" width="764" height="30.75" textlines="1" cropwidth="" cropheight="" cropleft="" croptop="" id="1">
    <![CDATA[<?php if ($mainstory->field_textbarette[0]['value'] != NULL){print $mainstory->field_textbarette[0]['value'].'. ';}print $mainstory->title;?>]]>
  </frame>
  
  <frame type="4" fontsize="10" maxCapacity="87" minCapacity="74" width="361.1278195" height="13" textlines="1" cropwidth="" cropheight="" cropleft="" croptop="" id="2">
    <?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
      <![CDATA[<?php print $photoobject->field_summary[0]['value'];?>]]>
    <?php }?>
  </frame>
  
  <frame type="4" fontsize="12.5" maxCapacity="3000" minCapacity="2516" width="182" height="460" textlines="" cropwidth="" cropheight="" cropleft="" croptop="" id="4">
    <![CDATA[<?php print $mainstory->field_textbody[0]['value'];?>]]>
  </frame>
  
  <frame type="4" fontsize="16" maxCapacity="122" minCapacity="102" width="764" height="22.75" textlines="1" cropwidth="" cropheight="" cropleft="" croptop="" id="5">
    <?php if ($mainstory->field_textchapo[0]['value'] != NULL){?>
      <![CDATA[<?php print $mainstory->field_textchapo[0]['value'];?>]]>
    <?php }?>
  </frame>
  
  <frame type="2" fontsize="" maxCapacity="" minCapacity="" width="376" height="215.25" textlines="" cropwidth="" cropheight="" cropleft="" croptop="" id="7">
    <?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
    $preset = 'print_376x215';
    imagecache_generate_image($preset, $photoobject->field_photofile[0]['filepath']);
    ?>
      <![CDATA[
        <photoinfo>
  		  <filepath><?php print $photoobject->field_photofile[0]['filename'];?></filepath>
  		  <internal_filepath><?php print imagecache_create_path($preset, $photoobject->field_photofile[0]['filepath']);?></internal_filepath>
  	    </photoinfo>
  	  ]]>
    <?php }?>
  
  </frame>
</template>