<?php

require_once dirname(dirname(__FILE__)).'/includes/constants.inc';

function getRosseltoolbarJs () {
  $js = '
  $(document).ready( function() {
    // Get the content of the toolbar
  	$.getJSON("'.SSO_SIMPLESAMLPHP_AUTH_PATH.'/toolbarJSON.php?cbk=?", { target: window.location.href }, function(data) {
  		buildToolbar(data);
  	});
  	
  	buildToolbar = function(data) {
    	// Add all the element
    	$("#rtb").html("<div id=\'wrap-toolbar\'></div>");
    	$("#wrap-toolbar").html("<div id=\'tool-right\'></div>");
    	
    	if(data.mode == "unauthenticated") {
    		$("#tool-right").html("<div id=\'ident\'><a href=\'javascript:openConnect()\' class=\'connected colorbox\'>'.t('Connection').'</a></div>" + data.iframe);
    	}
    	else {
    		$("#tool-right").html("<div id=\'ident\'><a href=\'" + data.profil + "\' class=\'disconnected colorbox\'>'.t('Hello').' <strong>" + data.user + "</strong></a><ul><li><a href=\'" + data.profil + "\'>'.t('My profile').'</a></li><li><a href=\''.SSO_LOGOUT_PATH.'?url=" + encodeURIComponent(window.location.href) + "\'>'.t('Logout').'</a></li></ul></div>");
    	}
    }
  });
  ';
  
  return $js;
}