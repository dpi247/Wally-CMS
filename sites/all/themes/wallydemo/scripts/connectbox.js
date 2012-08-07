$(document).ready(function() {
	$('.connect-box-open a').removeAttr('href');
	$('.connect-box-open').click(function() {
		$('#connect-overlay').fadeIn('fast',function() {
			$('#connect-box').animate({'top':'300px'}, 100);
		});
	});
	$('#connect-box-close').click(function() {
		$('#connect-box').animate({'top':'-200px'}, 100, function() {
			$('#connect-overlay').fadeOut('fast');
		});
	});
});
