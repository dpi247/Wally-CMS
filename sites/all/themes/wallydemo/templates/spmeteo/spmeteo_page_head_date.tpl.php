<?php 

/* Récupération du path
 * 
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

drupal_add_css($themeroot.$theme_path.'/css/services.css'); 


	if(isset($_GET["nom_cp"]) && !is_numeric($_GET["nom_cp"])) {
		$place = "à : ".$_GET["nom_cp"];
	} elseif (isset($_GET["ville_e"]) && !is_numeric($_GET["ville_e"])) {
		$ville = explode(";",$_GET["ville_e"]);
		$place = "à : ".$ville[1];
	} else {
		$place = "en Belgique"; 
	}

?>

<div id="letemps">
<h2><span>Le temps <?php print $place; ?></span></h2>
	<?php
	$jour = array('Mon'=>'Lundi','Tue'=>'Mardi','Wed'=>'Mercredi','Thu'=>'Jeudi','Fri'=>'Vendredi','Sat'=>'Samedi','Sun'=>'Dimanche');
	$mois = array('January'=>'JANVIER','February'=>'FEVRIER','March'=>'MARS','April'=>'AVRIL','May'=>'MAI','June'=>'JUIN','July'=>'JUILLET','August'=>'AOUT','September'=>'SEPTEMBRE','October'=>'OCTOBRE','November'=>'NOVEMBRE','December'=>'DECEMBRE');
	$html_date = "<div class=\"date\">";
	$html_date .= "<p>".$jour[date("D")]." ".date("d")." ".strtolower($mois[date("F")]).' '.date("Y")."</p>";
	$html_date .="</div>";
	echo $html_date;
	?>
</div>
