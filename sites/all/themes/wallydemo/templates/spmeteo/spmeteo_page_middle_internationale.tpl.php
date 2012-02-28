<?php
/*
 * $my_data; 
 */

  $img_path = $my_data['images_folder']."/meteo_little/";	 

	$html_continents = "";
	$html_pays = "";
	$html_regions = "";
		
	/*
	 * fonction qui affiche un pays donné.
	 */

	
	foreach ($my_data['country'] as $continent=>$pays) {
		$html_continents .= "<option value=\"".nicevalue($continent)."\">".check_plain($continent)."</option>\n";
		foreach ($pays as $pay => $regions) {
			$html_pays .= "<option class=\"sub_".nicevalue($continent)."\" value=\"".nicevalue($pay)."\">".check_plain($pay)."</option>\n";
			foreach ($regions as $region => $r) {
				$html_regions .= "<option class=\"sub_".nicevalue($pay)."\" value=\"".nicevalue($r)."\">".check_plain($region)."</option>\n";
			}
		}
		
	}
	
	$formu = drupal_get_form('spmeteo_international_form');
	drupal_add_js(drupal_get_path('theme', 'wallydemo') . '/scripts/meteo.js', 'theme');
	
?>
<div id="internationale">
  <h2><span>Météo internationale</span></h2>
  <div class="recherche column">
    <form>
      <select id="continents">
        <?php print $html_continents; ?>
      </select>
      <select id="pays">
        <?php print $html_pays; ?>
      </select>
      <select id="regions">
        <?php print $html_regions; ?>
      </select>
    </form>
    <?php //print $my_data["form"];
print $formu; 
?>
  </div>
  <div id="villes">
    <div class="right">
      <h3>Asie</h3>
      <ul>
        <?php 
					print(affiche_pays($my_data['temperatures']['141'],$img_path));
					print(affiche_pays($my_data['temperatures']['112'],$img_path));
					print(affiche_pays($my_data['temperatures']['277'],$img_path));
				?>
      </ul>
      <h3>Amérique / Amérique du Sud</h3>
      <ul>
        <?php 
					print(affiche_pays($my_data['temperatures']['235'],$img_path));
					print(affiche_pays($my_data['temperatures']['238'],$img_path));
					print(affiche_pays($my_data['temperatures']['11'],$img_path));
				?>
      </ul>
    </div>
    <h3>Europe</h3>
    <ul>
      <?php 
					print(affiche_pays($my_data['temperatures']['221'],$img_path));
					print(affiche_pays($my_data['temperatures']['99'],$img_path));
					print(affiche_pays($my_data['temperatures']['84'],$img_path));
				?>
    </ul>
    <h3>Afrique</h3>
    <ul>
      <?php 
					print(affiche_pays($my_data['temperatures']['50'],$img_path));
					print(affiche_pays($my_data['temperatures']['161'],$img_path));
					print(affiche_pays($my_data['temperatures']['227'],$img_path));
				?>
    </ul>
  </div>
</div>
<?php 
//unset variables
unset($my_data);
?>