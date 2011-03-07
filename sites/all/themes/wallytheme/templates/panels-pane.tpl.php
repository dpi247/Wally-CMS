<?php
$js = "
	$(document).ready(function() {
 		$('#view-display-id-redacblock_1-view-dom-id-2').remove();   
  });
";

if ($variables["pane"]->subtype == "default_destination_block") {
	drupal_add_js($js, 'inline');
}

print $content;
?>