Drupal.behaviors.esi = function(context) {
  $(".esi-ajax:not(.esi-ajax-processed)").each(function() {
    $(this).addClass('esi-ajax-processed');
    var css_id = $(this).attr('id');
    if (Drupal.settings.esi[css_id] == 'undefined') {
      return;
    }
    var web_src = Drupal.settings.esi[css_id];

    // Replace CACHE=ROLE with CACHE=role_hash.
    // 'SESS' is hard coded in conf_init().
    // 'R' is hard coded in the ESI module.
    var role_hash = esi_get_hash('RSESS');
    var suffix = '/CACHE=ROLE';
    if (role_hash.length != 0 && web_src.indexOf(suffix, web_src.length - suffix.length) !== -1) {
      web_src = web_src.replace(suffix, '/CACHE=' + role_hash);
    }

    // Replace CACHE=USER with CACHE=user_hash.
    // 'SESS' is hard coded in conf_init().
    // 'U' is hard coded in the ESI module.
    var user_hash = esi_get_hash('USESS');
    var suffix = '/CACHE=USER';
    if (user_hash.length != 0 && web_src.indexOf(suffix, web_src.length - suffix.length) !== -1) {
      web_src = web_src.replace(suffix, '/CACHE=' + user_hash);
    }

    $.get(web_src, function(data) {
      $('#' + css_id).replaceWith(data);
      Drupal.attachBehaviors(data);
    });
  });
}

function esi_get_hash(prefix) {
  var all_cookies = document.cookie.split(';');
  for (var i=0; i < all_cookies.length; i++) {
    if (all_cookies[i].indexOf(prefix) === 1) {
      var values = all_cookies[i].split('=');
      return values[1];
    }
  }
  return '';
}
