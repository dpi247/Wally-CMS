<template size="3" name="UNE-3" articlename="<?php print $node->nid;?>">
  <frame type="4" fontsize="22" maxCapacity="95" minCapacity="87" width="764" height="32.25" textlines="1">
    <![CDATA[<?php print $mainstory->title;?>]]>
  </frame>
  <?php if ($mainstory->field_textchapo[0]['value'] != NULL){?>
    <frame type="4" fontsize="16" maxCapacity="242" minCapacity="202" width="764" height="48" textlines="2">
      <![CDATA[<?php print $mainstory->field_textchapo[0]['value'];?>]]>
    </frame>
  <?php } if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
    $preset = 'print_376x240';
    imagecache_generate_image($preset, $photoobject->field_photofile[0]['filepath']);
    ?>
    <frame type="2" fontsize="" maxCapacity="" minCapacity="" width="376" height="240.5">
      <![CDATA[
        <photoinfo>
    		<filepath><?php print $photoobject->field_photofile[0]['filename'];?></filepath>
    		<internal_filepath><?php print imagecache_create_path($preset, $photoobject->field_photofile[0]['filepath']);?></internal_filepath>
    	  </photoinfo>
      ]]>
    </frame>
  <?php }?>
  <frame type="4" fontsize="12.5" maxCapacity="1156" minCapacity="957" width="182" height="264" textlines="">
    <![CDATA[<?php print $mainstory->field_textbody[0]['value'];?>]]>
  </frame>
  <?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
    <frame type="4" fontsize="10" maxCapacity="87" minCapacity="74" width="361.1278195" height="13" textlines="1">
      <![CDATA[<?php print $photoobject->field_summary[0]['value'];?>]]>
    </frame>
  <?php }?>
</template>