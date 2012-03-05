<?php 

/* Récupération du path
 * 
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

drupal_add_css($themeroot.$theme_path.'/css/services.css'); 

function get_localite_from_url() {
  $data = array(); 
	
  $path = $_GET['q'];
  $arguments = explode('/', $path);
  $data["ville"] = urldecode($arguments[count($arguments)-1]);
  
  $res = db_query('SELECT StationID, NameFR FROM meteo_location WHERE NameFR like "%%%s%%"', $data["ville"]);
	while ($row = db_fetch_array($res)) {
	  		$d["StationID"] = $row["StationID"]; 
				$d["NameFR"] = $row["NameFR"]; 
	  		$data[]=$d;	 
	}
  
  if (count($data)==1) {
	  // Brussel result by default
		$data["ville"] = "Bruxelles";
  	$res = db_query('SELECT StationID, NameFR FROM meteo_location WHERE NameFR like "%%%s%%"', $data["ville"]);
		$data["ville"] = "Belgique";
  	
	  while ($row = db_fetch_array($res)) {
	  		$d["StationID"] = $row["StationID"]; 
				$d["NameFR"] = $row["NameFR"]; 
	  		$data[]=$d;	 
	  }
	  	
  }
  
  return $data; 
}
$ville = get_localite_from_url(); 

?>
<?php 
if (isset($ville[0]["StationID"])) {

?>

<div id="letemps">
  <?php
  if ($ville["ville"]=="Belgique") {
?>
  <h2><span>Le temps en <?php print $ville["ville"];  ?></span></h2>
  <?php
  } else {
?>
  <h2><span>Le temps à <?php print wallydemo_check_plain($ville[0]["NameFR"])?></span></h2>
  <?php
   }
?>
  
  	<?php
	$jour = array('Mon'=>'Lundi','Tue'=>'Mardi','Wed'=>'Mercredi','Thu'=>'Jeudi','Fri'=>'Vendredi','Sat'=>'Samedi','Sun'=>'Dimanche');
	$mois = array('January'=>'JANVIER','February'=>'FEVRIER','March'=>'MARS','April'=>'AVRIL','May'=>'MAI','June'=>'JUIN','July'=>'JUILLET','August'=>'AOUT','September'=>'SEPTEMBRE','October'=>'OCTOBRE','November'=>'NOVEMBRE','December'=>'DECEMBRE');
	$html_date = "<div class=\"date\">";
	$html_date .= "<p>".$jour[date("D")]." ".date("d")." ".strtolower($mois[date("F")]).' '.date("Y")."</p>";
	$html_date .="</div>";
	echo $html_date;
	?>

  
  <?php 

	// Liste des localités.
 if (count($ville)>2 && $ville["ville"]!="Belgique") {
 	print "<div id='listelocalites'>";
	print "<h3>Liste des localité trouvées correspondant à <span class='localite'>'".$ville["ville"]."'</span> :</h3>";
	print "<ul>";
		foreach ($ville as $localite) {
			//var_dump($localite);
			if (is_array($localite)) print "<li><a class=\"arrow\" href='meteo/".$localite["NameFR"]."'>".$localite["NameFR"]."</a></li>";
		}	
	print "</ul></div>"; 	
 }
	
// Premier résultat de recherche.
$imgFolder = $my_data['images_folder']."/meteo_medium/";	
$cpt = 0;

foreach ($my_data["data"][$ville[0]["StationID"]] as $key=>$d ) {

	$cpt ++;
	switch ($cpt){
		case 1:
			$class = "first column";
			$row_start = '<div class="ephemerides huitjours"><ul>';
			$row_end = "";				
		break;
		case 2 :
			$class = "column";
			$row_start = "";
			$row_end = "";				
		break ;
		case 3 :
			$class = "last column";
			$row_start = "";
			$row_end = "</ul></div>";				
			$cpt = 0;
		break ;			
	}
	print $row_start; ?>
  <li class="<?php print $class ?>">
    <h3><?php print $d["date"] ?></h3>
    <img src="<?php print $imgFolder; print $d["logo"]; ?>.png" alt="<?php print $d["text"]; ?>" width="40" height="40" />
    <p><?php print $d["text"]; ?></p>
    <p>De <?php print $d["tmin"]; ?><abbr title="Celsius">C</abbr> &agrave; <?php print $d["tmax"]; ?><abbr title="Celsius">C</abbr></p>
    <p>Vent : <abbr title="<?php print $d["windd"]; ?>"><?php print $d["windd"]; ?></abbr></p>
    <p>Vitesse : <?php print $d["winds"]; ?> <abbr title="Kilomètre par heure">km/h</abbr></p>
  </li>
  <?php print $row_end;
}  // foreach
?> </div>
<?php 
} else {
}
unset($my_data);
unset($ville);
?>
