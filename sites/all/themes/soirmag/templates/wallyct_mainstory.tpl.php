<?php

    // FB comment should be exported to a module, with a ADMIN FORM to get the API ID.
    // this module will also implément a theming function to display comment 
    // form (additionnal options - Width / number of posts / ... 
    // 
    
    $path = base_path().path_to_theme();

    $nid = $mainstory->nid; 
    $textarette = $mainstory->field_textarette[0]["value"];
    $foretitle = $textarette." ".$mainstory->field_textforetitle[0]["value"];
    $title = $mainstory->title;
    $subtitle = $mainstory->field_textsubtitle[0]["value"];
    $body = $mainstory->field_textbody[0]["value"];
    $photo_gallery = theme("wallyct_photoobject_slideshow",$node->field_embededobjects_nodes, $node);

    $ariane = theme("soirmag_fil_ariane", $node->field_embededobjects_nodes, $node);
    $share = theme("soirmag_share", $mainstory, $node);

    $persons = theme("wallyct_personslist_detail",$mainstory->field_persons_nodes, $node);
    $free_tags = theme("wallyct_taxotermlist",$mainstory->field_free_tags, $node);
    $classified_tags = theme("wallyct_taxotermlist_tree",$mainstory->field_tags, $node);

    $date = date('l jS \of F Y h:i:s A', $mainstory->changed);
 
?>
			<div id="colonne-article" class="grid_6">
				<div id="article" class="grid_6 alpha omega">
					<div class="fil-ariane clearfix">
						<p>Vous &ecirc;tes ici : <?php print $ariane ?></p>
						<a class="feed" href="javascript:void(0)" title="S'abonner au RSS de la section + SECTION +"><img src="<?php print $path; ?>/mediastore/elements/comment_rss_16.png" width="16" height="16" alt="RSS" /></a>
					</div>

					<div class="inner">
						<h1><?php print $title ?></h1>
            <small><?php print $date; ?></small>
            <?php print $photo_gallery; ?>
						<?php print $body ?>

            <fb:comments xid="<?php print $nid; ?>" numposts="10" width="440"></fb:comments>
            
						<div class="share clearfix">
            <?php print $share; ?>
						</div>
						
						<div class="contact-form">
							<p><strong>Tous les champs sont obligatoires</strong></p>
							<form id="envoyer_ami" action="javascript:void(0)" method="post">
								<fieldset>
									<legend>Civilités</legend>
									<input type="hidden" name="story_envoyer" value="1" />
									<input type="hidden" name="story_do_send" value="1" />
									<input type="hidden" name="story_url" value="javascript:void(0)" />
									<ul class="conaughey">
										<li><label for="nom">Votre nom :</label><input type="text" name="nom" id="nom" class="required" /></li>
										<li><label for="from_email">Votre <span lang="en">E-Mail</span> :</label><input type="text" name="from_email" id="from_email" class="required email" /></li>
										<li><label for="story_title">Objet de l <span lang="en">E-Mail</span>:</label><input type="text" name="story_title" id="story_title" class="required" value="Heath Ledger, le dernier Joker ?" /></li>
										<li><label for="to_email"><span lang="en">E-Mail</span> du destinataire :</label><input type="text" name="to_email" id="to_email" class="required email" /></li>
										<li><input type="submit" value="envoyer" id="submit_friends" name="submit_friends" alt="Envoyer le formulaire" /></li>
									</ul>
								</fieldset>
							</form>
						</div>

					</div>
				</div>
				<div id="article-section" class="grid_6 alpha omega">
					<span class="title-section">Actu Stars</span>
					<a class="more" href="#" title="Voir plus d'Actu Stars">Voir plus d Actus Stars</a>
					<ul class="clearfix story-list">
						<li>
							<h2><a rel="canonical" href="javascript:void(0)">Bruel en guerre avec ses voisins</a></h2>
							<small>Le <span title="20 février 2009">20/02/2009</span> à 12<abbr title="heure">h</abbr>04</small>
							<a rel="canonical" href="javascript:void(0)" class="pos"><img src="<?php print $path; ?>/mediastore/temp/thumb_story1.jpg" width="140" height="93" alt="Bruel en guerre avec ses voisins" /><span class="overlay">&nbsp;</span></a>
							<p>Une clôture de 2 mètres de haut autour de sa propriété d’Isle-sur-la-Sorgue (Vaucluse, sud de la France) pose problème.</p>
							<div class="vote">
								<span class="nb-vote">6</span>
								<span class="label-vote"><a href="javascript:void(0)">Vote!</a></span>
							</div>
						</li>
						<li>
							<h2><a rel="canonical" href="javascript:void(0)">Scarlett Johanson pose en Marilyn Monroe</a></h2>
							<small>Le <span title="20 février 2009">20/02/2009</span> à 11<abbr title="heure">h</abbr>11</small>
							<a rel="canonical" href="javascript:void(0)" class="pos"><img src="<?php print $path; ?>/mediastore/temp/thumb_story2.jpg" width="140" height="93" alt="Scarlett Johanson pose en Marilyn Monroe" /><span class="overlay">&nbsp;</span></a>
							<p>Scarlett Johanson s’est vue transformée en Marylin Monroe lors d’une séance photo pour Dolce and Gabanna.</p>
							<div class="vote">
								<span class="nb-vote">3</span>
								<span class="label-vote"><a href="javascript:void(0)">Vote!</a></span>
							</div>
						</li>
						<li>
							<h2><a rel="canonical" href="javascript:void(0)">Rihanna : la première photo de son agression</a></h2>
							<small>Le <span title="20 février 2009">20/02/2009</span> à 10<abbr title="heure">h</abbr>44</small>
							<a rel="canonical" href="javascript:void(0)" class="pos"><img src="<?php print $path; ?>/mediastore/temp/thumb_story3.jpg" width="140" height="93" alt="Rihanna : la première photo de son agression" /><span class="overlay">&nbsp;</span></a>
							<p>Rihanna avait été attaquée par son petit ami Chris Brown dans la nuit du 7 février dernier. Elle souffrait de nombreuses blessures au visage et n’avait pas pu se présenter à la soirée des Grammy Awards.</p>
							<div class="vote">
								<span class="nb-vote">18</span>
								<span class="label-vote"><a href="javascript:void(0)">Vote!</a></span>
							</div>
						</li>
						<li>
							<h2><a rel="canonical" href="javascript:void(0)">“Slumdog Millionaire :” les deux petits héros assisteront aux Oscars</a></h2>
							<small>Le <span title="20 février 2009">20/02/2009</span> à 10<abbr title="heure">h</abbr>01</small>
							<a rel="canonical" href="javascript:void(0)" class="pos"><img src="<?php print $path; ?>/mediastore/temp/thumb_story4.jpg" width="140" height="93" alt="“Slumdog Millionaire :” les deux petits héros assisteront aux Oscars" /><span class="overlay">&nbsp;</span></a>
							<p>Azhar et Rubina, les héros indiens de “Slumdog Millionaire” auront finalement la chance d’assiter à la cérémonie des Oscars, dimanche 22 février.</p>
							<div class="vote">
								<span class="nb-vote">1</span>
								<span class="label-vote"><a href="javascript:void(0)">Vote!</a></span>
							</div>
						</li>
						<li>
							<h2><a rel="canonical" href="javascript:void(0)">Britney Spears devient magicienne</a></h2>
							<small>Le <span title="19 février 2009">19/02/2009</span> à 17<abbr title="heure">h</abbr>01</small>
							<a rel="canonical" href="javascript:void(0)" class="pos"><img src="<?php print $path; ?>/mediastore/temp/thumb_story5.jpg" width="140" height="93" alt="Britney Spears devient magicienne" /><span class="overlay">&nbsp;</span></a>
							<p>La chanteuse qui n’a pas fait de tourner depuis plus de cinq ans, promet un show spectaculaire. Pour l’occasion, elle va même apprendre à faire des tours de magie. </p>
							<div class="vote">
								<span class="nb-vote">3</span>
								<span class="label-vote"><a href="javascript:void(0)">Vote!</a></span>
							</div>
						</li>
					</ul>
				</div>
			</div>

			<div id="related-article" class="grid_6">
				<h2>articles liés</h2>
				<ul>
					<li><a href="javascript:void(0)">Heath Ledger : les fans se souviennent</a></li>
					<li><a href="javascript:void(0)">Carnage à la crèche : l'ombre du Joker</a></li>
					<li><a href="javascript:void(0)">Heath Ledger, le dernier Joker ?</a></li>
					<li><a href="javascript:void(0)">Heath Ledger, s'est suicidé</a></li>
					<li><a href="javascript:void(0)">Batman, The Dark Knight explose le box-office</a></li>
				</ul>
			</div>

			<div id="persons" class="grid_6">
				<h2>Personnes</h2>
        <?php
            print $persons;
        ?>
			</div>

			<div id="free-tags" class="grid_6">
				<h2>Free Tags</h2>
        <?php
          print $free_tags;
        ?>
			</div>

			<div id="classified-tags" class="grid_6">
				<h2>Classified Tags</h2>
        <?php
          print $classified_tags;
        ?>
			</div>

