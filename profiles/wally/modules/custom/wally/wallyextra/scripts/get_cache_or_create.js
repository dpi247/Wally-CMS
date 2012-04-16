$(document).ready(function() {
	$.each(Drupal.settings.wallyextra, function() {
		var datas = this.data;
		var cid = this.data.cid;
		var def_theme = this.data.default_theme;
		$.ajax({
			type: "POST",
			url: "/wallyextra/contenttypesajax",
			cache: false,
			data: datas,
			complete: function(data) {
				$("div.loading_"+cid+"_"+def_theme).html(data.responseText);
			}
		});
	});
});
