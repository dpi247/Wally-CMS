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
  				//buildToolbar(el,data);
  			});
  		}
  	});
  	
  	buildToolbar = function(el,data) {
    	// Add all the element
    	var toolbar = el;
    	$("#rtb").wrapInner("<div id=\'wrap-toolbar\'>");
    	$("#wrap-toolbar").wrapInner("<div id=\'toolbar\'>");
    	$("#toolbar").append("<div id=\'tool-right\'>");
    	
    	for (var item in data.items) {
    		// Create a a element
    		var pos = "";
    		var contents = "";
    		var loc = "";
    		var llk = "";
    		var ppk = "";
    		if(data.items[item].clazz) pos = "class=\'" + data.items[item].clazz + "\' ";
    		if(data.items[item].location == "extern") loc = "target=\'_blank\' ";
    		if(data.items[item].logo) contents = "<img src=\'" + data.items[item].logo + "\' />";
    		else contents = data.items[item].label;
    		
    		llk = data.items[item].ll;
    		ppk = data.items[item].pp;
    		
    		if(!ppk) $("#entries").append("<li id=\'" + data.items[item].id + "\' " + pos + "><a " + pos +" href=\'" + llk + "\' alt=\'" + llk + "\' title=\'" + data.items[item].description + "\' " + loc + " class=\'" + data.items[item].id + "\'>" + contents + "</a></li>");
    		else $("#" + ppk).append("<li class=\'subM "+data.items[item].clazz+"\'><a id=\'" + data.items[item].id + "\' href=\'" + llk + "\' alt=\'" + llk + "\' title=\'" + data.items[item].description + "\' " + loc + ">" + contents + "</a></li>");
    	}
    	
    	$(".subM").wrapAll("<ul />");
    	if(!(typeof data.user === "undefined") && data.user.indexOf("Visiteur") >= 0) data.user = "";
    	
    	//if(data.mode == "unauthenticated") {
    		$("#tool-right").wrapInner("
    			<div id=\'ident\'>
    				<a href=\'javascript:openConnect()\' class=\'connected colorbox\'>'.t('Connection').'</a>
    			</div>
    		" + data.iframe);
    	/*}
    	else {
    		$("#tool-right").wrapInner("
    			<div id=\'ident\'>
    				<a href=\'" + data.profil + "\' class=\'disconnected colorbox\'>
    					'.t('Hello').' <strong>" + data.user + "</strong>
    				</a>
    				<ul>
    					<li><a href=\'" + data.profil + "\'>'.t('My profile').'</a></li>
    					<li><a href=\''.$logout_path.'?url=" + encodeURIComponent(window.location.href) + "\'>'.t('Logout').'</a></li>
    				</ul>
    			</div>
    		");
    	}*/
    }
  });
  ';
  
  return $js;
}