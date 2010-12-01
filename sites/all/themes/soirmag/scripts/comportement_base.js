// unobtrusive javascript

//avoid flickering body
	document.write('<style type="text/css">body{display:none}</style>');

$(document).ready(function(){ 

//avoid flickering body
	$('body').css('display','block');

//navigation
	$('#menu .ss-menu').hide();
	$('#menu .ss-menu ul li:last-child').addClass('noborder');
	$("#menu li").bind('mouseover', function() {
		$('#menu .ss-menu').hide();
		$(this).children('.ss-menu').show();
	}).bind('mouseout', function() {
		$(this).children('.ss-menu').hide();
	});

//input search
	$('#search-text').focus(function(){
		if (this.value == this.defaultValue) this.value = '';
	}).blur(function(){
		if (this.value == '') this.value = this.defaultValue;
	});

//last-babe
	$('.last').hover(function () {
		$(this).css('cursor', 'pointer');
		$(this).css('background', 'white url(mediastore/elements/deg-last-on.gif) repeat-x 0 0');
		$(this).children('.content-desc').children('a').css('color', '#BE0612').css('text-decoration', 'underline');
		$(this).children('.img').children('a').children('span').css('background-image', ' url(mediastore/elements/babe-frame-on.png)');
		}, function () {
			$(this).css('background', 'white url(mediastore/elements/deg-last.gif) repeat-x 0 0');
			$(this).children('.img').children('a').children('span').css('','');
			$(this).children('.content-desc').children('a').css('color', 'black').css('text-decoration', 'none');
			$(this).children('.img').children('a').children('span').css('background-image', ' url(mediastore/elements/babe-frame.png)');
		}
	);

//clickable element
	$('.last, #agenda ul li').click(function () {
		window.location=$(this).find("a").attr("href"); return false;
	});

//add more item
	$('#agenda ul li:odd').css('background-color', '#f3f3f3');
	$('#agenda .more').click (function () {
		if ( $("#agenda .hide").length > 1 ) {
			$('#agenda ul').children('li.hide:first').fadeIn('slow').removeClass('hide');
		}
		else {
			$('#agenda ul').children('li.hide:last').fadeIn('slow').removeClass('hide');
			$(this).fadeOut();
		}
		return false;
	});

// JFL tools
	$('#jfl-tools ul').hide();
	$('a.slide').click(function () {
		$(this).parents('div').children('ul').slideToggle();
		return false;
	});
	$('a.fluidify').live('click', function () {
		$('#grid-system').attr('href', 'css/grid-fluid.css');
		$(this).parents('li').html('<a class="fixify" href="#">mise en page fixe</a>');
		return false;
	});
	$('a.fixify').live('click', function () {
		$('#grid-system').attr('href', 'css/960.css');
		$(this).parents('li').html('<a class="fluidify" href="#">mise en page fluide</a>');
		return false;
	});

}); //end dom ready