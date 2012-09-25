Drupal.behaviors.versionPicker = function (context) {
	$('div.version-picker').click(function () {
		var version = $('input[name="version"]:checked').val();
		$.ajax({
			type: "GET",
			url: "/wallyextra/customcontent/versionpicker/"+version,
			cache: false,
			complete: function(data) {
				$("#wallyextra_version").html(data.responseText);
			}
		});
	});
};
