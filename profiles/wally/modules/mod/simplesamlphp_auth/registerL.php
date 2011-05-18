<?php

	include("config.inc.php");

	$f = $_GET["site"];
	$n = $_GET["name"];
	$m = $_GET["mail"];
	$u = $_GET["uid"];

?>
<div class="p-main-content">
	<div class="p-wrap">
		<div class="p-the-content">

			<h1>Créer mon profil</h1>
			<p id="intro">Bienvenue au Soir. Inscrivez-vous et acc&eacute;dez &agrave; l'espace débats du Soir.be. 
			Vous pourrez r&eacute;agir aux articles, vous exprimer dans le forum et participer aux concours. 
			Profitez &eacute;galement des avantages et offres li&eacute;s au club LE SOIR, si vous &ecirc;tes abonn&eacute;.</p>
			<br />
			
			<div id="divtochange">
				
				<form name="mydata" id="mydata" action="" method="post" onsubmit="return crPr('forum');">
				<?php if($u == '' && $mail == '' && $n == '') { ?>
				<input type="hidden" name="form" value="done" /> 
				<input type="hidden" name="idStreet" id="idStreet" /> 
				<input type="hidden" name="lang" id="lang" /> 
				<input type="hidden" name="registerOp" value="FULL" /> 
				
				<div class="row">
					<label for="memail"><span>*</span> E-mail :</label>
					<input type="text" class="form-text" name="memail" id="memail" />
				</div>
				<div class="row">
					<label for="mconfemail"><span>*</span> Confirmation de l'E-mail :</label>
					<input type="text" class="form-text" name="mconfemail" id="mconfemail" />
				</div>
				<div class="row">
					<label for="mmdp"><span>*</span> Mot de passe (8 car min.) :</label>
					<input type="password" class="form-text" name="mmdp" id="mmdp" />
				</div>
				<div class="row">
					<label for="mconfmdp"><span>*</span> Confirmation de mot de passe :</label>
					<input type="password" class="form-text" name="mconfmdp" id="mconfmdp" />
				</div>
				<div class="separator"><span>&nbsp;</span></div>	
				<div class="row">
					<label for="civ"><span>*</span>Civilit&eacute; :</label>
						<select class="form-select" name="mgenre" id="mgenre">
							<option value=""></option>
							<option value="Mlle"<?php if($genre == "Mlle") echo "selected"; ?>>Mademoiselle</option>
							<option value="Mme"<?php if($genre == "Mme") echo "selected"; ?>>Madame</option>
							<option value="M." <?php if($genre == "M.") echo "selected"; ?>>Monsieur</option>
							
						</select>
				</div>
				<div class="row">
					<label for="mnom"><span>*</span> Nom :</label>
					<input type="text" class="form-text" name="mnom" id="mnom" />
				</div>						
				<div class="row">
					<label for="mprenom"><span>*</span> Prénom :</label>
					<input type="text" class="form-text" name="mprenom" id="mprenom" />
				</div>				
				<div class="row">
					<label for="mpays"><span>*</span> Pays :</label>
					<select class="form-select" name="mpays" id="mpays">
						<?php foreach ($country_list as $code => $country){
							echo "<option value=\"".$code."\"";
							echo">".$country."</option>\n";
						}?>	
					</select>
				</div>
				<div class="row">
					<label for="mcp"><span>*</span> Code Postal :</label>
					<input type="text" class="form-text" name="mcp" id="mcp" />							
				</div>
				<div class="row">
					<label for="mloc"><span>*</span>Localité :</label>
					<input type="text" class="form-text" name="mloc" id="mloc" />
				</div>
				<div class="row">
					<label for="mrue"><span>*</span>Adresse :</label>
					<input type="text" class="form-text" name="mrue" id="mrue" onkeyup='comp(document.getElementById("mpays").value);' />
				</div>
				<div id="adrL" class="row" style="display:none;">
					<label id="lcomp_select" for="comp_select">Choissisez une adresse :</label>
					<select id="comp_select" class="form-select" name="comp_select" size="1" onclick='setAdr()'></select>
				</div>				
				<div class="row some-result">
					<dl class="small-ddt">
						<dt><label for="mnumero" class="form-label-small"><span>*</span> Numéro :</label><input type="text" class="form-text-small" name="mnumero" id="mnumero" /></dt>
						<dd><label for="m-boite" class="form-label-small2"> Boîte :</label><input type="text" class="form-text-small" name="mboite" id="mboite" /></dd>
					</dl>
				</div>
				<div class="row">
					<label for="m-complement">Complément d'adresse :</label>
					<input type="text" class="form-text" name="mcomplement" id="mcomplement" />
				</div>
				<div class="row birth">
					<label for="mbirth"><span>*</span> Date de naissance :</label>
					<input type="text" class="form-text date" name="mbirth" id="mbirth" />
				</div>		
				<!-- <h2>Informations complémentaires :</h2> -->
				<div class="separator"><span>&nbsp;</span></div>
				
				
				<div class="row">
					<p>Si vous êtes déjà abonné à un de nos services, veuillez compléter les champs suivants.</p>
					<label for="num_abo">Numéro d'abonné :</label>
					<input type="text" class="form-text" name="num_abo" id="num_abo" />
				</div>
				<div class="row">
					<label for="cpabo">Code postal de livraison de l'abonnement :</label>
					<input type="text" class="form-text" name="cpabo" id="cpabo" />
					<a class="find" href="http://pdf.lesoir.be/mon_pofil/faq/" target="_blank" >Où trouver mon numéro d'abonné ?</a>
				</div>
			
					
				
				<div class="separator"><span>&nbsp;</span></div>										
				<div class="row checkboxB">
				
					<div class="to-block">
						<input type="checkbox" class="form-cb" name="newsletter1" id="newsletter1" value="Y" />
						<label for="newsletter1">J'accepte de recevoir la newsletter quotidienne d'actualités du journal Le Soir</label>
					</div>
					
					
					<div class="to-block">
						<input type="checkbox" class="form-cb" name="optin1" id="optin1" value="Y" />
						<label for="optin1">J'accepte de recevoir des offres, des informations du journal Le Soir</label>
					</div>
					<div class="to-block">
						<input type="checkbox" class="form-cb" name="optin3" id="optin3" value="Y" onclick="setC()" />
						<label for="optin3">J'accepte de recevoir des offres, des informations des partenaires du journal Le Soir</label>
					</div>
				</div>
					<div class="separator"><span>&nbsp;</span></div>
	<?php } if($f == "forum") { ?>
				<input type="hidden" name="nom" value="<?php echo $n; ?>" />
				<input type="hidden" name="mail" value="<?php echo $m; ?>" />
				<input type="hidden" name="uid" value="<?php echo $u; ?>" />
					<div class="row">
						<label for="mpseudo"><span>*</span> Pseudo :</label>
						<input type="text" class="form-text" name="mpseudo" id="mpseudo" />							
					</div>
					<div class="row checkboxB">
						<div class="to-block">
							<input type="checkbox" class="form-cb" name="charte" id="charte" value="Y" />
							<label>J'accepte les termes de la <a href="http://forums.lesoir.be/index.php?app=forums&module=extras&section=boardrules" target="_blank" id="chrt">charte</a> d'utilisation des forums du Soir. </label> 
						</div>
					</div>
				<?php } ?>
			
				<input type="submit" class="btn-submit" value="créer" />
				</form>
			</div>
				<div id="perror" class="error"></div>
			<p id="pmessage" class="message" style="color:green; text-align:center;"></p>
		</div>
		
		<div class="p-bottom">&nbsp;</div>
	</div>
</div>

