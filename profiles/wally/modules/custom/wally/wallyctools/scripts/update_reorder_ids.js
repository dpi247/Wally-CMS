$(document).ready(function() {
	$(document).bind("mouseup", function(event) {
		var i=1;
		$(".weight-id").each(function () {
			$(this).html(i++);
		});
	});
});
