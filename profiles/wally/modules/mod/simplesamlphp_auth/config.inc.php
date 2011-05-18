<?php
	$path = "http://pdf.lesoir.be/mon_profil/";
	$server_path = "/var/www/pdf.lesoir.be/mon_profil/";	
	$aboJourMin = "5";
	$aboDureeMin = "6";
	$aboDureeMax = "60";
	$orderMin = "-60d";
	$orderMax = "-1d";
	$mailAbo = "abonnements@lesoir.be";
	$mailAbo = "jda@audaxis.be";	
	$country_list = array("BE"=>"Belgique","AF"=>"Afghanistan","AL"=>"Albania","DZ"=>"Algeria","AS"=>"American Samoa","AD"=>"Andorra","AO"=>"Angola","AI"=>"Anguilla","AQ"=>"Antarctica","AG"=>"Antigua And Barbuda","AR"=>"Argentina","AM"=>"Armenia","AW"=>"Aruba","AU"=>"Australia","AT"=>"Austria","AZ"=>"Azerbaijan","BS"=>"Bahamas","BH"=>"Bahrain","BD"=>"Bangladesh","BB"=>"Barbados","BY"=>"Belarus","BE"=>"Belgique","BZ"=>"Belize","BJ"=>"Benin","BM"=>"Bermuda","BT"=>"Bhutan","BO"=>"Bolivia","BA"=>"Bosnia And Herzegovina","BW"=>"Botswana","BV"=>"Bouvet Island","BR"=>"Brazil","IO"=>"British Indian Ocean Territory","BN"=>"Brunei Darussalam","BG"=>"Bulgaria","BF"=>"Burkina Faso","BI"=>"Burundi","KH"=>"Cambodia","CM"=>"Cameroon","CA"=>"Canada","01"=>"Canaries","CV"=>"Cape Verde","KY"=>"Cayman Islands","CF"=>"Central African Republic","TD"=>"Chad","CL"=>"Chile","CN"=>"China","CX"=>"Christmas Island","CC"=>"Cocos (keeling) Islands","CO"=>"Colombia","KM"=>"Comoros","CG"=>"Congo","CD"=>"Congo The Democratic Republic Of The","CK"=>"Cook Islands","CR"=>"Costa Rica","CI"=>"Cote D'ivoire","HR"=>"Croatia","CU"=>"Cuba","CY"=>"Cyprus","CZ"=>"Czech Republic","DK"=>"Denmark","DJ"=>"Djibouti","DM"=>"Dominica","DO"=>"Dominican Republic","EC"=>"Ecuador","EG"=>"Egypt","SV"=>"El Salvador","GQ"=>"Equatorial Guinea","ER"=>"Eritrea","EE"=>"Estonia","ET"=>"Ethiopia","FK"=>"Falkland Islands (malvinas)","FO"=>"Faroe Islands","FJ"=>"Fiji","FI"=>"Finland","FR"=>"France","GF"=>"French Guiana","PF"=>"French Polynesia","TF"=>"French Southern Territories","GA"=>"Gabon","GM"=>"Gambia","GE"=>"Georgia","DE"=>"Germany - Deutschland","GH"=>"Ghana","GI"=>"Gibraltar","GR"=>"Greece","GL"=>"Greenland","GD"=>"Grenada","GP"=>"Guadeloupe","GU"=>"Guam","GT"=>"Guatemala","GN"=>"Guinea","GW"=>"Guinea-bissau","GY"=>"Guyana","HT"=>"Haiti","HM"=>"Heard Island And Mcdonald Islands","VA"=>"Holy See (vatican City State)","HN"=>"Honduras","HK"=>"Hong Kong","HU"=>"Hungary","IS"=>"Iceland","IN"=>"India","ID"=>"Indonesia","IR"=>"Iran Islamic Republic Of","IQ"=>"Iraq","IE"=>"Ireland","03"=>"IRLANDE DU NORD","IL"=>"Israel","IT"=>"Italy","JM"=>"Jamaica","JP"=>"Japan","JO"=>"Jordan","KZ"=>"Kazakhstan","KE"=>"Kenya","KI"=>"Kiribati","KP"=>"Korea Democratic People's Republic Of","KR"=>"Korea Republic Of","KW"=>"Kuwait","KG"=>"Kyrgyzstan","LA"=>"Lao People's Democratic Republic","LV"=>"Latvia","LB"=>"Lebanon","LS"=>"Lesotho","LR"=>"Liberia","LY"=>"Libyan Arab Jamahiriya","LI"=>"Liechtenstein","LT"=>"Lithuania","LU"=>"Luxembourg","MO"=>"Macao","MK"=>"Macedonia Former Yugoslav Republic Of","MG"=>"Madagascar","02"=>"MADERES","MW"=>"Malawi","MY"=>"Malaysia","MV"=>"Maldives","ML"=>"Mali","MT"=>"Malta","MH"=>"Marshall Islands","MQ"=>"Martinique","MR"=>"Mauritania","MU"=>"Mauritius","YT"=>"Mayotte","MX"=>"Mexico","FM"=>"Micronesia Federated States Of","MD"=>"Moldova Republic Of","MC"=>"Monaco","MN"=>"Mongolia","MS"=>"Montserrat","MA"=>"Morocco","MZ"=>"Mozambique","MM"=>"Myanmar","NA"=>"Namibia","NR"=>"Nauru","NL"=>"Nederland","NP"=>"Nepal","AN"=>"Netherlands Antilles","NC"=>"New Caledonia","NZ"=>"New Zealand","NI"=>"Nicaragua","NE"=>"Niger","NG"=>"Nigeria","NU"=>"Niue","NF"=>"Norfolk Island","MP"=>"Northern Mariana Islands","NO"=>"Norway","OM"=>"Oman","PK"=>"Pakistan","PW"=>"Palau","PS"=>"Palestinian Territory Occupied","PA"=>"Panama","PG"=>"Papua New Guinea","PY"=>"Paraguay","PE"=>"Peru","PH"=>"Philippines","PN"=>"Pitcairn","PL"=>"Poland","PT"=>"Portugal","PR"=>"Puerto Rico","QA"=>"Qatar","RE"=>"Reunion","RO"=>"Romania","RU"=>"Russian Federation","RW"=>"Rwanda","SH"=>"Saint Helena","KN"=>"Saint Kitts And Nevis","LC"=>"Saint Lucia","PM"=>"Saint Pierre And Miquelon","VC"=>"Saint Vincent And The Grenadines","WS"=>"Samoa","SM"=>"San Marino","ST"=>"Sao Tome And Principe","SA"=>"Saudi Arabia","SN"=>"Senegal","SC"=>"Seychelles","SL"=>"Sierra Leone","SG"=>"Singapore","SK"=>"Slovakia","SI"=>"Slovenia","SB"=>"Solomon Islands","SO"=>"Somalia","ZA"=>"South Africa","GS"=>"South Georgia And The South Sandwich Islands","ES"=>"Spain","LK"=>"Sri Lanka","SD"=>"Sudan","SR"=>"Suriname","SJ"=>"Svalbard And Jan Mayen","SZ"=>"Swaziland","SE"=>"Sweden","CH"=>"Switzerland","SY"=>"Syrian Arab Republic","TW"=>"Taiwan","TJ"=>"Tajikistan","TZ"=>"Tanzania United Republic Of","TH"=>"Thailand","TL"=>"Timor-leste","TG"=>"Togo","TK"=>"Tokelau","TO"=>"Tonga","TT"=>"Trinidad And Tobago","TN"=>"Tunisia","TR"=>"Turkey","TM"=>"Turkmenistan","TC"=>"Turks And Caicos Islands","TV"=>"Tuvalu","UG"=>"Uganda","UA"=>"Ukraine","AE"=>"United Arab Emirates","GB"=>"United Kingdom","US"=>"United States","UM"=>"United States Minor Outlying Islands","UY"=>"Uruguay","UZ"=>"Uzbekistan","VU"=>"Vanuatu","VE"=>"Venezuela","VN"=>"Viet Nam","VG"=>"Virgin Islands British","VI"=>"Virgin Islands U.s.","WF"=>"Wallis And Futuna","EH"=>"Western Sahara","YE"=>"Yemen","YU"=>"Yugoslavia","ZM"=>"Zambia","ZW"=>"Zimbabwe" );
	
function xml_sus_create($data) {
	$n_abo = ""; $n_client = ""; $dateActivite = ""; $dateFin = "";$idSSO = "";

	$n_abo = 'noAbonnement="'.$data->n_abo.'"';
	if($data->idSSO) $idSSO = ' idSSO="'.$data->idSSO.'"';	
	$client = '<benef noClient="'.$data->n_client.'"'.$idSSO.' />';
	if($data->dateActivite) $dateActivite = 'dateActivite="'.$data->dateActivite.'"';
	if($data->dateFin) $dateFin = 'dateFin="'.$data->dateFin.'"';
	
	$xml = <<<EOF
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE aboMvmt SYSTEM "aboMvmt.dtd">
<aboMvmt>
     	<trx $n_abo $dateFin $dateActivite typeTrx="SUS" collection="N" >
			$client
		</trx>
</aboMvmt>
EOF;

	$url = $_SERVER['DOCUMENT_ROOT'].'/mon_profil/XML/SUS/TMP_SOIRX_SUS_'.date("Ymd-Gis").'_'.$data->n_abo.'.xml';
	if($fp = fopen($url, 'w')) {
		if(fwrite($fp, $xml)) {
			fclose($fp);	
			if(rename($url, str_replace('TMP_','',$url))) {
				return true;
			}
			else {
				return false;
			}
		}	
		else {
			return false;
		}
	}
	else {
		return false;		
	}
}

function xml_mut_create($adresse) {
	$adresse = class_transform_specialchars($adresse);
	$n_abo = ""; $n_client = ""; $idStreet = ""; $lang = ""; $street = ""; $street_2 = ""; $zipcode = ""; $city = ""; $country = "";
	$dateActivite = ""; $dateFin = ""; $definitive = ""; $titre = "";$idSSO = "";
	
	$n_abo = 'noAbonnement="'.$adresse->n_abo.'"';
	if($adresse->idSSO) $idSSO = ' idSSO="'.$adresse->idSSO.'"';	
	$n_client = '<benef noClient="'.$adresse->n_client.'"'.$idSSO.' />';
	if($adresse->titre) $titre = '<Civ>'.$adresse->titre.'</Civ>';
	if($adresse->firstname) $firstname = '<Firstname>'.$adresse->firstname.'</Firstname>';
	if($adresse->lastname) $lastname= '<Lastname>'.$adresse->lastname.'</Lastname>';
	if($adresse->idStreet) $idStreet = 'RO_SopressStreet_ID="'.$adresse->idStreet.'"';
	if($adresse->lang) $lang = 'language="'.$adresse->lang.'"';
	if($adresse->ext) $ext = ' Bte '.cut_comma($adresse->ext);
	if($adresse->street) $street =  '<Address1>'.cut_comma($adresse->street).', '.cut_comma($adresse->number).''.cut_comma($ext).'</Address1>';
	if($adresse->street_2) $street_2 = '<Address2>'.cut_comma($adresse->street_2).'</Address2>';
	if($adresse->zipcode) $zipcode = '<PostalCode>'.$adresse->zipcode.'</PostalCode>';
	if($adresse->city) $city = '<Localite>'.$adresse->city.'</Localite>';	    
	if($adresse->country == 'B' || $adresse->country == 'Belgique' || $adresse->country == 'BELGIQUE') $adresse->country = 'BE';
	if($adresse->country) $country = '<Country>'.$adresse->country.'</Country>';
	if($adresse->dateActivite) $dateActivite = 'dateActivite="'.$adresse->dateActivite.'"';
	if($adresse->dateFin) $dateFin = 'dateFin="'.$adresse->dateFin.'"';
	if($adresse->definitive) $definitive = 'isDefinitive="'.$adresse->definitive.'"';
	
	$xml = <<<EOF
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE aboMvmt SYSTEM "aboMvmt.dtd">
<aboMvmt>
      	<trx $definitive $n_abo $dateActivite $dateFin typeTrx="MUT" >
			$n_client		
      		<address $idStreet $lang >
      			$titre      		
				$lastname
				$firstname
				$street
				$street_2
          		$zipcode
          		$city
          		$country
        	</address>
       </trx>
</aboMvmt>
EOF;
	$url = $_SERVER['DOCUMENT_ROOT'].'/mon_profil/XML/MUT/TMP_SOIRX_MUT_'.date("Ymd-Gis").'_'.$adresse->n_abo.'.xml';
	if($fp = fopen($url, 'w')) {
		if(fwrite($fp, $xml)) {
			fclose($fp);	
			if(rename($url, str_replace('TMP_','',$url))) {
				return true;
			}
			else {
				return false;
			}
		}	
		else {
			return false;
		}
	}
	else {
		return false;		
	}
}	
function cut_comma($s) {
		$s = str_replace(',',' ',$s);
		
		return $s;
}

function class_transform_specialchars($adresse) {
	foreach($adresse as $key => $value) {
		$s = $value;
		$s = str_replace('&','&amp;',$s);
		$s = str_replace('<','&lt;',$s);
		$s = str_replace('>','&gt;',$s);
		$adresse->$key = $s;
	}	
	return $adresse;
}

function sMailAbo($mailFrom,$mailTo,$sujet,$adresse) {
	
	$message =
	'N° Client : '.$adresse->n_client."\n".
	'Civilité : '.$adresse->titre."\n".		
	'Nom : '.$adresse->lastname."\n".
	'Prénom : '.$adresse->firstname."\n".
	'N° Abo : '.$adresse->n_abo."\n".
	'Rue : '.$adresse->street."\n".
	'Complément d\'adresse : '.$adresse->street_2."\n".
	'N° : '.$adresse->number."\n".
	'Boîte: '.$adresse->ext."\n".
	'Ville : '.$adresse->city."\n".
	'Code postal : '.$adresse->zipcode."\n".
	'Pays : '.$adresse->country."\n".
	'Email : '.$adresse->mail."\n".
	'Date debut : '.$adresse->dateActivite."\n".
	'Date fin : '.$adresse->dateFin;		
	
	$entetes = 'From: '.$mailFrom."\r\n";
	$entetes .= 'Content-Type: text/plain; charset=utf-8'."\r\n";
	$entetes .= 'Content-Transfer-Encoding: 8bit';	
	$envoi_mail = mail($mailTo, $sujet, $message, $entetes);	
}

function sMailClient($mailFrom,$mailTo,$sujet,$message,$Name="LeSoir.be") {
	
	$entetes = 'From: '.$mailFrom."\r\n";
	$entetes = "From: ". $Name . " <" . $mailFrom . ">\r\n";
	$entetes .= 'Content-Type: text/plain; charset=utf-8'."\r\n";
	$entetes .= 'Content-Transfer-Encoding: 8bit';	
	$envoi_mail = mail($mailTo, $sujet, $message, $entetes);	
}

function validEmail($email)
{
   $isValid = true;
   $atIndex = strrpos($email, "@");
   if (is_bool($atIndex) && !$atIndex)
   {
      $isValid = false;
   }
   else
   {
      $domain = substr($email, $atIndex+1);
      $local = substr($email, 0, $atIndex);
      $localLen = strlen($local);
      $domainLen = strlen($domain);
      if ($localLen < 1 || $localLen > 64)
      {
         // local part length exceeded
         $isValid = false;
      }
      else if ($domainLen < 1 || $domainLen > 255)
      {
         // domain part length exceeded
         $isValid = false;
      }
      else if ($local[0] == '.' || $local[$localLen-1] == '.')
      {
         // local part starts or ends with '.'
         $isValid = false;
      }
      else if (preg_match('/\\.\\./', $local))
      {
         // local part has two consecutive dots
         $isValid = false;
      }
      else if (!preg_match('/^[A-Za-z0-9\\-\\.]+$/', $domain))
      {
         // character not valid in domain part
         $isValid = false;
      }
      else if (preg_match('/\\.\\./', $domain))
      {
         // domain part has two consecutive dots
         $isValid = false;
      }
      else if(!preg_match('/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/',str_replace("\\\\","",$local)))
      {
         // character not valid in local part unless 
         // local part is quoted
         if (!preg_match('/^"(\\\\"|[^"])+"$/',
             str_replace("\\\\","",$local)))
         {
            $isValid = false;
         }
      }
      if ($isValid && !(checkdnsrr($domain,"MX") || checkdnsrr($domain,"A")))
      {
         // domain not found in DNS
         $isValid = false;
      }
   }
   return $isValid;
}
//FONCTION DE VALIDATION
function valid_pdata($vdata){
	return (isset($vdata) && $vdata != '');	
}

function validIdsso($idSSO) {
	if(preg_match("/^[a-z]{1}[A-Z 0-9]{6}$/",$idSSO)) return true;
	else return false;
}

function mailCommande($info) {
	
	$data = '';
	if($info['nom']) $data .= utf8_encode('Nom : '.$info['nom'])."\n";
	if($info['prenom']) $data .= utf8_encode('Prenom : '.$info['prenom'])."\n";
	if($info['rue']) $data .= utf8_encode('Rue : '.$info['rue'])."\n";
	if($info['complement']) $data .= utf8_encode('Complément d\'adresse : '.$info['complement'])."\n";
	if($info['numero']) $data .= 'N° : '.utf8_encode($info['numero'])."\n";
	if($info['boite']) $data .= utf8_encode('Boîte : '.$info['boite'])."\n";
	if($info['localite']) $data .= utf8_encode('Ville : '.$info['localite'])."\n";
	if($info['code_postal']) $data .= utf8_encode('Code postal : '.$info['code_postal'])."\n";
	if($info['pays']) $data .= utf8_encode('Pays : '.$info['pays'])."\n";
	if($info['email']) $data .= utf8_encode('Email : '.$info['email'])."\n";
	if($info["orderId"]) $data .= utf8_encode('orderId : '.$info['orderId'])."\n";
	if($posted["acceptance"]) $data .= utf8_encode('acceptance : '.$info['acceptance'])."\n";
	if($posted["payid"]) $data .= utf8_encode('payid : '.$info['payid'])."\n";
	$data .= 'Date du journal commandé : '.$info['date']."\n";	

	return $data;

}

?>