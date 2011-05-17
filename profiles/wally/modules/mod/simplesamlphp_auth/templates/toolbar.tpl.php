<?php

$simplesamlphp_auth_path = drupal_get_path('module', 'simplesamlphp_auth');

drupal_add_js($simplesamlphp_auth_path.'/scripts/jquery.ui.datepicker-fr.js');
drupal_add_js($simplesamlphp_auth_path.'/scripts/comportementToolbar.js');
drupal_add_js($simplesamlphp_auth_path.'/scripts/jquery.ui.rosseltoolbar.js');

drupal_add_js('
	$(document).ready(function() {
		$("#rtb").rosseltoolbar();
	});
', 'inline');

?>
