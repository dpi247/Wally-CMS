$(function(){
	var timer;

	$("#special-nav li:nth-child(2)").addClass("second");
	
	$('#special-nav').hover(function(){
		$('#menuprincipal').unbind('hover');
		$('#menuprincipal *').unbind('hover');
		$('#special-nav').show();
	});
	

	//edition orange menu
	$('#menuprincipal').hover(function(){
			window.clearTimeout(timer);
			$('#special-nav').hide();
	}, function() {	
		
		timer = setTimeout(function() {
			$('#special-nav').show();
		}, 3000 );
	}
	);
});
