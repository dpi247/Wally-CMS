$(document).ready(function () {	
							
	jQuery(".tab:not(:first)").hide();

	//to fix u know who
	jQuery(".tab:first").show();
	
	jQuery(".htabs a").click(function(){
	  jQuery(".htabs a") .removeClass("active");
      jQuery(this) .addClass("active");
		
		stringref = jQuery(this).attr("href").split('#')[1];

		jQuery('.tab:not(#'+stringref+')').hide();

		if (jQuery.browser.msie && jQuery.browser.version.substr(0,3) == "6.0") {
			jQuery('.tab#' + stringref).show();
		}
		else 
			jQuery('.tab#' + stringref).fadeIn();
		
		return false;
	});
	
	    // collapsible box
     	boxToggle("#commentsdiv");
								
		// Carousel
		$(".imageslides").jCarouselLite({
			btnNext: ".pc-next",
			btnPrev: ".pc-prev",
			visible: 5,
			scroll: 1,
			speed: 200
		});
		
		// dropdown menus
	$('.menu ul li').hover(
		function () {
			//show its submenu
			$('ul', this).slideDown(300);
		}, 
		function () {
			//hide its submenu
			$('ul', this).slideUp(300);			
		}
	);
	$('.menu2 ul li').hover(
		function () {
			//show its submenu
			$('ul', this).slideDown(300);
		}, 
		function () {
			//hide its submenu
			$('ul', this).slideUp(300);			
		}
	);
	    // news ticker
			$('#js-news').ticker({
			speed: 0.10,
			fadeInSpeed: 600,
			titleText: 'Today : '
		});
		
		//tipsy
		$('.socialicons a').tipsy({gravity: 's'});
		$('.icons a').tipsy({gravity: 's'});
		$('.box h2 a').tipsy({gravity: 'w'});
		$('.smallfeed a').tipsy({gravity: 'e'});
});

	Cufon.replace('.header h1');
	Cufon.replace('.todaydate');
	Cufon.replace('.box h2');
	Cufon.replace('.sidebar h2');
	Cufon.replace('.largebox h2');
	Cufon.replace('.toplinks');
	Cufon.replace('.post h2');
	Cufon.replace('.content h1');
	Cufon.replace('.content h2');
	Cufon.replace('.content h3');
	Cufon.replace('.content h4');
	Cufon.replace('.content h5');
	Cufon.replace('.content h6');
	Cufon.replace('.comments-box h3');
	Cufon.replace('h2.search');
	