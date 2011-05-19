<?php

function getRosseltoolbarJs ($app_path = 'profiles/wally/modules/mod/simplesamlphp_auth', $logout_path = '/simplesaml/sso_logout.php') {
  $js = '
  $(document).ready( function() {
    $.widget("ui.rosseltoolbar", {
  		options: {
  			location: "top",
  			type: "nologo"
  		},
  		
  		_create: function() {
  			var self = this,
  					o = self.options,
  					el = self.element;
  			
  			// Get the content of the toolbar
  			$.getJSON("'.$app_path.'/toolbarJSON.php?cbk=?", { target: window.location.href }, function(data) {
  				this.data = data;
  				buildToolbar(el,data);
  			});
  		}
  	});
  	
  	buildToolbar = function(el,data) {
    	// Add all the element
    	var toolbar = el;
    	$("#rtb").wrapInner("<div id=\'wrap-toolbar\'>");
    	$("#wrap-toolbar").wrapInner("<div id=\'toolbar\'>");
    	$("#toolbar").append("<div id=\'tool-right\'>");
    	
    	if(data.mode == "unauthenticated") {
    		$("#tool-right").wrapInner("<div id=\'ident\'><a href=\'javascript:openConnect()\' class=\'connected colorbox\'>'.t('Connection').'</a></div>" + data.iframe);
    	}
    	else {
    		$("#tool-right").wrapInner("<div id=\'ident\'><a href=\'" + data.profil + "\' class=\'disconnected colorbox\'>'.t('Hello').' <strong>" + data.user + "</strong></a><ul><li><a href=\'" + data.profil + "\'>'.t('My profile').'</a></li><li><a href=\''.$logout_path.'?url=" + encodeURIComponent(window.location.href) + "\'>'.t('Logout').'</a></li></ul></div>");
    	}
    }
  });
  ';
  
  return $js;
}