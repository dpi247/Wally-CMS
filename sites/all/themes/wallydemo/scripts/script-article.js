$(function(){
	
	//galerie article
	$('base').remove();
	var target = $('#picture #main .allMedias').get(0);
	$('#picture #main .mini-pagination').localScroll({
		target: target,
		axis: 'xy',
		queue: true,
		duration: 500
	});
	$('#picture #main .wrappAllMedia div:first').addClass('divCurrent');
	$("#picture #main .mini-pagination li:first").addClass('ongletCourant');
	$("#picture #main .mini-pagination li a").click(function() {
		$('#picture #main .mini-pagination li').removeClass("ongletCourant");
		$(this).parents("li").addClass("ongletCourant");
		var myClass = $(this).attr("href");
		myClass = '#picture #main #'+myClass.substr(1);
		$('#picture #main .wrappAllMedia div').removeClass("divCurrent");
		$(myClass).addClass("divCurrent");		
	});
	
	

});