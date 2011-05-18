<?php
	require_once("config.inc.php");
	require_once('/var/www/php/ssoLibs/ssoPhpToolbox/IDPInstance_rossel.php');
	require_once('/var/www/php/ssoLibs/ssoPhpToolbox/SSOToolbox.php');
	$target = "http://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI']; 
	$toolbox = new SSOToolbox(array('targetUrl'=>$target));
	$infoSSO = $toolbox->isAuthenticated($target);	
	$data = json_decode(urldecode($_GET['data']));

// print_r($data);
	if($data->registerOp == "FULL") {		

		if(valid_pdata($data->mgenre))$attributes['title'][0] = utf8_decode($data->mgenre);
		else  $ret["mgenre"] = "Veuillez remplir le champ genre. ";
		if(valid_pdata($data->memail))$attributes['mail'][0] = utf8_decode($data->memail);
		else $ret["memail"] = "Veuillez remplir le champ e-mail. ";		
		if(valid_pdata($data->mconfemail))$mconfmail = utf8_decode($data->mconfemail);
		else $ret["mconfemail"] = "Veuillez remplir le champ confirmation d'e-mail. ";
		if(valid_pdata($data->mmdp)) {
			if(strlen($data->mmdp) > 7) $attributes["mdp"][0] = utf8_decode($data->mmdp);
			else $ret["mmdp"] =  "Le mot de passe doit comporter 8 caractères minimun. ";
		} else {
			$ret["mmdp"] =  "Veuillez remplir le champ mot de passe. ";
		}
		if(valid_pdata($data->mconfmdp))$confmdp = utf8_decode($data->mconfmdp);
		else $ret["mconfmdp"] = "Veuillez remplir le champ confirmation de mot de passe. ";
		if(valid_pdata($data->mprenom))$attributes['givenname'][0] = utf8_decode($data->mprenom);
		else $ret["mprenom"] =  "Veuillez remplir le champ prénom. ";
		if(valid_pdata($data->mnom))$attributes['sn'][0] = utf8_decode($data->mnom);
		else  $ret["mnom"] .= "Veuillez remplir le champ nom. ";
		if(valid_pdata($data->mbirth))$attributes['sunidentityserverppdemographicsbirthday'][0] = utf8_decode($data->mbirth);
		else $ret["mbirth"] = "Veuillez remplir le champ date de naissance. ";
		if(valid_pdata($data->mpays))$attributes['co'][0] = utf8_decode($data->mpays);
		else $ret["mpays"] =  "Veuillez remplir le champ pays. ";
		if(valid_pdata($data->mcp))$attributes['postalcode'][0] = utf8_decode($data->mcp);
		else $ret["mcp"] =  "Veuillez remplir le champ code postal. ";
		if(valid_pdata($data->mloc))$attributes['l'][0] = utf8_decode($data->mloc);
		else $ret["mloc"] =  "Veuillez remplir le champ localité. ";
		if(valid_pdata($data->mrue))$attributes['postaladdress'][0] = utf8_decode($data->mrue);
		else $ret["mrue"] = "Veuillez remplir le champ rue. ";	
		if(valid_pdata($data->mnumero))$attributes['postalnum'][0] = utf8_decode($data->mnumero);
		else $ret["mnumero"] =  "Veuillez remplir le champ numéro. ";
		$attributes['postalbox'][0] = utf8_decode($data->mboite);
		$attributes['postaladdress2'][0] = utf8_decode($data->mcomplement);	
		$attributes['optin1'][0] = $data->optin1;
		$attributes['optin3'][0] = $data->optin3;
		$attributes['newsletter1'][0] = $data->newsletter1;
		$attributes['num_abo'][0] = $data->num_abo;
		$attributes['cpabo'][0] = $data->cpabo;
		
		
		
		$is_abo = FALSE;
		if($attributes['num_abo'][0] != "" && $attributes['cpabo'][0] != ""){
				$cpabo = mysql_real_escape_string($attributes['cpabo'][0]);
				$numabo = mysql_real_escape_string($attributes['num_abo'][0]);
				$rpdf_link = mysql_connect("commutest.rossel.be", "rosselpdftestusr", "pdflspwdTest");
				mysql_select_db("rosselpdftest", $rpdf_link);
				$aboquery = "SELECT * FROM abo_papier WHERE postalcode ='".$cpabo."' AND noClient = '".$numabo."' AND is_logged <> 1 AND dateFin > NOW()";
				$aboresult = mysql_query($aboquery, $rpdf_link);
					
				if(mysql_num_rows($aboresult)>0){
					$is_abo = TRUE;
				}else{
					$ret['num_abo'] = "Votre numéro d'abonné n'a pas été trouvé. Veuillez vérifier que le numéro et le code postal sont corrects.";	
				}
		}
		
		
		if($data->site == "forum") {
			require_once("DBConnectF.php");
			$query = "SELECT * FROM ibf_members WHERE members_l_display_name ='".$data->mpseudo."'";
			$r = mysql_query($query, $DBLink);	
			if(mysql_num_rows($r) < 1) { 			
				if(valid_pdata($data->mpseudo))$attributes['pseudo'] = utf8_decode($data->mpseudo);
				else $ret["mpseudo"] =  "Veuillez remplir le champ pseudo. ";
			} else {
				$ret["mpseudo"] =  "Ce pseudo est déjà utilisé. ";
			}

			if($data->charte == 'Y')$attributes['charte'] = utf8_decode($data->charte);
			else $ret["charte"] =  "Veuillez accepter la charte d'utilisation. ".$query;
		}

				
		if(count($ret) == 0) {
			if(!validEmail($attributes['mail'][0])) $ret["memail"] = "L'adresse e-mail n'est pas valide. ";
			if($attributes['mail'][0] != $mconfmail) $ret["mconfemail"] = "L'adresse e-mail de confirmation n'est pas valide. ";
			$result = $toolbox->searchUser("*",array("mail"=>$attributes['mail'][0]));
			if(count($result) > 0) $ret["memail"] = "Cette adresse e-mail est déjà utilisée. ";
			if($attributes["mdp"][0]  != $confmdp) $ret["mconfmdp"] = "Le mot de passe de confirmation n'est pas valide. ";
		
			require_once("DBConnect.php");		
			$query = "SELECT * FROM sso_validating WHERE email='".$attributes['mail'][0]."' AND used='N'";
			$r = mysql_query($query, $DBLink);	
			if(mysql_num_rows($r) > 0) $ret["memail"] = "Cette adresse e-mail est déjà en attente de validation. ";
			$data = mysql_fetch_array($r);
			if($errors == "") {
				$result = $toolbox->searchUser("*",array("mail"=>$attributes['mail'][0])); 
				if(count($result) > 0) $ret["memail"] = "Cet adresse e-mail est déjà utilisée. ";
					if(count($ret) == 0) {	
						$data = serialize($attributes);
						$ret["MESSAGE"] = "Votre profil est en attente de validation. <br />Un mail a été envoyé à votre adresse e-mail. Pour valider votre profil, vous devez cliquer sur le lien qui se trouve dans le mail.";
						$tt = time();
						$unique_code = md5($attributes['mail'][0].$tt."moijmlesso");				
						$query = "INSERT INTO sso_validating (email, code, time, data, num_abo, cp_abo) VALUES ('".$attributes['mail'][0]."', '".$unique_code."', ".$tt.",'".$data."', '".$num_abo."', '".$cp_abo."')";
						$results = mysql_query($query, $DBLink);
						$mesg = '
	Bonjour '.$attributes['title'][0].' '.$attributes['sn'][0].',

	Pour confirmer votre création de votre profil, veuillez cliquez ici ou «copier-coller» le lien suivant dans la barre d\'adresse de votre navigateur Internet :
	http://pdf.lesoir.be/mon_profil/data/validate_profil.php?ucode='.$unique_code.'

	Vous pouvez à tout moment mettre à jour vos informations personnelles ou compléter votre profil en cliquant ici (ouhttp://pdf.lesoir.be/mon_profil/?action=profile).
	Nous vous remercions pour votre confiance. Nous sommes à votre disposition pour toutes vos suggestions ou pour répondre à vos questions.


	Cordialement
	L\'équipe «abonnements» du Soir
	Pour nous contacter: abonnements@lesoir.be ';	
					sMailClient("noreply@lesoir.be",$attributes['mail'][0],'Création de votre profil. ',$mesg);
					$success = true;
					
			
					
				}
			}
		}
	
	} else	{
		$data = json_decode(urldecode($_GET['data']));
		require_once("DBConnectF.php");
		$query = "SELECT * FROM ibf_members WHERE members_l_display_name ='".$data->mpseudo."'";
		$r = mysql_query($query, $DBLink);	
		if(mysql_num_rows($r) < 1) { 			
			if(valid_pdata($data->mpseudo)) $attributes['pseudo'] = $data->mpseudo;
			else $ret["mpseudo"] =  "Veuillez remplir le champ pseudo. ";
		} else {
			$ret["mpseudo"] =  "Ce pseudo est déjà utilisé. ";
		}

		if($data->charte == 'Y') $attributes['charte'] = $data->charte;
		else $ret["charte"] =  "Veuillez accepter la charte d'utilisation.";		
		if(count($ret) == 0) { 
			$n = $data->sn;
			$p = $data->mpseudo;
			$e = $data->mail;
			$u = $data->uid;
			$res = file_get_contents("http://forums2.lesoir.be/reactions_forum/classes/crLocalMember.php?nom=".$n."&pseudo=".$p."&mail=".$e."&uid=".$u);	
			if($res != "*NOT**OK*") {
				$mesg = '
	Bonjour Mr/Mme '.$n.',

	Vous êtes dès à présent inscrit au service Forums du Soir. Vous pouvez réagir sur les forums et les articles du Soir.
	Votre pseudo est : '.$res.'
	
	
	Vous pouvez à tout moment mettre à jour vos informations personnelles ou compléter votre profil en cliquant ici (ou http://pdf.lesoir.be/mon_profil/?action=profile).
	Nous vous remercions pour votre confiance. Nous sommes à votre disposition pour toutes vos suggestions ou pour répondre à vos questions.


	Cordialement
	L\'équipe «abonnements» du Soir
	Pour nous contacter: abonnements@lesoir.be ';	
				sMailClient("noreply@lesoir.be",$e,'Service du Soir. ',$mesg);		
				$ret["MESSAGE"] = "Vous pouvez à présent réagir aux articles du Soir. ";
				$success = true;
			}
			else {
				$ret["mpseudo"] =  "Ce compte existe déjà. ";
			}
		}
	}
	
	if(!$success) {
		$ret["ERROR"] = "Y";
	}
	$ret = json_encode($ret);
	echo $ret;

?>
