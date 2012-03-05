Drupal.behaviors.footLive = function(context) {
	// module football
	$(".foot tr td.dom").wrapInner("<div></div>");
	$(".foot tr td.ext").wrapInner("<div></div>");

	// chargement du module
	var waiting_timer = setInterval(function() {
		if (typeof $("#foot_dl #foot_d").attr("id") != "undefined" && $("#foot_dl #foot_d").length != 0) {
			clearInterval(waiting_timer);
			change_module();
			$.timer(60000, change_module);
		}
	}, 50);

	//menu foot statique
	$("#foot_s .wrap_inner .onglet").not(":first").hide(); 
	$("#foot_s .wrap_inner .onglet:first").addClass("selected"); 
	$("ul.onglets_live a").click(function(){ 
		$("ul.onglets_live a").parents("li").removeClass("selected"); 
		$("#foot_s .wrap_inner .onglet").hide(); 
		$(this.hash).show(); 
		$(this).blur().parents("li").addClass("selected"); 
		return false; 
	});
};

function change_module() {
	$("#foot_dl").load("http://football-regional.sudpresse.be/cache/football0910/LIVE/HOMEPAGE/homepage_sp.html?random"+Math.random(),function() {
		$(".inner-bloc .onglet").not(":first").hide();
		$(".foot tr:even").css("background", "#DBEBF8");
		if (jQuery.browser.msie) {$(".foot tr:even td").css("background", "#DBEBF8");}
		$("ul.onglets_live li:first").addClass("selected");
		$(".inner-bloc .onglet:first").addClass("selected"); 
		$("ul.onglets_live a").click(function(){ 
			$("ul.onglets_live a").parents("li").removeClass("selected"); 
			$(".inner-bloc .onglet").hide(); 
			$(this.hash).show(); 
			$(this).blur().parents("li").addClass("selected"); 
			return false; 
		});
	});
}
