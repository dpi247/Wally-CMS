var mydomain=window.location.href.match(/:\/\/(.[^/]+)/)[1];
	
$.ajax({
	url: "/verif_splash?domain="+mydomain,
	context: document.body,
	success: function(data){
		wSplash(data);
	}
});


function wSplash(data) {
	if(data == 'true')  $('head').append('<script src="/sites/all/themes/wallydemo/scripts/splash.js" type="text/javascript"></script>');
}