<?php
  $path = base_path().path_to_theme();
 
  if (isset($node)) {
    $html_head = theme("soirmag_header", $node);
    $leaderboard = theme("soirmag_adds", $node, 'leaderboard.jpg');
  } else {
    $html_head = theme("soirmag_header", NULL);
    $leaderboard = theme("soirmag_adds", NULL, 'leaderboard.jpg');
  }

  $html_menu = theme("soirmag_main_menu", 'primary-links');
   
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
	<head>
    <meta http-equiv="Content-type" content="text/html; charset=UTF-8" />
    <title><?php print $head_title ?></title>
    
    <meta http-equiv="Content-Script-Type" content="text/javascript" />
		<meta http-equiv="Content-Style-Type" content="text/css" />
		<meta http-equiv="Content-language" content="fr" />

    <?php print $head ?>
    <?php print $styles ?>

		<link rel="shortcut icon" type="image/x-icon" href="favicon.ico" />
		<link rel="icon" type="image/png" href="favicon.png" />
		<link rel="apple-touch-icon" href="/iphone_icon.png" />

		<link rel="index" title="Actualité" href="#" />
		<link rel="help" title="Aide" href="#" />
		<link rel="glossary" title="Plan du site" href="#" />
		<!--[if !IE]>image par défaut pour partage<![endif]-->
		<link rel="image_src" href="#" />

		<link rel="alternate" title="titre de la section" href="#" type="application/rss+xml" />

		<!--[if lte IE 6]>
			<script type="text/javascript" src="scripts/supersleight.js"></script>
		<![endif]-->
 
    <?php print $scripts ?>

  </head>

	<body class="actu-star">
		<!--[if IE 6]><div id="IE6"><![endif]-->
		<!--[if IE 7]><div id="IE7"><![endif]-->
		<!--[if (IE) & (!IE 6) & (!IE 7)]><div id="IE"><![endif]-->
		<!--[if !IE]>--><div id="NOTIE"><!--<![endif]-->

    <?php print $html_head; ?>

		<div id="menu">
			<div class="container_12 clearfix">
        <?php print $html_menu; ?>
				<div id="a-la-recherche">
					<form name="search" id="search" action="javascript:void(0)" method="post">
						<label for="search-text">Rechercher :</label>
						<input type="text" name="search-text" id="search-text" value="Rechercher" />
						<input type="submit" name="search-submit" id="search-submit" value="OK" />
					</form>
				</div>
      </div>
    </div>

		<div id="global" class="container_12 clearfix">

			<div id="leaderboard" class="grid_10">
        <?php print $leaderboard; ?>
        <?php if ($messages): print $messages; endif; ?>
        <?php if ($help): print $help; endif; ?>
      </div>

			<div id="rss" class="grid_2">
				<a class="abo" href="javascript:void(0)">Abonnez-vous</a>
				<ul>
					<li class="rss"><a href="javascript:void(0)" title="Abonnez-vous à nos flux RSS"><img src="<?php print $path; ?>/mediastore/elements/RSS.png" width="32" height="32" alt="Tous les flux RSS" /></a></li>
					<li class="twitter"><a href="http://www.twitter.com/Soirmag" title="Suivez-nous sur Twitter"><img src="<?php print $path; ?>/mediastore/elements/twitter.png" width="32" height="32" alt="Suivez-nous sur Twitter" /></a></li>
					<li class="facebook"><a href="javascript:void(0)" title="Rejoignez notre groupe Facebook"><img src="<?php print $path; ?>/mediastore/elements/facebook.png" width="32" height="32" alt="Rejoignez notre groupe Facebook" /></a></li>
				</ul>
			</div>

      <?php print $content ?>
		
		</div>

		<div id="footer">
			<div class="container_12 clearfix">
				<div class="grid_2 prefix_6 return-top">
					<a href="#header" title="Retour en haut de la page">Haut &uarr;</a>
				</div>
				<p class="copyright"><a class="ext" href="http://www.rossel.be" title="Aller sur le site de Rossel.be (fenêtre externe)" >Rossel</a> &amp; <acronym title="Compagnie">Cie</acronym>. <abbr title="Société Anonyme">S.A</abbr>, Soirmag.be, &copy; Bruxelles 2009, <a href="javascript:void(0)">Informations légales</a>. <strong>Publicité :</strong> <a class="ext"  href="http://www.viarossel.be" title="Viarossel (fenêtre externe)">Viarossel.be</a></p>
				<div class="grid_4">
					<span class="strong">Groupe Rossel</span>
					<ul>
						<li><a href="http://www.lecho.be" title="Aller sur le site Lecho.be (fenêtre externe)" class="ext">Lecho.be</a></li>
						<li><a href="http://www.cinenews.be" title="Aller sur le site Cinenews.be (fenêtre externe)" class="ext">Cinenews.be</a></li>
						<li><a href="http://www.ticketnet.be" title="Aller sur le site Ticketnet.be (fenêtre externe)" class="ext">Ticketnet.be</a></li>
						<li><a href="http://www.sillonbelge.be" title="Aller sur le site Sillonbelge.be (fenêtre externe)" class="ext">Lesillonbelge.be</a></li>
						<li><a href="http://www.passiondesmontres.be" title="Aller sur le site Passion des Montres (fenêtre externe)" class="ext">Passion des Montres</a></li>
					</ul>
					<ul>
						<li><a href="http://www.lesoir.be" title="Aller sur le site Lesoir.be (fenêtre externe)" class="ext">Lesoir.be</a></li>
						<li><a href="http://www.sudpresse.be" title="Aller sur le site Sudpresse.be (fenêtre externe)" class="ext">Sudpresse.be</a></li>
						<li><a href="http://www.references.be" title="Aller sur le site References.be (fenêtre externe)" class="ext">References.be</a></li>
						<li><a href="http://www.vlan.be" title="Aller sur le site Vlan.be (fenêtre externe)" class="ext">Vlan.be</a></li>
						<li><a href="http://www.netevents.be" title="Aller sur le site Netevents.be (fenêtre externe)" class="ext">Netevents.be</a></li>
					</ul>
				</div>
				<div class="grid_4">
					<span class="strong">Naviguer</span>
					<ul>
						<li><a href="javascript:void(0)">Galeries photos</a></li>
						<li><a href="javascript:void(0)">Jeux</a></li>
						<li><a href="javascript:void(0)">Services</a></li>
						<li><a href="javascript:void(0)">Liens</a></li>
					</ul>
					<ul>
						<li><a href="javascript:void(0)">Actu soirmag</a></li>
						<li><a href="javascript:void(0)">Actu stars</a></li>
						<li><a href="javascript:void(0)">Actu Royal</a></li>
						<li><a href="javascript:void(0)">TV News</a></li>
						<li><a href="javascript:void(0)">Galeries vidéos</a></li>
					</ul>
				</div>
				<div class="grid_4">
					<span class="strong">Rechercher</span>
					<form name="search-bis" id="search-bis" action="javascript:void(0)" method="post">
						<label for="searchtext">Rechercher :</label>
						<input type="text" name="searchtext" id="searchtext" />
						<input type="submit" name="searchsubmit" id="searchsubmit" value="OK" />
					</form>
					<a href="http://www.cim.be" title="Cim.be (nouvelle fenêtre)"><img class="img_middle" src="<?php print $path; ?>/mediastore/elements/metriweb.gif" width="120" height="34" alt="CIM Metriweb" /></a>
				</div>
			</div>
		</div>

		</div>

    <div id="fb-root"></div>
    <script src="http://connect.facebook.net/en_US/all.js"></script>
    <script>
      FB.init({
        appId  : '163708680330578',
        status : true, // check login status
        cookie : true, // enable cookies to allow the server to access the session
        xfbml  : true  // parse XFBML
      });
    </script>
	</body>

</html>
