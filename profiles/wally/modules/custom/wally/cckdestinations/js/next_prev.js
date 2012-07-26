var nextprevvisible = new Array();
var nextprevshouldvisible = new Array();

$(document).ready(function() {
	$.each(Drupal.settings.next_prev, function(index, value) {
		$("body").append("<div id='"+value.div_id+"' class='next_prev' style='display: none;'><img src='/misc/progress.gif' /></div>");
	});

	$(".next_prev").each(function() {
		var bloc_id = this.id;

		$.ajax({
			type: "GET",
			url: "/cckdestinations/next_prev/"+Drupal.settings.next_prev[bloc_id].nid+"/"+bloc_id,
			complete: function(data) {
				$("#"+bloc_id).html(data.responseText);
			}
		});

		nextprevvisible[bloc_id] = false;
		nextprevshouldvisible[bloc_id] = false;

		toggledisplay(bloc_id);
		$(window).bind("scroll resize", function(e){
			toggledisplay(bloc_id);
		});
	});
});

function toggledisplay(bloc_id) {
	var scrolltop = jQuery(window).scrollTop();
	var windowheight = $(document).height() - jQuery(window).height();

	nextprevshouldvisible[bloc_id] = ((windowheight - scrolltop) >= (0.2 * windowheight)) ? false : true;
	if (nextprevshouldvisible[bloc_id] && !nextprevvisible[bloc_id]) {
		$("#"+bloc_id).slideDown();
		nextprevvisible[bloc_id] = true;
	} else if (!nextprevshouldvisible[bloc_id] && nextprevvisible[bloc_id]) {
		$("#"+bloc_id).slideUp();
		nextprevvisible[bloc_id] = false;
	}
}
