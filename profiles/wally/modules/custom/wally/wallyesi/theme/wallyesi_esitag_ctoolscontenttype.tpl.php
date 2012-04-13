<?php 
$attributes_string='';
foreach ($variables["esi"]['attributes'] as $key=>$value){
  $attributes_string.="$key=\"$value\" ";
}
?>
<esi:<?php print$variables['esi']['action']?> <?php print $attributes_string?> />
