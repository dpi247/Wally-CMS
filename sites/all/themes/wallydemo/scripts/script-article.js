$(function(){
	
	//galerie article
	$('base').remove();
	var target = $('#picture .allMedias').get(0);
	$('#picture .mini-pagination').localScroll({
		target: target,
		axis: 'xy',
		queue: true,
		duration: 500
	});
	$('#picture .wrappAllMedia div:first').addClass('divCurrent');
	$("#picture .mini-pagination li:first").addClass('ongletCourant');
	$("#picture .mini-pagination li a").click(function() {
		$('#picture .mini-pagination li').removeClass("ongletCourant");
		$(this).parents("li").addClass("ongletCourant");
		var myClass = $(this).attr("href");
		myClass = '#picture #'+myClass.substr(1);
		$('#picture .wrappAllMedia div').removeClass("divCurrent");
		$(myClass).addClass("divCurrent");		
	});
	
	

});