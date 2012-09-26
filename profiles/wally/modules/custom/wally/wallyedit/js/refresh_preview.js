Drupal.behaviors.refreshPreview = function(context) {
	buildList();
	$("#wallyedit_preview_container").bind("DOMFocusIn", function () {
		buildList();
	});
};

function buildList() {
	$("#edit-select-preview").bind("change", function() {
		$("#loading_preview").hide();

		var temp = "";
		if ($(this).val() != "disabled") {
			if (typeof(Drupal.settings.cache_name) == "object") {
				var cache_name = Drupal.settings.cache_name[0];
			} else {
				var cache_name = Drupal.settings.cache_name;
			}
			temp = "/node/"+cache_name+"/preview2/"+$(this).val();
		}
		$("#prev_iframe").attr("src", temp);
	});
}
