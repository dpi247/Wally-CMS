$(function(){
	
	//galerie article
	$('base').remove();
	var target = $('.allMedias').get(0);
	$('.mini-pagination').localScroll({
		target: target,
		axis: 'xy',
		queue: true,
		duration: 500
	});
	$('.wrappAllMedia div:first').addClass('divCurrent');
	$(".mini-pagination li:first").addClass('ongletCourant');
	$(".mini-pagination li a").click(function() {
		$('.mini-pagination li').removeClass("ongletCourant");
		$(this).parents("li").addClass("ongletCourant");
		var myClass = $(this).attr("href");
		myClass = '#'+myClass.substr(1);
		$('.wrappAllMedia div').removeClass("divCurrent");
		$(myClass).addClass("divCurrent");		
	});

});