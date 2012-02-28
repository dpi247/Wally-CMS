<?php 
/*
 * Template pour la carte meteo du jour.
 * 
 * données: $my_data array();   
 */

/* Récupération du path
 * 
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

drupal_add_css($themeroot.$theme_path.'/css/services.css'); 

?>
<?php 
	$imgFolder = $my_data['images_folder']."/meteo_little/";		
	$logo = array('Ciel serein','Ciel peu nuageux','Ciel très nuageux','Couvert','Brouillard','Pluie','Bruine','Neige','Pluie et neige','Pluie et bruine','Averses de pluie','Averses orageuses','Giboulées','Averse de neige','Averses de pluie et neige','Orage','Orageux','Verglas');
?>

<div id="letemps">
  <h2><span>Le temps en Belgique</span></h2>
  <?php
	$jour = array('Mon'=>'Lundi','Tue'=>'Mardi','Wed'=>'Mercredi','Thu'=>'Jeudi','Fri'=>'Vendredi','Sat'=>'Samedi','Sun'=>'Dimanche');
	$mois = array('January'=>'JANVIER','February'=>'FEVRIER','March'=>'MARS','April'=>'AVRIL','May'=>'MAI','June'=>'JUIN','July'=>'JUILLET','August'=>'AOUT','September'=>'SEPTEMBRE','October'=>'OCTOBRE','November'=>'NOVEMBRE','December'=>'DECEMBRE');
	$html_date = "<div class=\"date\">";
	$html_date .= "<p>".$jour[date("D")]." ".date("d")." ".strtolower($mois[date("F")]).' '.date("Y")."</p>";
	$html_date .="</div>";
	echo $html_date;
	?>
  <div class="ephemerides">
    <ul>
      <li class="first column"> <img src="<?php print $imgFolder; print $my_data['today']['weather']; ?>.png" alt="<?php print $my_data['today']['title']; ?>" width="20" height="20" class="right" />
        <h3><a href="<?php echo drupal_get_path_alias("/meteo")?>">Aujourd'hui</a></h3>
        <p><?php print $my_data['today']['tmin']; ?>&deg;/<?php print $my_data['today']['tmax']; ?>&deg;</p>
        <p><?php print $logo[($my_data['today']['weather'])-1]; ?></p>
      </li>
      <li class="column"> <img src="<?php print $imgFolder; print $my_data['tomorrow']['weather']; ?>.png" alt="<?php print $my_data['tomorrow']['title']; ?>" width="20" height="20" class="right" />
        <h3><a href="<?php echo drupal_get_path_alias("/meteo")?>/demain">Demain</a></h3>
        <p><?php print $my_data['tomorrow']['tmin']; ?>&deg;/<?php print $my_data['tomorrow']['tmax']; ?>&deg;</p>
        <p><?php print $logo[($my_data['tomorrow']['weather'])-1]; ?></p>
      </li>
      <li class="last column"> <img src="<?php print $imgFolder; print $my_data['day3']['weather']; ?>.png" alt="<?php print $my_data['day3']['title']; ?>" width="20" height="20" class="right" />
        <h3><a href="<?php echo drupal_get_path_alias("meteo")?>/apresdemain">Apr&egrave;s-demain</a></h3>
        <p><?php print $my_data['day3']['tmin']; ?>&deg;/<?php print $my_data['day3']['tmax']; ?>&deg;</p>
        <p><?php print $logo[($my_data['day3']['weather'])-1]; ?></p>
      </li>
    </ul>
    <div class="prochains">
      <p class="right"><a href="<?php echo drupal_get_path_alias("/meteo")?>/8jours">La m&eacute;t&eacute;o des 8 prochains jours</a></p>
    </div>
  </div>
  <div class="carte"> <img src="/<?php print $my_data['medias_folder'] ?>_sp/D1.png" width="428" height="349" />
    <p class="rose_vents">Vent de <?php print $my_data['tomorrow']['vit']; ?><acronym title="Kilom&egrave;tre par heure">km/h</acronym>, Secteur <?php print $my_data['tomorrow']['sect']; ?></p>
  </div>
</div>
<?php 
//unset variables
unset($my_data);
?>
