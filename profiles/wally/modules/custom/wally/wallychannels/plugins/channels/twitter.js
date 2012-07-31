Drupal.behaviors.twitterPreview = function(context) {
	$(".channel-twitter div.form-option").each(function() {
		$.ajax({
			type: "GET",
			url: "/wallychannels/get-settings/twitter/"+this.id,
			complete: function(data) {
				var settings = data.responseText;
				if (settings != "") {
				console.log(settings);
				}
			}
		});
	});
};
