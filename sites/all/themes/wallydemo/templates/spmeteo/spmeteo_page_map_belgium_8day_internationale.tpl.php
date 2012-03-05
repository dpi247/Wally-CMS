<?php 
/*
 * $my_data; 
 */

/* Récupération du path
 * 
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

drupal_add_css($themeroot.$theme_path.'/css/services.css'); 

function get_international_from_url() {
  $data = array(); 
	
  $path = $_GET['q'];
  $arguments = explode('/', $path);
  $data["ville"] = $arguments[count($arguments)-1];
  
  $res = db_query("SELECT StationID, Name FROM meteo_city WHERE UCASE(StationID) = '%s' ORDER BY Name",
										strtoupper($data["ville"]));
  
  while ($row = db_fetch_array($res)) {
  		$d["StationID"] = $row["StationID"]; 
			$d["Name"] = $row["Name"]; 
  		$data[]=$d;	 
  }
return $data; 
}
$ville = get_international_from_url(); 

if (isset($ville[0]["StationID"])) {
?>

<div id="letemps">
  <h2><span>Le temps à <?php print wallydemo_check_plain($ville[0]["Name"])?></span></h2>
  <?php
	$jour = array('Mon'=>'Lundi','Tue'=>'Mardi','Wed'=>'Mercredi','Thu'=>'Jeudi','Fri'=>'Vendredi','Sat'=>'Samedi','Sun'=>'Dimanche');
	$mois = array('January'=>'JANVIER','February'=>'FEVRIER','March'=>'MARS','April'=>'AVRIL','May'=>'MAI','June'=>'JUIN','July'=>'JUILLET','August'=>'AOUT','September'=>'SEPTEMBRE','October'=>'OCTOBRE','November'=>'NOVEMBRE','December'=>'DECEMBRE');
	$html_date = "<div class=\"date\">";
	$html_date .= "<p>".$jour[date("D")]." ".date("d")." ".strtolower($mois[date("F")]).' '.date("Y")."</p>";
	$html_date .="</div>";
	echo $html_date;
	?>
  <?php 

	$imgFolder = $my_data['images_folder']."/meteo_medium/";	
	$cpt = 0;
	foreach ($my_data["temperatures"][$ville[0]["StationID"]]["day"] as $key=>$d ) {
		
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
	} ?>
  </ul>
</div>
</div>
<?php 
} // if isset()
unset($my_data);
unset($ville);
?>
