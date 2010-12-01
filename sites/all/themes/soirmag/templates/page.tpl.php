<?php
  $path = base_path().path_to_theme();
 
  if (isset($node)) {
    $html_head = theme("soirmag_header", $node);
    $leaderboard = theme("soirmag_adds", $node, 'leaderboard.jpg');
  } else {
    $html_head = theme("soirmag_header", NULL);
    $leaderboard = theme("soirmag_adds", NULL, 'leaderboard.jpg');
  }
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
				<ul class="clearfix">
					<li class="item-01"><a href="javascript:void(0)">Actu soirmag</a></li>
					<li class="item-02 selected"><a href="javascript:void(0)">Actu stars</a></li>
					<li class="item-03"><a href="javascript:void(0)">Actu Royal</a></li>
					<li class="item-04"><a href="javascript:void(0)">TV News</a>
						<div class="ss-menu">
							<div>
								<ul>
									<li><a href="javascript:void(0)">Actu&nbsp;TV</a></li>
									<li><a href="javascript:void(0)">Star&nbsp;Academy&nbsp;9</a></li>
									<li><a href="javascript:void(0)">Nouvelle&nbsp;Star</a></li>
									<li><a href="javascript:void(0)">Secret&nbsp;Story&nbsp;3</a></li>
								</ul>
							</div>
						</div>
					</li>
					<li class="item-05"><a href="javascript:void(0)">Médias</a>
						<div class="ss-menu">
							<div>
								<ul>
									<li><a href="javascript:void(0)">Galeries&nbsp;photos</a></li>
									<li><a href="javascript:void(0)">Galeries&nbsp;vidéos</a></li>
									<li><a href="javascript:void(0)">Photos&nbsp;Babes</a></li>
									<li><a href="javascript:void(0)">Photos&nbsp;People</a></li>
								</ul>
							</div>
						</div>
					</li>
					<li class="item-06"><a href="javascript:void(0)">Jeux</a>
						<div class="ss-menu">
							<div>
								<ul>
									<li><a href="javascript:void(0)">Concours</a></li>
									<li><a href="javascript:void(0)">Sudoku</a></li>
									<li><a href="javascript:void(0)">Jeux&nbsp;Flash</a></li>
								</ul>
							</div>
						</div>
					</li>
					<li class="item-07"><a href="javascript:void(0)">Services</a>
						<div class="ss-menu">
							<div>
								<ul>
									<li><a href="javascript:void(0)">Abonnement</a></li>
									<li><a href="javascript:void(0)">Programmes&nbsp;TV</a></li>
									<li><a href="javascript:void(0)">Horoscope</a></li>
									<li><a href="javascript:void(0)">Rencontres</a></li>
									<li><a href="javascript:void(0)">Contacts</a></li>
								</ul>
							</div>
						</div>					
					</li>
					<li class="item-08"><a href="javascript:void(0)">Liens</a>
						<div class="ss-menu">
							<div>
								<ul>
									<li><a href="javascript:void(0)">Le Soir</a></li>
									<li><a href="javascript:void(0)">Victoire&nbsp;Mag</a></li>
									<li><a href="javascript:void(0)">Les&nbsp;oeuvres&nbsp;du&nbsp;Soir</a></li>
									<li><a href="javascript:void(0)">Références</a></li>
									<li><a href="javascript:void(0)">Vlan</a></li>
									<li><a href="javascript:void(0)">Carchannel</a></li>
									<li><a href="javascript:void(0)">TickenetNet</a></li>
								</ul>
							</div>
						</div>						
					</li>
				</ul>
				
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

			<div id="colonne-milieu" class="grid_2">
				<div id="mur-galerie" class="grid_2 mur alpha omega">
					<h2>galeries</h2>
					<ul>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_1.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_12.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_13.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_14.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_15.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_16.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
					</ul>
				</div>
				<div id="mur-videos" class="grid_2 mur alpha omega">
					<h2>vid&eacute;os</h2>
					<ul>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_2.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_14.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_16.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_12.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_13.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_14.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
					</ul>
				</div>
				<div id="tag-cloud" class="grid_2 alpha omega">
					<h2>nuage de tags</h2>
					<ul>
						<li class="level-7"><a href="javascript:void(0)">Lily Allen</a></li>
						<li class="level-3"><a href="javascript:void(0)">Virginie Efira</a></li>
						<li class="level-2"><a href="javascript:void(0)">Paris Hilton</a></li>
						<li class="level-2"><a href="javascript:void(0)">Rihanna</a></li>
						<li class="level-6"><a href="javascript:void(0)">Chris Brown</a></li>
						<li class="level-3"><a href="javascript:void(0)">Koh Lanta</a></li>
						<li class="level-2"><a href="javascript:void(0)">Delarue</a></li>
						<li class="level-4"><a href="javascript:void(0)">Loana</a></li>
						<li class="level-1"><a href="javascript:void(0)">Rachida Dati</a></li>
						<li class="level-5"><a href="javascript:void(0)">Heath Ledger</a></li>
						<li class="level-1"><a href="javascript:void(0)">Cristiano Ronaldo</a></li>
						<li class="level-4"><a href="javascript:void(0)">Joey Starr</a></li>
					</ul>
				</div>
				<div id="cinema" class="grid_2 alpha omega">
					<h2>&agrave; l'affiche</h2>
					<a href="javascript:void(0)">
						<img src="<?php print $path; ?>/mediastore/temp/affiche.jpg" alt="nom du film" width="120" height="178" />
						<span class="overlay">&nbsp;</span>
					</a>
					<h3><a href="javascript:void(0)">Vendredi 13</a></h3>
				</div>
			<div id="profil-rdv" class="grid_2 alpha mur omega">
				<h2>rencontres</h2>
					<ul>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_5.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_6.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_7.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_8.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_9.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_10.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
					</ul>
			</div>
			<div id="bio-star" class="grid_2 mur alpha omega">
				<h2>bio de stars</h2>
					<ul>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_20.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_21.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_22.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_23.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_24.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
						<li>
							<a href="javascript:void(0)">
								<img src="<?php print $path; ?>/mediastore/temp/thumb_25.jpg" alt="description de l'image" width="54" height="54" />
								<span class="overlay">&nbsp;</span>
							</a>
						</li>
					</ul>
			</div>
			</div>


			<div id="colonne-droite" class="grid_4">

				<div id="last-babe" class="grid_4 last alpha omega">
					<div class="img">
						<a href="javascript:void(0)"><img src="<?php print $path; ?>/mediastore/temp/thumb_3.jpg" alt="Eva Padberg - Babe du jour" width="100" height="60" /><span class="overlay">&nbsp;</span></a>
					</div>
					<div class="content-desc">
						<a href="javascript:void(0)">Eva Padberg</a>
						<p>9 images/1889 consultation(s)</p>
					</div>
				</div>


				<div id="pub" class="grid_4 imu alpha omega">
					<img src="<?php print $path; ?>/mediastore/temp/imu.gif" width="300" height="250" alt="imu" />
				</div>




				<div id="bloc-top-5" class="grid_4 alpha omega">
					<h2>les plus recommandés</h2>
					<ol>
						<li><span><a href="javascript:void(0)">Virginie Efira et Patrick Ridremont : un divorce en live !</a></span></li>
						<li><span><a href="javascript:void(0)">Chris Brown : il culpabilise pour Rihanna</a></span></li>
						<li><span><a href="javascript:void(0)">Loana : de bonnes nouvelles</a></span></li>
						<li><span><a href="javascript:void(0)">Amy Winehouse emmenée d'urgence à l'hôpital !</a></span></li>
						<li><span><a href="javascript:void(0)">Kevin Costner, papa pour la 6ème fois !</a></span></li>
					</ol>
				</div>
				<div class="grid_4 auto-promo alpha omega">
					<ul>
						<li id="promo-horoscope"><a href="javascript:void(0)"><span>Horoscope <small>(par E. Teissier)</small></span></a></li>
						<li id="promo-meteo"><a href="javascript:void(0)"><span>Météo <small>(semaine, plage, neige)</small></span></a></li>
						<li id="promo-jeux"><a href="javascript:void(0)"><span>Jeux <small>(Sudoku, Flash, etc.)</small></span></a></li>
					</ul>
				</div>

				<div id="carchannel" class="grid_4 partenaire alpha omega">
					<h2>Carchannel</h2>
					<div class="thumb-img">
						<a href="javascript:void(0)" title="Renault : la phase 2 de la Clio III"><img src="<?php print $path; ?>/mediastore/temp/thumb_x3.jpg" width="135" height="76" alt="Renault : la phase 2 de la Clio III" /><span class="overlay">&nbsp;</span></a>
					</div>
					<div class="thumb-img">
						<a href="javascript:void(0)" title="Mazda3 i-stop et MPS à Genève, première mondiale"><img src="<?php print $path; ?>/mediastore/temp/thumb_x4.jpg" width="135" height="76" alt="Mazda3 i-stop et MPS à Genève, première mondiale" /><span class="overlay">&nbsp;</span></a>
					</div>
					<ol>
						<li><a href="javascript:void(0)">Mini Cooper cabrio L'esprit ouvert</a></li>
						<li><a href="javascript:void(0)">Citroën Concept DS Inside : une première mondiale</a></li>
						<li><a href="javascript:void(0)">Hyundai ix55 Décalage horaire</a></li>
					</ol>
				</div>

				<div id="moi-jeux" class="grid_4 partenaire alpha omega">
					<h2>Jeux videos</h2>
					<div class="thumb-img">
						<a href="javascript:void(0)" title="Enorme succès pour la démo de Halo Wars"><img src="<?php print $path; ?>/mediastore/temp/thumb_x1.jpg" width="135" height="76" alt="Enorme succès pour la démo de Halo Wars" /><span class="overlay">&nbsp;</span></a>
					</div>
					<div class="thumb-img">
						<a href="javascript:void(0)" title="Les premières images de God of War 3 !"><img src="<?php print $path; ?>/mediastore/temp/thumb_x2.jpg" width="135" height="76" alt="Les premières images de God of War 3 !" /><span class="overlay">&nbsp;</span></a>
					</div>
					<ol>
						<li><a href="javascript:void(0)">La PSP passe le cap des 50 millions !</a></li>
						<li><a href="javascript:void(0)">Deux nouveaux Battlefield annoncés !</a></li>
						<li><a href="javascript:void(0)">Deux nouveaux Guitar Hero en 2009.</a></li>
					</ol>
				</div>

				<div id="agenda" class="grid_4 partenaire alpha omega">
					<h2>Agenda</h2>
					<a class="clearfix more" href="#" title="Voir plus d'événements">Plus</a>
					<ul class="clearfix">
						<li><a href="javascript:void(0)"><img src="<?php print $path; ?>/mediastore/temp/thumb60.jpg" alt="titre de l'evenement" width="60" height="60" /></a><span>Vendredi 20/02</span><br /><a href="javascript:void(0)">Approche&hellip;</a><br /><span class="lieu">Théâtre de Joli-Bois </span></li>
						<li><a href="javascript:void(0)"><img src="<?php print $path; ?>/mediastore/temp/thumb61.jpg" alt="titre de l'evenement" width="60" height="60" /></a><span>Vendredi 20/02</span><br /><a href="javascript:void(0)">Le Championnat de Belgique d'Air Guitar</a><br /><span class="lieu">L'Escalier</span></li>
						<li class="hide"><a href="javascript:void(0)"><img src="<?php print $path; ?>/mediastore/temp/thumb62.jpg" alt="titre de l'evenement" width="60" height="60" /></a><span>Vendredi 20/02</span><br /><a href="javascript:void(0)">Generation '80 Remix - Carnaval Edition</a><br /><span class="lieu">You Night Club</span></li>
						<li class="hide"><a href="javascript:void(0)"><img src="<?php print $path; ?>/mediastore/temp/thumb63.jpg" alt="titre de l'evenement" width="60" height="60" /></a><span>Vendredi 20/02</span><br /><a href="javascript:void(0)">Kardes Türküler</a><br /><span class="lieu">Théâtre Saint-Michel</span></li>
						<li class="hide"><a href="javascript:void(0)"><img src="<?php print $path; ?>/mediastore/temp/thumb64.jpg" alt="titre de l'evenement" width="60" height="60" /></a><span>Vendredi 20/02</span><br /><a href="javascript:void(0)">Pretty Women - Venise</a><br /><span class="lieu">Fiesta Club</span></li>
					</ul>
				</div>

				<div id="vlan" class="grid_4 partenaire alpha omega">
					<img src="<?php print $path; ?>/mediastore/temp/vlan.gif" alt="vlan" width="300" />
				</div>
				
			</div>
		
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
