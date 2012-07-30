$(document).ready(function() {
	/*$.each(Drupal.settings.next_prev.boxes, function(index, value) {
		$("body").append("<div id='"+value.div_id+"' class='next_prev' style='display: none;'><img src='/misc/progress.gif' /></div>");
	});*/

	$(".next_prev").each(function() {
		var bloc_id = this.id;

		/*$.ajax({
			type: "GET",
			url: "/cckdestinations/next_prev/"+Drupal.settings.next_prev.boxes[bloc_id].nid+"/"+bloc_id,
			complete: function(data) {
				$("#"+bloc_id).html(data.responseText);
			}
		});*/

		toggledisplay(bloc_id);
		$(window).bind("scroll resize", function(e){
			toggledisplay(bloc_id);
		});
	});
});

function toggledisplay(bloc_id) {
	// Selector of the content's DIV
	var div_selector = Drupal.settings.next_prev.div_selector;
	// This is the height of the bottom of the window :
	// "height of the top of the window" + "height of the window" - "difference between the top of the page and the top of the content's DIV"
	var scrolltop = jQuery(window).scrollTop() + jQuery(window).height() - $(div_selector).offset().top;
	// Height of the content's DIV
	var windowheight = $(div_selector).height();

	// We fix the trigger at 80% of the content's DIV
	var shouldvisible = ((windowheight - scrolltop) >= (0.2 * windowheight)) ? false : true;
	if (shouldvisible && !$("#"+bloc_id).is(":visible")) {
		$("#"+bloc_id).slideDown();
	} else if (!shouldvisible && $("#"+bloc_id).is(":visible")) {
		$("#"+bloc_id).slideUp();
	}
}
