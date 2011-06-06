document.getElementsByTagName("html")[0].className = "has_js";

$(document).ready(function(){  

	//input search
		$('.replace').focus(function(){
			if (this.value == this.defaultValue) this.value = '';
		}).blur(function(){
			if (this.value == '') this.value = this.defaultValue;
		});
	
	//clickable element
		$('.clickable').click(function () {
			window.location=$(this).find("a").attr("href"); return false;
		});
	
	//scroll fil info
	if ($('.infos').length > 0) {
		var target = $('#overflow-me').get(0);
		$('#nav-infos').localScroll({
			target: target,
			axis: 'xy',
			queue: true,
			duration: 500
		});
		$("#nav-infos li:first").addClass('current');
		$("#nav-infos li a").click(function(){
			$('#nav-infos li').removeClass("current");
			$(this).parents("li").addClass("current");
		});
	}
	
	//brique galeries
	$(".galeria ul a").click(function(){
		var largePath = $(this).attr("href");
		var largeAlt = $(this).attr("title");
		$(this).parents(".galeria").find(".largeImg").attr({ src: largePath, alt: largeAlt });
		return false;
	});
	
	//tricky bloc blog
	if ($('#pourpre').length > 0) {
		var offset = $("#pourpre").offset();
		$('body').append('<div class="tricky"></div>');
		$('.tricky').css('top', offset.top);
		$('.tricky').width(($('body').width() - 980) / 2);
	}
	
	//main event
	if ($('#event').length > 0) {
		$("#event ul a").click(function(){
			var imgPath = $(this).children('img').attr('rel');
			var imgTitle = $(this).children('img').attr('alt');
			var imgUrl = $(this).attr('href');
			
			$('#event .main-event').attr({ href: imgUrl });
			$('#event .main-event').children('img').attr({ src: imgPath });
			$('#event h1 a').text(imgTitle).attr({ href: imgUrl });
			return false;
		});
	}
	
	//pagination
	if( window.location.href.slice(window.location.href.indexOf('?') + 1).indexOf("page") > -1 ){ 
		$('.top-index').addClass('on-bis');
	}

}); //end dom ready