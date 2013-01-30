<frame type="4" fontsize="22" maxCapacity="45" minCapacity="41" width="376" height="30.75" textlines="1">
  <![CDATA[<?php print $title;?>]]>
</frame>
<?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
  $preset = 'print_182x110';
  imagecache_generate_image($preset, $photoobject->field_photofile[0]['filepath']);
  ?>
  <frame type="2" fontsize="" maxCapacity="" minCapacity="" width="182" height="110.25">
    <![CDATA[
      <photoinfo>
  		<filepath><?php print $photoobject->field_photofile[0]['filename'];?></filepath>
  		<internal_filepath><?php print imagecache_create_path($preset, $photoobject->field_photofile[0]['filepath']);?></internal_filepath>
  	  </photoinfo>
  	]]>
  </frame>
<?php }?>
<frame type="4" fontsize="12.5" maxCapacity="674" minCapacity="554" width="182" height="220" textlines="">
  <![CDATA[<?php print $textbody;?>]]>
</frame>
<?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
  <frame type="4" fontsize="10" maxCapacity="35" minCapacity="31" width="167.1278195" height="13" textlines="1">
    <![CDATA[<?php print $photoobject->field_summary[0]['value'];?>]]>
  </frame>
<?php }?>