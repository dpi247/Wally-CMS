<?php
// $Id: media_embedly.variables.inc,v 1.0 2011/01/07 ODM $

/**
 * @file Contains the variables and defaults used by Media: Embedly.
 */

/**
 * The variable namespace for Media: Embedly.
 */
define('MEDIA_EMBEDLY_NAMESPACE', 'media_embedly__');

/**
 * This is the main URL for Embedly
 */
define('MEDIA_EMBEDLY_MAIN_URL', 'http://www.embed.ly/');

/**
 * This is the URL to the API of Embedly.
 */
define('MEDIA_EMBEDLY_API_INFO', 'http://api.embed.ly/embed');
define('MEDIA_EMBEDLY_API_APPLICATION_URL', 'http://api.embed.ly');

/**
 * This defines the version of the content data array that we serialize
 * in emvideo_kewego_data(). If we change the expected keys of that array,
 * we must increment this value, which will allow older content to be updated
 * to the new version automatically.
 */
define('MEDIA_EMBEDLY_DATA_VERSION', 4);

define('MEDIA_EMBEDLY_STATUS_UPDATE_NONE', 0);
define('MEDIA_EMBEDLY_STATUS_UPDATE_FROM_UNAVAILABLE', 1);
define('MEDIA_EMBEDLY_STATUS_UPDATE_ALWAYS', 2);

define('MEDIA_EMBEDLY_STATUS_UNAVAILABLE', 0x0000);
define('MEDIA_EMBEDLY_STATUS_AVAILABLE', 0x0001);
define('MEDIA_EMBEDLY_STATUS_PRIVATE', 0x0002);



/**
 * Wrapper for variable_get() that uses the Media: Embedly variable registry.
 *
 * @param string $name
 *  The variable name to retrieve. Note that it will be namespaced by
 *  pre-pending MEDIA_EMBEDLY_NAMESPACE, as to avoid variable collisions
 *  with other modules.
 * @param unknown $default
 *  An optional default variable to return if the variable hasn't been set
 *  yet. Note that within this module, all variables should already be set
 *  in the media_embedly_variable_default() function.
 * @return unknown
 *  Returns the stored variable or its default.
 *
 * @see media_embedly_variable_set()
 * @see media_embedly_variable_del()
 * @see media_embedly_variable_default()
 */
function media_embedly_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing still desired.
  if (!isset($default)) {
    $default = media_embedly_variable_default($name);
  }
  // Namespace all variables
  $variable_name = MEDIA_EMBEDLY_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() that uses the Media: Embedly variable registry.
 *
 * @param string $name
 *  The variable name to set. Note that it will be namespaced by
 *  pre-pending MEDIA_EMBEDLY_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 * @param unknown $value
 *  The value for which to set the variable.
 * @return unknown
 *  Returns the stored variable after setting.
 *
 * @see media_embedly_variable_get()
 * @see media_embedly_variable_del()
 * @see media_embedly_variable_default()
 */
function media_embedly_variable_set($name, $value) {
  $variable_name = MEDIA_EMBEDLY_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() that uses the Media: Embedly variable registry.
 *
 * @param string $name
 *  The variable name to delete. Note that it will be namespaced by
 *  pre-pending MEDIA_EMBEDLY_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 *
 * @see media_embedly_variable_get()
 * @see media_embedly_variable_set()
 * @see media_embedly_variable_default()
 */
function media_embedly_variable_del($name) {
  $variable_name = MEDIA_EMBEDLY_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the Media: Embedly namespace.
 *
 * @param string $name
 *  Optional variable name to retrieve the default. Note that it has not yet
 *  been pre-pended with the MEDIA_EMBEDLY_NAMESPACE namespace at this time.
 * @return unknown
 *  The default value of this variable, if it's been set, or NULL, unless
 *  $name is NULL, in which case we return an array of all default values.
 *
 * @see media_embedly_variable_get()
 * @see media_embedly_variable_set()
 * @see media_embedly_variable_del()
 */
function media_embedly_variable_default($name = NULL) {
  static $defaults;

  if (!isset($defaults)) {
    $defaults = array(
      'cron_time' => 10,
      'cron_limit' => 50,
      'video_width' => 300,
      'audio_width' => 300,
      'cache_time' => 1440,
      // Array of the allowed URLs from which the videos may come.
      'allowed_video_url' => 'http://*justin.tv/*
http://*justin.tv/*/b/*
http://*justin.tv/*/w/*
http://qik.com/video/*
http://qik.com/*
http://qik.ly/*
http://*.dailymotion.com/video/*
http://*.dailymotion.com/*/video/*
http://www.collegehumor.com/video:*
http://vids.myspace.com/index.cfm?fuseaction=vids.individual&videoid*
http://www.myspace.com/index.cfm?fuseaction=*&videoid*
http://www.metacafe.com/watch/*
http://www.metacafe.com/w/*
http://blip.tv/file/*
http://*.blip.tv/file/*
http://video.yahoo.com/watch/*/*
http://video.yahoo.com/network/*
http://liveleak.com/view?*
http://www.liveleak.com/view?*
http://animoto.com/play/*
http://dotsub.com/view/*
http://www.worldstarhiphop.com/videos/video*.php?v=*
http://worldstarhiphop.com/videos/video*.php?v=*
http://teachertube.com/viewVideo.php*
http://www.teachertube.com/viewVideo.php*
http://www1.teachertube.com/viewVideo.php*
http://www2.teachertube.com/viewVideo.php*
http://bambuser.com/v/*
http://bambuser.com/channel/*
http://bambuser.com/channel/*/broadcast/*
http://www.schooltube.com/video/*/*
http://bigthink.com/ideas/*
http://bigthink.com/series/*
http://sendables.jibjab.com/view/*
http://sendables.jibjab.com/originals/*
http://www.xtranormal.com/watch/*
http://dipdive.com/media/*
http://dipdive.com/member/*/media/*
http://dipdive.com/v/*
http://*.dipdive.com/media/*
http://*.dipdive.com/v/*
http://v.youku.com/v_show/*.html
http://v.youku.com/v_playlist/*.html
http://www.snotr.com/video/*
http://snotr.com/video/*
http://*yfrog.*/*
http://tweetphoto.com/*
http://www.flickr.com/photos/*
http://flic.kr/*
http://twitpic.com/*
http://www.twitpic.com/*
http://twitpic.com/photos/*
http://www.twitpic.com/photos/*
http://*imgur.com/*
http://*.posterous.com/*
http://post.ly/*
http://twitgoo.com/*
http://i*.photobucket.com/albums/*
http://s*.photobucket.com/albums/*
http://phodroid.com/*/*/*
http://www.mobypicture.com/user/*/view/*
http://moby.to/*
http://xkcd.com/*
http://www.xkcd.com/*
http://imgs.xkcd.com/*
http://www.asofterworld.com/index.php?id=*
http://www.asofterworld.com/*.jpg
http://asofterworld.com/*.jpg
http://www.qwantz.com/index.php?comic=*
http://23hq.com/*/photo/*
http://www.23hq.com/*/photo/*
http://*dribbble.com/shots/*
http://drbl.in/*
http://*.smugmug.com/*
http://*.smugmug.com/*#*
http://emberapp.com/*/images/*
http://emberapp.com/*/images/*/sizes/*
http://emberapp.com/*/collections/*/*
http://emberapp.com/*/categories/*/*/*
http://embr.it/*
http://picasaweb.google.com*/*/*#*
http://picasaweb.google.com*/lh/photo/*
http://picasaweb.google.com*/*/*
http://dailybooth.com/*/*
http://brizzly.com/pic/*
http://pics.brizzly.com/*.jpg
http://img.ly/*
http://www.tinypic.com/view.php*
http://tinypic.com/view.php*
http://www.tinypic.com/player.php*
http://tinypic.com/player.php*
http://www.tinypic.com/r/*/*
http://tinypic.com/r/*/*
http://*.tinypic.com/*.jpg
http://*.tinypic.com/*.png
http://meadd.com/*/*
http://meadd.com/*
http://*.deviantart.com/art/*
http://*.deviantart.com/gallery/*
http://*.deviantart.com/#/*
http://fav.me/*
http://*.deviantart.com
http://*.deviantart.com/gallery
http://*.deviantart.com/*/*.jpg
http://*.deviantart.com/*/*.gif
http://*.deviantart.net/*/*.jpg
http://*.deviantart.net/*/*.gif
http://plixi.com/p/*
http://plixi.com/profile/home/*
http://plixi.com/*
http://www.fotopedia.com/*/*
http://fotopedia.com/*/*
http://photozou.jp/photo/show/*/*
http://photozou.jp/photo/photo_only/*/*
http://instagr.am/p/*
http://skitch.com/*/*/*
http://img.skitch.com/*
https://skitch.com/*/*/*
https://img.skitch.com/*
http://share.ovi.com/media/*/*
http://www.questionablecontent.net/
http://questionablecontent.net/
http://www.questionablecontent.net/view.php*
http://questionablecontent.net/view.php*
http://questionablecontent.net/comics/*.png
http://www.questionablecontent.net/comics/*.png
http://picplz.com/user/*/pic/*/
http://twitrpix.com/*
http://*.twitrpix.com/*
http://www.someecards.com/*/*
http://someecards.com/*/*
http://some.ly/*
http://www.some.ly/*
http://pikchur.com/*
http://achewood.com/*
http://www.achewood.com/*
http://achewood.com/index.php*
http://www.achewood.com/index.php*
http://www.whitehouse.gov/photos-and-video/video/*
http://www.whitehouse.gov/video/*
http://wh.gov/photos-and-video/video/*
http://wh.gov/video/*
http://www.hulu.com/watch*
http://www.hulu.com/w/*
http://hulu.com/watch*
http://hulu.com/w/*
http://*crackle.com/c/*
http://www.fancast.com/*/videos
http://www.funnyordie.com/videos/*
http://www.funnyordie.com/m/*
http://funnyordie.com/videos/*
http://funnyordie.com/m/*
http://www.ted.com/talks/*.html*
http://www.ted.com/talks/lang/*/*.html*
http://www.ted.com/index.php/talks/*.html*
http://www.ted.com/index.php/talks/lang/*/*.html*
http://*nfb.ca/film/*
http://www.thedailyshow.com/watch/*
http://www.thedailyshow.com/full-episodes/*
http://www.thedailyshow.com/collection/*/*/*
http://movies.yahoo.com/movie/*/video/*
http://movies.yahoo.com/movie/*/trailer
http://movies.yahoo.com/movie/*/video
http://www.colbertnation.com/the-colbert-report-collections/*
http://www.colbertnation.com/full-episodes/*
http://www.colbertnation.com/the-colbert-report-videos/*
http://www.comedycentral.com/videos/index.jhtml?*
http://www.theonion.com/video/*
http://theonion.com/video/*
http://wordpress.tv/*/*/*/*/
http://www.traileraddict.com/trailer/*
http://www.traileraddict.com/clip/*
http://www.traileraddict.com/poster/*
http://www.escapistmagazine.com/videos/*
http://www.trailerspy.com/trailer/*/*
http://www.trailerspy.com/trailer/*
http://www.trailerspy.com/view_video.php*
http://www.atom.com/*/*/
http://fora.tv/*/*/*/*
http://www.spike.com/video/*
http://www.gametrailers.com/video/*
http://gametrailers.com/video/*
http://www.koldcast.tv/video/*
http://www.koldcast.tv/#video:*
http://techcrunch.tv/watch*
http://techcrunch.tv/*/watch*
http://mixergy.com/*
http://video.pbs.org/video/*
http://www.zapiks.com/*
http://tv.digg.com/diggnation/*
http://tv.digg.com/diggreel/*
http://tv.digg.com/diggdialogg/*
http://www.trutv.com/video/*
http://www.nzonscreen.com/title/*
http://nzonscreen.com/title/*
http://app.wistia.com/embed/medias/*
https://app.wistia.com/embed/medias/*
http://hungrynation.tv/*/episode/*
http://www.hungrynation.tv/*/episode/*
http://hungrynation.tv/episode/*
http://www.hungrynation.tv/episode/*
http://indymogul.com/*/episode/*
http://www.indymogul.com/*/episode/*
http://indymogul.com/episode/*
http://www.indymogul.com/episode/*
http://channelfrederator.com/*/episode/*
http://www.channelfrederator.com/*/episode/*
http://channelfrederator.com/episode/*
http://www.channelfrederator.com/episode/*
http://tmiweekly.com/*/episode/*
http://www.tmiweekly.com/*/episode/*
http://tmiweekly.com/episode/*
http://www.tmiweekly.com/episode/*
http://99dollarmusicvideos.com/*/episode/*
http://www.99dollarmusicvideos.com/*/episode/*
http://99dollarmusicvideos.com/episode/*
http://www.99dollarmusicvideos.com/episode/*
http://ultrakawaii.com/*/episode/*
http://www.ultrakawaii.com/*/episode/*
http://ultrakawaii.com/episode/*
http://www.ultrakawaii.com/episode/*
http://barelypolitical.com/*/episode/*
http://www.barelypolitical.com/*/episode/*
http://barelypolitical.com/episode/*
http://www.barelypolitical.com/episode/*
http://barelydigital.com/*/episode/*
http://www.barelydigital.com/*/episode/*
http://barelydigital.com/episode/*
http://www.barelydigital.com/episode/*
http://threadbanger.com/*/episode/*
http://www.threadbanger.com/*/episode/*
http://threadbanger.com/episode/*
http://www.threadbanger.com/episode/*
http://vodcars.com/*/episode/*
http://www.vodcars.com/*/episode/*
http://vodcars.com/episode/*
http://www.vodcars.com/episode/*
http://confreaks.net/videos/*
http://www.confreaks.net/videos/*
http://video.allthingsd.com/video/*
http://aniboom.com/animation-video/*
http://www.aniboom.com/animation-video/*
http://clipshack.com/Clip.aspx?*
http://www.clipshack.com/Clip.aspx?*
http://grindtv.com/*/video/*
http://www.grindtv.com/*/video/*
http://ifood.tv/recipe/*
http://ifood.tv/video/*
http://ifood.tv/channel/user/*
http://www.ifood.tv/recipe/*
http://www.ifood.tv/video/*
http://www.ifood.tv/channel/user/*
http://logotv.com/video/*
http://www.logotv.com/video/*
http://lonelyplanet.com/Clip.aspx?*
http://www.lonelyplanet.com/Clip.aspx?*
http://streetfire.net/video/*.htm*
http://www.streetfire.net/video/*.htm*
http://trooptube.tv/videos/*
http://www.trooptube.tv/videos/*
http://www.godtube.com/featured/video/*
http://godtube.com/featured/video/*
http://www.godtube.com/watch/*
http://godtube.com/watch/*
http://www.tangle.com/view_video*
http://mediamatters.org/mmtv/*
http://www.clikthrough.com/theater/video/*
http://espn.go.com/video/clip*
http://espn.go.com/*/story*
http://abcnews.com/*/video/*
http://abcnews.com/video/playerIndex*
http://washingtonpost.com/wp-dyn/*/video/*/*/*/*
http://www.washingtonpost.com/wp-dyn/*/video/*/*/*/*
http://www.boston.com/video*
http://boston.com/video*
http://www.facebook.com/photo.php*
http://www.facebook.com/video/video.php*
http://www.facebook.com/v/*
http://cnbc.com/id/*?*video*
http://www.cnbc.com/id/*?*video*
http://cnbc.com/id/*/play/1/video/*
http://www.cnbc.com/id/*/play/1/video/*
http://cbsnews.com/video/watch/*
http://www.google.com/buzz/*/*/*
http://www.google.com/buzz/*
http://www.google.com/profiles/*
http://google.com/buzz/*/*/*
http://google.com/buzz/*
http://google.com/profiles/*
http://www.cnn.com/video/*
http://edition.cnn.com/video/*
http://money.cnn.com/video/*
http://today.msnbc.msn.com/id/*/vp/*
http://www.msnbc.msn.com/id/*/vp/*
http://www.msnbc.msn.com/id/*/ns/*
http://today.msnbc.msn.com/id/*/ns/*
http://multimedia.foxsports.com/m/video/*/*
http://msn.foxsports.com/video*
http://www.globalpost.com/video/*
http://www.globalpost.com/dispatch/*
http://guardian.co.uk/*/video/*/*/*/*
http://www.guardian.co.uk/*/video/*/*/*/*
http://bravotv.com/*/*/videos/*
http://www.bravotv.com/*/*/videos/*
http://video.nationalgeographic.com/*/*/*.html
http://dsc.discovery.com/videos/*
http://animal.discovery.com/videos/*
http://health.discovery.com/videos/*
http://investigation.discovery.com/videos/*
http://military.discovery.com/videos/*
http://planetgreen.discovery.com/videos/*
http://science.discovery.com/videos/*
http://tlc.discovery.com/videos/*
http://www.last.fm/music/+videos/*
http://www.last.fm/music/+images/*
http://www.last.fm/music/*/_/*
http://www.last.fm/music/*/*
http://www.mixcloud.com/*/*/
http://www.radionomy.com/*/radio/*
http://radionomy.com/*/radio/*
http://www.entertonement.com/clips/*
http://www.rdio.com/#/artist/*/album/*
http://www.rdio.com/artist/*/album/*
http://www.zero-inch.com/*
http://*.bandcamp.com/
http://*.bandcamp.com/track/*
http://*.bandcamp.com/album/*
http://freemusicarchive.org/music/*
http://www.freemusicarchive.org/music/*
http://freemusicarchive.org/curator/*
http://www.freemusicarchive.org/curator/*
http://www.npr.org/*/*/*/*/*
http://www.npr.org/*/*/*/*/*/*
http://www.npr.org/*/*/*/*/*/*/*
http://www.npr.org/templates/story/story.php*
http://huffduffer.com/*/*
http://www.audioboo.fm/boos/*
http://audioboo.fm/boos/*
http://boo.fm/b*
http://www.xiami.com/song/*
http://xiami.com/song/*
http://www.saynow.com/playMsg.html*
http://www.saynow.com/playMsg.html*
http://listen.grooveshark.com/s/*',

    // Array of the allowed URLs from which the photos may come.
      'allowed_audio_url' => 'http://soundcloud.com/*
http://soundcloud.com/*/*
http://soundcloud.com/*/sets/*
http://soundcloud.com/groups/*
http://snd.sc/*
http://www.last.fm/music/*
http://www.last.fm/music/+videos/*
http://www.last.fm/music/+images/*
http://www.last.fm/music/*/_/*
http://www.last.fm/music/*/*
http://www.mixcloud.com/*/*/
http://www.radionomy.com/*/radio/*
http://radionomy.com/*/radio/*
http://www.entertonement.com/clips/*
http://www.rdio.com/#/artist/*/album/*
http://www.rdio.com/artist/*/album/*
http://www.zero-inch.com/*
http://*.bandcamp.com/
http://*.bandcamp.com/track/*
http://*.bandcamp.com/album/*
http://freemusicarchive.org/music/*
http://www.freemusicarchive.org/music/*
http://freemusicarchive.org/curator/*
http://www.freemusicarchive.org/curator/*
http://www.npr.org/*/*/*/*/*
http://www.npr.org/*/*/*/*/*/*
http://www.npr.org/*/*/*/*/*/*/*
http://www.npr.org/templates/story/story.php*
http://huffduffer.com/*/*
http://www.audioboo.fm/boos/*
http://audioboo.fm/boos/*
http://boo.fm/b*
http://www.xiami.com/song/*
http://xiami.com/song/*
http://www.saynow.com/playMsg.html*
http://www.saynow.com/playMsg.html*
http://listen.grooveshark.com/s/*', 
    );
    //http://www.ustream.tv/recorded/*
//http://www.ustream.tv/channel/*
//http://www.ustream.tv/*
    
  }

  if (!isset($name)) {
    return $defaults;
  }

  if (isset($defaults[$name])) {
    return $defaults[$name];
  }
}

/**
 * Return the fully namespace variable name.
 *
 * @param string $name
 *  The variable name to retrieve the namespaced name.
 * @return string
 *  The fully namespace variable name, prepended with
 *  MEDIA_EMBEDLY_NAMESPACE.
 */
function media_embedly_variable_name($name) {
  return MEDIA_EMBEDLY_NAMESPACE . $name;
}
