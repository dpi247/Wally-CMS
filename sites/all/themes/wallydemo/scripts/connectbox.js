$(document).ready(function() {
	$('#topmenu li.sudinfo').after('<li class="connect-box-open">'+Drupal.settings.connect_content+'</li>');

	$('.connect-box-open').click(function() {
		$('#connect-overlay').fadeIn('fast',function() {
			$('#connect-box').animate({'top':'300px'}, 1000);
		});
	});
	$('.connect-box-close').click(function() {
		$('#connect-box').fadeOut('fast');
		$('#connect-overlay').fadeOut('fast');
	});
});
