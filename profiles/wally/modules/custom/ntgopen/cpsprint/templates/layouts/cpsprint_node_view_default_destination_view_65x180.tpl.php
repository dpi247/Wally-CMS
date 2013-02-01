<template size="2" name="UNE-2-G" articlename="<?php print $node->nid;?>">
  <frame type="4" fontsize="22" maxCapacity="31" minCapacity="28" width="182" height="66" textlines="2" cropwidth="" cropheight="" cropleft="" croptop="" id="2">
    <![CDATA[<?php if ($mainstory->field_textbarette[0]['value'] != NULL){print $mainstory->field_textbarette[0]['value'].'. ';}print $mainstory->title;?>]]>
  </frame>
  
  <frame type="2" fontsize="" maxCapacity="" minCapacity="" width="182" height="176" textlines="" cropwidth="" cropheight="" cropleft="" croptop="" id="3">
    <?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
    $preset = 'print_182x176';
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
    
  <frame type="4" fontsize="10" maxCapacity="35" minCapacity="31" width="167.127819548872" height="13" textlines="1" cropwidth="" cropheight="" cropleft="" croptop="" id="4">
    <?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
      <![CDATA[<?php print $photoobject->field_summary[0]['value'];?>]]>
    <?php }?>
  </frame>
  
  <frame type="4" fontsize="12.5" maxCapacity="513" minCapacity="493" width="182" height="264" textlines="" cropwidth="" cropheight="" cropleft="" croptop="" id="6">
    <![CDATA[<?php print $mainstory->field_textbody[0]['value'];?>]]>
  </frame>
</template>