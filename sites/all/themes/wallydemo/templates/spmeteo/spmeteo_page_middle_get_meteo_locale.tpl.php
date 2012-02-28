<h1>AFAC - page_miuddle_get_mtteo_locale</h1>
<?php 

if(isset($_GET["stationid"]) || isset($_GET["nom_cp"])) {
	$choice = 1;
	if(!isset($_GET["stationid"])) {
		$param = $_GET["nom_cp"]; 
		if(is_numeric($param)) {
			$sql = "SELECT StationID, NameFR FROM meteo_location WHERE ZIP = ".$param;
		} else {
			$sql = "SELECT StationID, NameFR FROM meteo_location WHERE UCASE(NameFR) LIKE '".strtoupper($param)."%' OR UCASE(NameNL) LIKE '".strtoupper($param)."%' ORDER BY NameFR ";
		}
		$res = db_query($sql);
		$count = mysql_num_rows($res);
		if($count > 1) {
			$choice = 0;
		}
		else { 
			$row = db_fetch_array($res);
			$stationid = $row["StationID"];
			$choice = 1;
		}
	} else {
		$stationid = $_GET["stationid"];
		$choice = 1;
	}
	if($choice == 0) { ?>
	
	<div id="letemps">
		<h2><span>Résultats : </span></h2>
		<div class="resultats">
			<h3>Veuillez préciser votre commune :</h3>
			<ul class="listvil">
			<?php while ($row = db_fetch_array($res)) { ?>
			<li><a href="?stationid=<?php print $row["StationID"]?>&nom_cp=<?php print $row["NameFR"]?>"><?php print $row["NameFR"]?></a></li>		
			<?php } ?>
			</ul>
		</div>
	</div>	
	
	<?php } else { ?>
	
		<div id="letemps">
		<?php 
		$my_data = _spmeteo_getdata('belgium_map_8day',NULL,$stationid);
		$imgFolder = $my_data['images_folder']."/meteo_medium/";	
		$cpt = 0;
		foreach ($my_data["data"] as $key=>$d ) {	
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
		</div>
<?php	}
} 
unset($my_data);
?>