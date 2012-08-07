$(document).ready(function() {
	$('.connectboxopen').click(function(){
        $('#overlay').fadeIn('fast',function(){
            $('#connectbox').animate({'top':'160px'},500);
        });
    });
    $('#connectboxclose').click(function(){
        $('#box').animate({'top':'-200px'},500,function(){
            $('#overlay').fadeOut('fast');
        });
    });
});
