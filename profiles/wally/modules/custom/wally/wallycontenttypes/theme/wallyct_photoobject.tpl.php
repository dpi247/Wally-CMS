<?php

?>

<a title="<?php print $embededobject->title?>" rel="prettyPhoto[pp_gal]" href="/<?php print $embededobject->field_photofile[0]['filepath']; ?>"> 
  <?php print  theme('imagecache','slider_preset',$embededobject->field_photofile[0]['filepath'])?>
</a>
