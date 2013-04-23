<?php
// $Id: media-embedly-default.tpl.php,v 1.0 2011/01/07 ODM $

/**
 * @file media_embedly/themes/media-embedly-default.tpl.php
 *
 * Template file for Media: Embedly's theme('media_embedly_default_external').
 *
 * This will display Embedly's returned embedded media.
 *
 */
 $options = $variables['variables'];
?>
<div class="embedly-embed-default-external">
<?php 
print($options['html']);
 ?>
</div>
