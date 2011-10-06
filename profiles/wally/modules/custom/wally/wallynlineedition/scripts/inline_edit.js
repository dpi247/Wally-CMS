Drupal.behaviors.wallycontenttypes = function (context) {
	$(".inline_edit").each(function() {
		var cur_id = this.id;
		addClickFieldAction(cur_id);
	});

	function addClickFieldAction(cur_id) {
		$("#" + cur_id + ".not_clicked").click(function () {
			$.ajax({
				type: "GET",
				url: "/inlineedit/"+cur_id+"/"+Drupal.settings.getQ,
				cache: false,
				//data: {
				//	goto_redirect: "/"+Drupal.settings.getQ
				//},
				complete: function(data) {
					$("#" + cur_id).removeClass("not_clicked");
					$("#" + cur_id).addClass("clicked");
					$("#" + cur_id).unbind("click");
					$("#" + cur_id).html(data.responseText);
				}
			});
		});
	}
};
