
function diapo(container) {
	
	main = "div#"+container+" div.main_image"; 
	thumb = "div#"+container+" div.image_thumb"; 
	
//	alert("main: "+main); 
//	alert("t: "+thumb); 
	
	//Show Banner
	$(main+" .desc").show(); //Show Banner
	$(main+" .block").animate({ opacity: 0.85 }, 1 ); //Set Opacity

	//Click and Hover events for thumbnail list
	$(thumb+" ul li:first").addClass('active');
	
	$(thumb+" ul li").bind('click', {full: main, t: thumb}, function(event) {
		 																		
		
		
		//Set Variables
		var imgAlt = $(this).find('img').attr("alt"); //Get Alt Tag of Image
		var imgTitle = $(this).find('a').attr("href"); //Get Main Image URL
		var imgDesc = $(this).find('.block').html(); 	//Get HTML of block
		var imgDescHeight = $(main).find('.block').height();	//Calculate height of block	
		if ($(this).is(".active")) {  //If it's already active, then...
			return false; // Don't click through
		} else {
			//Animate the Teaser				
			$(event.data.full+" .block").animate({ opacity: 0, marginBottom: -imgDescHeight }, 250 , function() {
				$(event.data.full+" .block").html(imgDesc).animate({ opacity: 0.85,	marginBottom: "0" }, 250 );
				$(event.data.full+" img").attr({ src: imgTitle , alt: imgAlt});
			});
		}
		
		$(event.data.t+" ul li").removeClass('active'); //Remove class of 'active' on all lists
		$(this).addClass('active');  //add class of 'active' on this list only
		return false;
		
	}) .hover(function(){
		$(this).addClass('hover');
		}, function() {
		$(this).removeClass('hover');
	});

	$(main+" div.desc a.collapse").bind('onclick', {full: main}, function(event) {
																		
		alert(full);																
		$(full+" .block").slideToggle();  //Toggle the description (slide up and down)
		$(full+" div.desc a.collapse").toggleClass("show"); //Toggle the class name of "show" (the hide/show tab)
	});

//Close Function

};
