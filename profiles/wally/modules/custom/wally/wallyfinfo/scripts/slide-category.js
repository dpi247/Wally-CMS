$(document).ready(function() {
  $('#carmonde').show();
  $('#carsport').hide();
  $('#careco').hide();
  $('#carnet').hide();
  $('#carsciences').hide();
  $('#carcult').hide();
  $('#caraffiche').hide();
  $('#carelections').hide();
  $('#cardivers').hide();
  $('#carbel').hide();

  $('li#belgique').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#carsport').hide();
    $('#careco').hide();
    $('#carnet').hide();
    $('#carsciences').hide();
    $('#carcult').hide();
    $('#caraffiche').hide();
    $('#carelections').hide();
    $('#cardivers').hide();
    $('#carbel').fadeIn("slow");
    $('li#belgique').addClass ('packcatselect');
  });

  $('li#monde').click(function() {
    $('li').removeClass('packcatselect');
    $('#carsport').hide();
    $('#careco').hide();
    $('#carnet').hide();
    $('#carsciences').hide();
    $('#carcult').hide();
    $('#caraffiche').hide();
    $('#carelections').hide();
    $('#cardivers').hide();
    $('#carbel').hide();
    $('#carmonde').fadeIn("slow");
    $('li#monde').addClass ('packcatselect');
  });

  $('li#sport').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#careco').hide();
    $('#carnet').hide();
    $('#carsciences').hide();
    $('#carcult').hide();
    $('#caraffiche').hide();
    $('#carelections').hide();
    $('#cardivers').hide();
    $('#carbel').hide();
    $('#carsport').fadeIn("slow");
    $('li#sport').addClass ('packcatselect');
  });

  $('li#eco').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#carsport').hide();
    $('#carnet').hide();
    $('#carsciences').hide();
    $('#carcult').hide();
    $('#caraffiche').hide();
    $('#carelections').hide();
    $('#cardivers').hide();
    $('#carbel').hide();
    $('#careco').fadeIn("slow");
    $('li#eco').addClass ('packcatselect');
  });

  $('li#net').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#carsport').hide();
    $('#careco').hide();
    $('#carsciences').hide();
    $('#carcult').hide();
    $('#caraffiche').hide();
    $('#carelections').hide();
    $('#cardivers').hide();
    $('#carbel').hide();
    $('#carnet').fadeIn("slow");
    $('li#net').addClass ('packcatselect');
  });

  $('li#sciences').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#carsport').hide();
    $('#careco').hide();
    $('#carnet').hide();
    $('#carcult').hide();
    $('#caraffiche').hide();
    $('#carelections').hide();
    $('#cardivers').hide();
    $('#carbel').hide();
    $('#carsciences').fadeIn("slow");
    $('li#sciences').addClass ('packcatselect');
  });

  $('li#culture').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#carsport').hide();
    $('#careco').hide();
    $('#carnet').hide();
    $('#carsciences').hide();
    $('#caraffiche').hide();
    $('#carelections').hide();
    $('#cardivers').hide();
    $('#carbel').hide();
    $('#carcult').fadeIn("slow");
    $('li#culture').addClass ('packcatselect');
  });

  $('li#affiche').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#carsport').hide();
    $('#careco').hide();
    $('#carnet').hide();
    $('#carsciences').hide();
    $('#carcult').hide();
    $('#carelections').hide();
    $('#cardivers').hide();
    $('#carbel').hide();
    $('#caraffiche').fadeIn("slow");
    $('li#affiche').addClass ('packcatselect');
  });

  $('li#elections').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#carsport').hide();
    $('#careco').hide();
    $('#carnet').hide();
    $('#carsciences').hide();
    $('#carcult').hide();
    $('#caraffiche').hide();
    $('#cardivers').hide();
    $('#carbel').hide();
    $('#carelections').fadeIn("slow");
    $('li#elections').addClass ('packcatselect');
  });

  $('li#divers').click(function() {
    $('li').removeClass('packcatselect');
    $('#carmonde').hide();
    $('#carsport').hide();
    $('#careco').hide();
    $('#carnet').hide();
    $('#carsciences').hide();
    $('#carcult').hide();
    $('#caraffiche').hide();
    $('#carelections').hide();
    $('#carbel').hide();
    $('#cardivers').fadeIn("slow");
    $('li#divers').addClass ('packcatselect');
  });
});
