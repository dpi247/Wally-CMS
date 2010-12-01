// unobtrusive javascript 

$(document).ready(function(){ 

//lightbox type inline in articles
	$('#galeria ul').cycle({ 
	    fx:     'all', 
	    prev:   '#btn-galeria-prev',
	    next:   '#btn-galeria-next',
		continuous:    0
	});

//alternative story list
	$('ul.story-list li:odd').css('background-color', '#f3f3f3');
	
//contact form
	$('.contact-form').hide();
	$('a.par-mail').click(function() {
		$('.contact-form').slideToggle();
	});
	
}); //end dom ready