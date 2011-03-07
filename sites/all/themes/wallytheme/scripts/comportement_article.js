$(document).ready(function(){ 
	
	//lightbox type inline in articles
	$('#galeria ul').cycle({ 
	    fx:     'scrollHorz', 
	    prev:   '#btn-galeria-prev',
	    next:   '#btn-galeria-next',
		after: onAfter,
		continuous:    0
	});
	function onAfter(curr,next,opts) {
		var caption = 'Image ' + (opts.currSlide + 1) + ' de ' + opts.slideCount;
		$('#galeria .galeria-tools .count-item').html(caption);
		$('#galeria .galeria-legend').html($('#galeria li:eq(' + opts.currSlide +') img').attr('alt'));
	}
	$('#galeria ul li a img').toggle(function() {
		$('#galeria ul').cycle('pause');
		return false;
	},function() {
		$('#galeria ul').cycle('resume');
	});

	//contact form
	$('.contact-form').hide();
	$('a.par-mail').click(function() {
		$('.contact-form').slideToggle();
	});
	
	//$('.mini-galerie a').colorbox({slideshow:true});
	
	$(".gallery:first a[rel^='prettyPhoto']").prettyPhoto({animationSpeed:'slow',theme:'light_square',slideshow:2000, autoplay_slideshow: false});
	$(".gallery:gt(0) a[rel^='prettyPhoto']").prettyPhoto({animationSpeed:'fast',slideshow:10000});
	//alert('coucou');
}); //end dom ready