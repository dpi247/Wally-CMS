<frame type="4" fontsize="30" maxCapacity="41" minCapacity="35" width="376" height="88" textlines="2" id="1">
  <![CDATA[<?php if ($textbarette != NULL){print $textbarette.'. ';}print $title;?>]]>
</frame>

<frame type="4" fontsize="16" maxCapacity="122" minCapacity="109" width="376" height="44" textlines="2" id="2">
  <?php if ($textchapo != NULL){?>
    <![CDATA[<?php print $textchapo;?>]]>
  <?php }?>
</frame>
<?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
  $preset = 'print_182x176';
  imagecache_generate_image($preset, $photoobject->field_photofile[0]['filepath']);
  ?>
  <frame type="2" fontsize="" maxCapacity="" minCapacity="" width="182" height="176" id="3">
    <![CDATA[
      <photoinfo>
		<filepath><?php print $photoobject->field_photofile[0]['filename'];?></filepath>
  		<internal_filepath><?php print imagecache_create_path($preset, $photoobject->field_photofile[0]['filepath']);?></internal_filepath>
  	  </photoinfo>
  	]]>
  </frame>
<?php }?>
<frame type="4" fontsize="10" maxCapacity="2340" minCapacity="2145" width="182" height="396" textlines="" id="4">
  <![CDATA[<?php print $textbody;?>]]>
</frame>