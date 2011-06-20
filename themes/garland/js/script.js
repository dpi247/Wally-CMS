$(function(){

	// The big menu
	$("#gen-nav .clearfix li").hover(function(){
		var item = $(this).attr('class');
		$("#display-menu").load('menu/'+item+'.html',function(response, status, xhr){
			if (status == "error") {
				$(this).hide();
				return this;
			}
			$(this).show();
			//hauteur des blocs dans les menus deroulants
			$(".zone-col").equalHeights(200);
		});
	}, function () {
    	$("#display-menu").hide();
	});
	// first trick
	$("#display-menu").hover(function(){
		$(this).show();
	}, function () {
    	$(this).hide();
	});
	
	//Tous les sujets
	$('#accordion').accordion({ autoHeight: false });
	
	//onglets
	$(".almost").tabs();
	$("#fil-info").tabs();
	
	//the big footer
	$(".all-websites").click(function(){
		$("#display-footer").toggle();
		return false;
	});
	$(".close").click(function(){
		$("#display-footer").hide();
		return false;
	});
	
	//journal numerique
	$(".carousel").jCarouselLite({
		btnNext: ".next",
		btnPrev: ".prev"
	});

});