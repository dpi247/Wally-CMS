<?php

require_once dirname(dirname(__FILE__)).'/includes/constants.inc';

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
  		var $dialog;
  		//JSON direct sample
  		$.get("'.SSO_SIMPLESAMLPHP_AUTH_PATH.'/getConnect.php", { target: window.location.pathname }, function(data) {
  			// if there is data, filter it and render it out
  			if(data){
  				$dialog = $("#ident").html(data).attr("id",idC);
  			}
  		});
  	};
    
    coPr = function(login,password) {
    	var dTime = new Date().getTime();
    	$("#loading").show();
    	$("#mydata").hide();
    	$("#titerr").hide();
    	$("#fgpwd").html("Merci pour votre patience");
    	$("#titerr").html("");
    
    	$.post("'.SSO_SIMPLESAMLPHP_AUTH_PATH.'/verifLog.php?cbk=?", { 
    		log: login, pass: password, rnd: dTime, target: encodeURI(window.location.href)
    	},
    	function(data){
    		if(data[0]=="ERROR") {
    			$("#regformOC").html("<div class=\'p-main-content\'><div class=\'p-wrap\'><div class=\'p-the-content\'><h1 id=\'titerr\'>"+ data[1] +"</h1><div id=\'divtochange\'><div id=\'loading\'>&nbsp;</div><form name=\'mydata\' id=\'mydata\' action=\'\' method=\'post\' onsubmit=\'return coPr(document.getElementById(\"memail\").value, document.getElementById(\"mmdp\").value);\'><input type=\'hidden\' name=\'form\' value=\'done\' /> <div class=\'row\'><label for=\'memail\'><span>*</span> E-mail :</label>	<input type=\'text\' class=\'form-text\' name=\'memail\' id=\'memail\' /></div>	<div class=\'row\'><label for=\'mmdp\'><span>*</span> Mot de passe :</label><input type=\'password\' class=\'form-text\' name=\'mmdp\' id=\'mmdp\' /></div><input type=\'submit\' class=\'btn-submit\' value=\'je r&eacute;essaie\' /></form><div class=\'row\' id=\'fgpwd\'><a href=\''.SSO_FORGET.'?aff=FULL&amp;backurlPDF="+encodeURIComponent(window.location.href)+"\'>J\'ai oubli&eacute; mon mot de passe</a></div></div></div><div class=\'p-bottom\'>&nbsp;</div></div></div><iframe id=\'ssol\' style=\'display: none;\'></iframe>");
    			$("#loading").hide();
    		}
    		else {
    			// IF LOGIN AND PASSWORD IS GOOD, WE LOAD THE IFRAME TO REDIRECT THE PAGE
    			var url = "'.SSO_SIMPLESAMLPHP_AUTH_PATH.'/iframeSSO.php?login=" + encodeURIComponent(login) + "&pwd=" + encodeURIComponent(password) + "&target=" + encodeURIComponent(window.location.href);
    			if ($.browser.msie) {
    				window.location.replace(url);
    			} else {
    				$("#ssol").attr("src",url);
    			}
    			$("#fgpwd").html("Rafra&icirc;chir la page si ce message appara&icirc;t");
    		}
    	}, "json");
     
    	return false;
    };
  });
  ';
  
  return $js;
}
