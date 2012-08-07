$(document).ready(function() {
	$('#topmenu li.sudinfo').after('<li class="connect-box-open">'+Drupal.settings.connect_content+'</li>');
	var wWidth = $(window).width();
	var popupWidth = $('#connect-box').width();
	var popupLeft = (wWidth/2) - (popupWidth/2);
	$('#connect-box').css({'left': popupLeft});
	$('#connect-box #user-login-form').attr('action', '/user?destination='+Drupal.settings.connect_content_destination);

	$('.connect-box-open').click(function() {
		$('#connect-overlay').show();
		$('#connect-overlay').fadeIn('fast', function() {
			$('#connect-box').animate({'top':'300px'}, 1000);
		});
	});
	$('.connect-box-close').click(function() {
		$('#connect-box').fadeOut('fast', function() {
			$('#connect-overlay').fadeOut('fast');
			$('#connect-box').animate({'top':'-200px'}, 10, function() {
				$('#connect-box').fadeIn('fast');
			});
		});
	});
});
