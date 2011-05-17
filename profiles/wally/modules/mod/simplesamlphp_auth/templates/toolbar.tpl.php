<?php

$simplesamlphp_auth_path = drupal_get_path('module', 'simplesamlphp_auth');
$js_to_add = array('jquery.ui.widget', 'jquery.ui.datepicker');

jquery_ui_add($js_to_add);
drupal_add_js($simplesamlphp_auth_path.'/scripts/jquery.ui.datepicker-fr.js');
drupal_add_js($simplesamlphp_auth_path.'/scripts/comportementToolbar.js');
drupal_add_js($simplesamlphp_auth_path.'/scripts/jquery.ui.rosseltoolbar.js');

drupal_add_js('
	$(document).ready(function() {
		$("#rtb").rosseltoolbar();
	});
', 'inline');

?>
