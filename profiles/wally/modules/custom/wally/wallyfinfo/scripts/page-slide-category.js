$(document).ready(function() {
  $('#infobel').hide();
  $('#infomonde').show();
  $('#infosport').hide();
  $('#infoeco').hide();
  $('#infonet').hide();
  $('#infosciences').hide();
  $('#infocult').hide();
  $('#infoaffiche').hide();
  $('#infoelections').hide();
  $('#infodivers').hide();

  $('li#belgiqueinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infomonde').hide();
    $('#infosport').hide();
    $('#infoeco').hide();
    $('#infonet').hide();
    $('#infosciences').hide();
    $('#infocult').hide();
    $('#infoaffiche').hide();
    $('#infoelections').hide();
    $('#infodivers').hide();
    $('#infobel').fadeIn(2000);
    $('li#belgiqueinfos').addClass ('catselect');
  });

  $('li#mondeinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infosport').hide();
    $('#infoeco').hide();
    $('#infonet').hide();
    $('#infosciences').hide();
    $('#infocult').hide();
    $('#infoaffiche').hide();
    $('#infoelections').hide();
    $('#infodivers').hide();
    $('#infomonde').fadeIn(2000);
    $('li#mondeinfos').addClass ('catselect');
  });

  $('li#sportinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infomonde').hide();
    $('#infoeco').hide();
    $('#infonet').hide();
    $('#infosciences').hide();
    $('#infocult').hide();
    $('#infoaffiche').hide();
    $('#infoelections').hide();
    $('#infodivers').hide();
    $('#infosport').fadeIn(2000);
    $('li#sportinfos').addClass ('catselect');
  });

  $('li#ecoinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infomonde').hide();
    $('#infosport').hide();
    $('#infonet').hide();
    $('#infosciences').hide();
    $('#infocult').hide();
    $('#infoaffiche').hide();
    $('#infoelections').hide();
    $('#infodivers').hide();
    $('#infoeco').fadeIn(2000);
    $('li#ecoinfos').addClass ('catselect');
  });

  $('li#netinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infomonde').hide();
    $('#infosport').hide();
    $('#infoeco').hide();
    $('#infosciences').hide();
    $('#infocult').hide();
    $('#infoaffiche').hide();
    $('#infoelections').hide();
    $('#infodivers').hide();
    $('#infonet').fadeIn(2000);
    $('li#netinfos').addClass ('catselect');
  });

  $('li#sciencesinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infomonde').hide();
    $('#infosport').hide();
    $('#infoeco').hide();
    $('#infonet').hide();
    $('#infocult').hide();
    $('#infoaffiche').hide();
    $('#infoelections').hide();
    $('#infodivers').hide();
    $('#infosciences').fadeIn(2000);
    $('li#sciencesinfos').addClass ('catselect');
  });

  $('li#cultureinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infomonde').hide();
    $('#infosport').hide();
    $('#infoeco').hide();
    $('#infonet').hide();
    $('#infosciences').hide();
    $('#infoaffiche').hide();
    $('#infoelections').hide();
    $('#infodivers').hide();
    $('#infocult').fadeIn(2000);
    $('li#cultureinfos').addClass ('catselect');
  });

  $('li#afficheinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infomonde').hide();
    $('#infosport').hide();
    $('#infoeco').hide();
    $('#infonet').hide();
    $('#infosciences').hide();
    $('#infocult').hide();
    $('#infoelections').hide();
    $('#infodivers').hide();
    $('#infoaffiche').fadeIn(2000);
    $('li#afficheinfos').addClass ('catselect');
  });

  $('li#electionsinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infomonde').hide();
    $('#infosport').hide();
    $('#infoeco').hide();
    $('#infonet').hide();
    $('#infosciences').hide();
    $('#infocult').hide();
    $('#infoaffiche').hide();
    $('#infodivers').hide();
    $('#infoelections').fadeIn(2500);
    $('li#electionsinfos').addClass ('catselect');
  });

  $('li#diversinfos').click(function() {
    $('li').removeClass("catselect");
    $('#infobel').hide();
    $('#infomonde').hide();
    $('#infosport').hide();
    $('#infoeco').hide();
    $('#infonet').hide();
    $('#infosciences').hide();
    $('#infocult').hide();
    $('#infoaffiche').hide();
    $('#infoelections').hide();
    $('#infodivers').fadeIn(2500);
    $('li#diversinfos').addClass ('catselect');
  });
});
