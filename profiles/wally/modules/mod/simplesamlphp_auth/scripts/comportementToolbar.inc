<?php

var_dump(pathinfo(dirname(__FILE__)));
require_once dirname(__FILE__).'/../includes/constants.inc';

function getComportementToolbarJs() {
  $js = '
  $(document).ready( function() {
		if (window.XMLHttpRequest) // Firefox 
			http = new XMLHttpRequest(); 
		else if (window.ActiveXObject) // Internet Explorer 
			http = new ActiveXObject("Microsoft.XMLHTTP");
		else	// XMLHttpRequest not supported by browser 
    	alert("'.t('Your browser doesn\'t support XMLHTTPRequest objects.').'");
		
		$("#ident ul").hide();
		$("#ident a.disconnected").hover(function() {
			$(this).siblings("ul").show().delay(1000).show();
		}, function() {
			$(this).siblings("ul").delay(1000).hide();
		});
		
		$("#ident ul").hover(function(){
			$(this).show();
		}, function(){
			$(this).hide();
		});
	
  	openConnect = function() {
  		var idC = "regformOC";
  		if ($("#"+idC).length > 0) {
  			$dialog = $("#"+idC);
  			$dialog.dialog("open");
  		}
  		else {
  			var $dialog;
  			//JSON direct sample
  			$.get("'.SSO_SIMPLESAMLPHP_AUTH_PATH.'/getConnect.php", { target: window.location.pathname }, function(data) {
  				// if there is data, filter it and render it out
  				if(data){
  					$dialog = buildDialog($dialog, data, idC);
  				} else {
  					var errormsg = "<p>'.t('Error: could not load the page.').'</p>";
  					$dialog = buildDialog($dialog, errormsg, idC);
  				}
  			});
  		}
  	};
  	
  	buildDialog = function($dialog, data, id) {
  		$dialog = $("<div></div>").html(data)
  			.attr("id",id)
  			.dialog({
    			autoOpen: false,
    			modal: true
    		});
    	$dialog.dialog("open");
    	return false;
    };
  });
  ';
  
  return $js;
}
