<?php 

drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/services.css','file','screen');


$map_url = drupal_get_path_alias("services/trafic");
$stats_url = drupal_get_path_alias("services/trafic/stats");
$traj_url = drupal_get_path_alias("services/trafic/trajets");
$prev_url = drupal_get_path_alias("services/trafic/previsions");
?>
<div class="submenu trafic service">
  <ul>
    <li class="carte"><span><a linkindex="131" href="/<?php print $map_url; ?>">Carte</a></span></li>
    <li class="info"><span><a linkindex="134" href="/<?php print $stats_url; ?>">Info trafic</a></span></li>
    <li class="previsions"><span><a linkindex="133" href="/<?php print $prev_url; ?>">Pr&eacute;visions</a></span></li>
    <li class="trajets"><span><a linkindex="132" href="/<?php print $traj_url; ?>">Trajets</a></span></li>
  </ul>
</div>