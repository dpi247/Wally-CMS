// $Id: supercron.crontab.js,v 1.1 2010/03/18 23:25:59 63reasons Exp $

var clip = null;
var moviePath = '';

Drupal.behaviors.supercron_cron_tab = function(context) {
  // setup single ZeroClipboard object for all our elements
  ZeroClipboard.setMoviePath(Drupal.settings.basePath + Drupal.settings.supercron_module_path + '/js/ZeroClipboard.swf');
  clip = new ZeroClipboard.Client();
  clip.setHandCursor( true );

  // assign a common mouseover function for all elements using jQuery
  $('a.supercron-crontab-command', context).mouseover( function() {
    // set the clip text to our innerHTML
    clip.setText($(this).attr('rel'));

    // reposition the movie over our element
    // or create it if this is the first time
    if (clip.div) {
      clip.receiveEvent('mouseout', null);
      clip.reposition(this);
    }
    else clip.glue(this);

    // gotta force these events due to the Flash movie
    // moving all around.  This insures the CSS effects
    // are properly updated.
    clip.receiveEvent('mouseover', null);
  });
};
