app_path="http://www.lesoir.be/modules/toolbar/module/";
url_val_nwlt = "http://pdf.lesoir.be/mon_profil/newsletter/validate.php";
proxies_path = "/modules/toolbar/";
var http = null;
if		(window.XMLHttpRequest) // Firefox 
	http = new XMLHttpRequest(); 
else if	(window.ActiveXObject) // Internet Explorer 
	http = new ActiveXObject("Microsoft.XMLHTTP");
else	// XMLHttpRequest non supportÃ© par le navigateur 
   alert("Votre navigateur ne supporte pas les objets XMLHTTPRequest...");



var ross = (function($) {
initToolbar = function(){
	
	$('#toolbar ul li ul').hide();
	$('#toolbar ul li:has(ul)').hover(function(){
		$(this).children('ul').show();
	}, function() {
		$(this).children('ul').hide();
	});
	$('#toolbar ul li:has(ul) a').bind('focus',function(){
		$(this).siblings('ul').show();
	});
	//newsletter
	$("#encart-newsletter").hide();
	$(".show-news").click(function() { 
		$.get("/toolbar/module/getNews.php", { target: window.location.pathname }, function(data) {
			// if there is data, filter it and render it out
				if(data){
					$('#encart-newsletter').html(data);
				} else {
					var errormsg = '<p>Error: could not load the page.</p>';
					$('#encart-newsletter').html(errormsg);
				}
				
			});
	});
	$(".show-news").hover(function() {
			$(".show-news").click();
			$("#encart-newsletter").show();
	});
	$("#toolbar a:not(.show-news)").not("#encart-newsletter a").hover(function() {
		$("#encart-newsletter").hide();
	});
	$("#encart-newsletter").hover(function() {
		$(this).show();
	},function(){
		$(this).hide();
	});;
	
	$("a.pdf").hover(function() {
		$("#une-abo").show();
	});
	$("#toolbar a:not(.pdf)").not("#une-abo a").hover(function() {
		$("#une-abo").hide();
	});
	$("#une-abo").hover(function() {
		$(this).show();
	},function(){
		$(this).hide();
	});
	
	$('#ident ul').hide();
	$('#ident a.disconnected').hover(function(){
	$(this).siblings('ul').show().delay(1000).show();
	}, function(){
	$(this).siblings('ul').delay(1000).hide();
	});
	$('#ident ul').hover(function(){
		$(this).show();
	},function(){
		$(this).hide();
	});

	
};

openConnect = function() {
	
	var idC = "regformOC";	
	if($("#"+idC).length > 0) {
		$dialog = $("#"+idC);
		$dialog.dialog('open');
	}
	else {
		var $dialog;
		//JSON direct sample
	   	$.get("/toolbar/module/getConnect.php", { target: window.location.pathname }, function(data) {
		// if there is data, filter it and render it out
			if(data){
				$dialog=buildDialog($dialog, data, idC);
			} else {
				var errormsg = '<p>Error: could not load the page.</p>';
				$dialog=buildDialog($dialog, errormsg, idC);
			}
		});
	}
};


openRegister = function(site,uid,mail,name) {

	var idL = "regformOR";
	if($("#"+idL).length > 0) {
		$dialog = $("#"+idL);
		$dialog.dialog('open');		
	}
	else {
		var $dialog;		
		//alert("/toolbar/reactions/proxy.php?action=openRegister&site="+site+"&uid="+uid+"&mail="+encodeURIComponent(mail)+"&name="+name);
		//JSON direct sample
	   	$.get("/toolbar/reactions/proxy.php?action=openRegister&site="+site+"&uid="+uid+"&mail="+encodeURIComponent(mail)+"&name="+name, { target: window.location.pathname }, function(data) {
		// if there is data, filter it and render it out
			if(data) {
				$dialog=buildDialog($dialog, data,idL);
			} else {
				var errormsg = '<p>Error: could not load the page.</p>';
				$dialog=buildDialog($dialog, errormsg, idL);
			}
		});
		
		//if(uid != ""){ 
			setDatePickerOnmbirth();
			setAdr();	
			setC();
			comp();
		//}
	}
};


buildDialog = function($dialog, data, id) {
	
	$dialog = $('<div></div>').html(data)
		.attr("id",id)
		.dialog({
			autoOpen: false,
			modal: true
		});
	$dialog.dialog('open');
	return false;
};

setDatePickerOnmbirth = function() {
	if($("#mbirth").length > 0) {
			$.datepicker.setDefaults($.datepicker.regional['fr']);		
			$("#mbirth").datepicker({
					yearRange: '-100:-1',
					regional: 'fr',
					changeMonth: true,
					changeYear: true,
					showOn: 'both',
					buttonImage: app_path.concat('images/elements/calendar.gif'),
					buttonImageOnly: true,
					dateFormat: 'dd/mm/yy'
				});
		$("#mbirth").datepicker("refresh");
	}
	else {
		setTimeout(setDatePickerOnmbirth,100);
	}
};

setAdr = function() {
	
	if(document.getElementById("comp_select")) {
		var sel = document.mydata.comp_select ;
		var tmp = sel.options[sel.selectedIndex].value ;
		var reg = new RegExp("[_]+", "g");
		var tab = tmp.split(reg);
		var ville = document.mydata.mloc ;
		var cp = document.mydata.mcp ;
		var rue = document.mydata.mrue ;
		ville.value = tab[1];
		cp.value = tab[0];
		rue.value = tab[2];	
		document.getElementById("idStreet").value = tab[3];
		document.getElementById("lang").value = tab[4];
		
		sel.attributes['size'].value = 0;
		document.getElementById("adrL").style.display = 'none';
	}
};


setC = function()  {
	if (document.getElementById("optin3")) {
		if (document.getElementById("optin3").checked==true) {
			if(document.getElementById("optin2")) document.getElementById("optin2").checked = true;
			if(document.getElementById("optin1")) document.getElementById("optin1").checked = true;
		}
	}
	else {
		if (document.getElementById("optin2")) {
			if (document.getElementById("optin2").checked==true) {
				if(document.getElementById("optin1")) document.getElementById("optin1").checked = true;
			}
		}
	}
};


setPays = function(zone) {
	$("#pays").load('javascript/'+zone+'.html',{},
		function() { 
		}						
	);	
};


coPr = function(login,password) {
	var dTime = new Date().getTime();
	$("#loading").show();
	$("#mydata").hide();
	$("#titerr").hide();
	$("#fgpwd").html("Merci pour votre patience");
	$("#titerr").html("");

	$.post("/toolbar/module/verifLog.php?cbk=?", { 
		log: login, pass: password, rnd: dTime, target: encodeURI(window.location.href)
	},
	function(data){
		if(data[0]=="ERROR") {
			
			$('#regformOC').html('<div class="p-main-content"><div class="p-wrap"><div class="p-the-content"><h1 id="titerr">'+ data[1] +'</h1><div id="divtochange"><div id="loading">&nbsp;</div><form name="mydata" id="mydata" action="" method="post" onsubmit=\'return coPr(document.getElementById(\"memail\").value, document.getElementById(\"mmdp\").value);\'><input type="hidden" name="form" value="done" /> <div class="row"><label for="memail"><span>*</span> E-mail :</label>	<input type="text" class="form-text" name="memail" id="memail" /></div>	<div class="row"><label for="mmdp"><span>*</span> Mot de passe :</label><input type="password" class="form-text" name="mmdp" id="mmdp" /></div><input type="submit" class="btn-submit" value="je r&eacute;essaie" /></form><div class="row" id="fgpwd"><a href="http://pdf.lesoir.be/liseuseSSO/authentification/forget.php?aff=FULL&amp;backurlPDF='+encodeURIComponent(window.location.href)+'">J\'ai oubli&eacute; mon mot de passe</a></div></div></div><div class="p-bottom">&nbsp;</div></div></div><iframe id="ssol" style="display: none;"></iframe>');
			$("#loading").hide();
		
		}
		else {
			// IF LOGIN AND PASSWORD IS GOOD, WE LOAD THE IFRAME TO REDIRECT THE PAGE
			var url = "/toolbar/module/iframeSSO.php?login=" + encodeURIComponent(login) + "&pwd=" + encodeURIComponent(password) + "&target=" + encodeURIComponent(window.location.href);
			if ($.browser.msie) {
				window.location.replace(url);
			}else
			{
				$("#ssol").attr('src',url);
			}
			$("#fgpwd").html("Rafra&icirc;chir la page si ce message appara&icirc;t");
		}
	}, "json");
 
	//$.postJSON("/toolbar/module/verifLog.php?cbk=?", { 
			//log: login, pass: password, rnd: dTime, target: encodeURI(window.location.href)}, function(data){
		//$("#loading").hide();
		//if(data[0]=="ERROR") {
			
			//$('#regformOC').html('<div class="p-main-content"><div class="p-wrap"><div class="p-the-content"><h1 id="titerr">'+ data[1] +'</h1><div id="divtochange"><div id="loading">&nbsp;</div><form name="mydata" id="mydata" action="" method="post" onsubmit=\'return coPr(document.getElementById(\"memail\").value, document.getElementById(\"mmdp\").value);\'><input type="hidden" name="form" value="done" /> <div class="row"><label for="memail"><span>*</span> E-mail :</label>	<input type="text" class="form-text" name="memail" id="memail" /></div>	<div class="row"><label for="mmdp"><span>*</span> Mot de passe :</label><input type="password" class="form-text" name="mmdp" id="mmdp" /></div><input type="submit" class="btn-submit" value="je r&eacute;essaie" /></form><div class="row"><a href="http://pdf.lesoir.be/liseuseSSO/authentification/forget.php?aff=FULL&amp;backurlPDF='+encodeURIComponent(window.location.href)+'">J\'ai oubli&eacute; mon mot de passe</a></div></div></div><div class="p-bottom">&nbsp;</div></div></div><iframe id="ssol" style="display: none;"></iframe>');
			//$("#loading").hide();
		
		//}
		//else {
			//// IF LOGIN AND PASSWORD IS GOOD, WE LOAD THE IFRAME TO REDIRECT THE PAGE
			//var url = "/toolbar/module/iframeSSO.php?login=" + encodeURIComponent(login) + "&pwd=" + encodeURIComponent(password) + "&target=" + encodeURIComponent(window.location.href);
			//if ($.browser.msie) {
				//window.location.replace(url);
			//}else
			//{
				//$("#ssol").attr('src',url);
			//}
		//}
	//});
	return false;
};

comp = function(pays) {
	if(pays == "") {
		if(document.mydata.mDest.length > 0) {
			for (var i=0; i < document.mydata.mDest.length; i++) {
				if (document.mydata.mDest[i].checked) {
					var where = document.mydata.mDest[i].value;
				}
			}
		}
		else {
			var where = 'belgique';	
		}
	}
	else { if(pays == "BE") pays = "belgique"; }
	
	if(where == "belgique" || pays == "belgique") {
		var ville = document.mydata.mloc.value ;
		var cp = document.mydata.mcp.value ;
		var rue = document.mydata.mrue.value ;
		var numero = document.mydata.mnumero.value ;
		if(!numero) numero = 0;
	
	 	$.get("/toolbar/reactions/proxy.php?action=comp&ville="+ville+"&cp="+cp+"&rue="+rue+"&numero="+numero, '', function(adresses) {
			// if there is data, filter it and render it out
			if(adresses) {
				adresses = eval('(' + adresses + ')');
				var objCount=0;
				for(_obj in adresses) objCount++;
				if(objCount > 0) {
					
					document.getElementById("adrL").style.display = 'block';
					var sel = document.mydata.comp_select ;
					sel.attributes['size'].value = 5;

					for(i=0 ; i < objCount ; i++) {
						var o = new Option(adresses[i][0].city + ": " + adresses[i][0].street,adresses[i][0].zipcode + "_" + adresses[i][0].city + "_" + adresses[i][0].street + "_" + adresses[i][0].idStreet + "_" + adresses[i][0].lang);
						sel.options[i] = o;		
					}
					sel.selectedIndex = 0 ;
				}
				else {
					//var sel = document.mydata.comp_select ;
					//sel.attributes['size'].value = 0;
				}			
			} else {
				
			}
		});
	}
};

crPr = function(site) {
	var data = new Object();
	if(document.mydata.registerOp) {
		data["form"] = document.mydata.form.value;
		data["idStreet"] = document.mydata.idStreet.value;
		data["lang"] = document.mydata.lang.value;
		data["mgenre"] = document.mydata.mgenre.value;
		data["memail"] = document.mydata.memail.value;
		data["mconfemail"] = document.mydata.mconfemail.value;
		data["mmdp"] = document.mydata.mmdp.value;
		data["mconfmdp"] = document.mydata.mconfmdp.value;
		data["mnom"] = document.mydata.mnom.value;
		data["mprenom"] = document.mydata.mprenom.value;
		data["mbirth"] = document.mydata.mbirth.value;
		data["mpays"] = document.mydata.mpays.value;
		data["mcp"] = document.mydata.mcp.value;
		data["mloc"] = document.mydata.mloc.value;
		data["mrue"] = document.mydata.mrue.value;
		data["comp_select"] = document.mydata.comp_select.value;
		data["mnumero"] = document.mydata.mnumero.value;
		data["mboite"] = document.mydata.mboite.value;
		data["mcomplement"] = document.mydata.mcomplement.value;
		if(document.mydata.newsletter1.checked) data["newsletter1"] = document.mydata.newsletter1.value;
		if(document.mydata.optin1.checked) data["optin1"] = document.mydata.optin1.value;
		if(document.mydata.optin3.checked) data["optin3"] = document.mydata.optin3.value;
		data["registerOp"] = document.mydata.registerOp.value;
		data["num_abo"] = document.mydata.num_abo.value;
		data["cpabo"] = document.mydata.cpabo.value;
	}
	
	if(site == "forum") {		
		data["mpseudo"] = document.mydata.mpseudo.value;
		if(document.mydata.charte.checked) data["charte"] = "Y";
		data["site"] = "forum";
		data["sn"] = document.mydata.nom.value;
		data["mail"] = document.mydata.mail.value;
		data["uid"]= document.mydata.uid.value;
	}
	//alert ("/toolbar/reactions/proxy.php?action=verifdata&data="+encodeURIComponent(JSON.stringify(data)));
	$.post("/toolbar/reactions/proxy.php?action=verifdata", { data: encodeURIComponent(JSON.stringify(data)), target: window.location.pathname }, function(data) {
		if(data) {
			var divp = document.getElementById("perror").innerHTML = "";		
			var messages = eval('(' + data + ')');			
			//alert(data);	
			if(messages["ERROR"]=="Y") {
				for ( keyVar in messages ) {
					if(keyVar != "ERROR") {
						var dClass = document.getElementById(keyVar).className;
						document.getElementById(keyVar).className = dClass + " little-error";
						var divp = document.getElementById("perror");
						p = document.createElement("P");
						text = document.createTextNode(messages[keyVar]);
						p.appendChild(text);
						divp.appendChild(p);		
					}
				}
			}
			else {
				document.getElementById("divtochange").innerHTML = '<br /><p style = "text-align: center;" class="message">' 
				+ messages["MESSAGE"] + '</p><a href="" style="width:4em;color:white" class="btn-submit" />Fermer</a>';					
			}	
		}
	});
	
	return false;
};


sNews = function() {
	
	var mail = document.mynews.monmail.value;
	//var check = document.mynews.mde_fla.checked;
	var res = "";
	
	//if(check) {
		var emailRegEx = /^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$/i;
     	if (mail.search(emailRegEx) != -1) {
			
			// INSCRIPTION NEWSLETTERS
			http.open("GET", url_val_nwlt + "?idNews=32_33&mailNews=" + mail + "&valueNews=Y", true);
			http.onreadystatechange=function() {
				if(http.readyState == 4) {
					res = http.responseText;
					if(res == 'ok') document.getElementById("divtochange").innerHTML = '<br /><p style = "text-align: center;" class="message">Votre inscription a bien &eacute;t&eacute; enregistr&eacute;e. </p><a href="javascript:closeNews()" style="width:4em;color:white" class="btn-submit" />Fermer</a>';
					else document.getElementById("divtochange").innerHTML = '<br /><p style = "text-align: center;" class="message">Un probl&egrave;me est survenue, veuillez r&eacute;essayer plus tard. </p><a href="javascript:closeNews()" style="width:4em;color:white" class="btn-submit" />Fermer</a>';
				}
			};
			http.send(null);				
			
		}
		else {
			alert("Veuillez rentrer un adresse e-mail valide. ");
		}
	//}
	//else {
		//alert("Vous devez cocher la case pour vous inscrire. ");
	//}
	
	return false;
};

closeNews = function() {
	
	$("#encart-newsletter").animate({top:"-300px"}, 750); 
};
}) (jQuery);
