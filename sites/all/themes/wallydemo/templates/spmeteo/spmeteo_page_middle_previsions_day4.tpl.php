<?php 
/*
 * Template pour la carte meteo du 3 prochains jours.
 * 
 * données: $my_data array();
 */
?>
<div id="previsions">
  <h2><span>Pr&eacute;visions</span></h2>
  <div><?php print $my_data['day4']['text'] ?></div>
</div>
<?php 
//unset variables
unset($my_data);
?>