<frame type="4" fontsize="22" maxCapacity="93" minCapacity="82" width="376" height="56.25" textlines="2" id="1">
  <![CDATA[<?php if ($textbarette != NULL){print $textbarette.'. ';}print $title;?>]]>
</frame>

<frame type="4" fontsize="16" maxCapacity="171" minCapacity="143" width="376" height="72" textlines="3" id="2">
  <?php if ($textchapo != NULL){?>
    <![CDATA[<?php print $textchapo;?>]]>
  <?php }?>
</frame>
<?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
  $preset = 'print_376x208';
  imagecache_generate_image($preset, $photoobject->field_photofile[0]['filepath']);
  ?>
  <frame type="2" fontsize="" maxCapacity="" minCapacity="" width="376" height="208.75" id="3">
    <![CDATA[
      <photoinfo>
       	<filepath><?php print $photoobject->field_photofile[0]['filename'];?></filepath>
  		<internal_filepath><?php print imagecache_create_path($preset, $photoobject->field_photofile[0]['filepath']);?></internal_filepath>
  	  </photoinfo>
  	]]>
  </frame>
<?php }?>
<frame type="4" fontsize="12.5" maxCapacity="742" minCapacity="615" width="182" height="176" textlines="" id="4">
  <![CDATA[<?php print $textbody;?>]]>
</frame>
<frame type="4" fontsize="10" maxCapacity="87" minCapacity="74" width="361.1278195" height="13" textlines="1" id="5">
  <?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
    <![CDATA[<?php print $photoobject->field_summary[0]['value'];?>]]>
  <?php }?>
</frame>
