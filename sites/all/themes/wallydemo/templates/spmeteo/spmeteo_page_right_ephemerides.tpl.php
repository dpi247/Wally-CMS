<?php 
/*
 * Template pour la carte meteo du 3 prochains jours.
 * 
 * données: $my_data array();
 */
?>
<div class="right colright">
  <div id="dicton">
    <h2><span>Dicton du jour</span></h2>
    <div><?php print $my_data["divers"]["dicton"]; ?></div>
  </div>
  <div id="marees">
    <h2><span>Mar&eacute;es</span></h2>
    <table border="0" cellspacing="0" cellpadding="0">
      <thead>
        <tr>
          <th class="th1">Mar&eacute;e</th>
          <th class="grisb3b3b3">Ostende</th>
        </tr>
      </thead>
      <tr>
        <td class="grisf5f5e1" scope="row">Haute</td>
        <td><?php print $my_data["divers"]["maree_os_h1"]; ?> <?php print $my_data["divers"]["maree_os_h2"]; ?></td>
      </tr>
      <tr>
        <td class="grisf5f5e1" scope="row">Basse</td>
        <td><?php print $my_data["divers"]["maree_os_b1"]; ?> <?php print $my_data["divers"]["maree_os_b2"]; ?></td>
      </tr>
      </table>
      <table border="0" cellpadding="0" cellspacing="0">
        <thead>
          <tr>
            <th class="th1">Mar&eacute;e</th>
            <th class="grisb3b3b3">Anvers</th>
          </tr>
        </thead>
        <tr>
          <td class="grisf5f5e1" scope="row">Haute</td>
          <td><?php print $my_data["divers"]["maree_an_h1"]; ?> <?php print $my_data["divers"]["maree_an_h2"]; ?></td>
        </tr>
        <tr>
          <td class="grisf5f5e1" scope="row"> Basse</td>
          <td><?php print $my_data["divers"]["maree_an_b1"]; ?> <?php print $my_data["divers"]["maree_an_b2"]; ?></td>
        </tr>
      </table>
  </div>
</div>
<div class="column">
  <div id="soleil">
    <h2><span>Soleil</span></h2>
    <table border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td class="grisf5f5e1" scope="row">Lever</td>
        <td><?php print $my_data["divers"]["sun_up"]; ?></td>
      </tr>
      <tr>
        <td class="grisf5f5e1" scope="row"> Coucher</td>
        <td><?php print $my_data["divers"]["sun_down"]; ?></td>
      </tr>
    </table>
  </div>
  <div id="lune">
    <h2><span>Lune</span></h2>
    <table border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td class="grisf5f5e1" scope="row">Lever</td>
        <td class="alignright"><?php print $my_data["divers"]["lune_up"]; ?></td>
      </tr>
      <tr>
        <td class="grisf5f5e1" scope="row">Coucher</td>
        <td class="alignright"><?php print $my_data["divers"]["lune_down"]; ?></td>
      </tr>       
      <tr>
        <td class="grisf5f5e1" scope="row">Premier quartier</td>
        <td class="alignright"><?php print $my_data["divers"]["lune_pq"]; ?></td>
      </tr>
      <tr>
        <td class="grisf5f5e1" scope="row"> Pleine lune</td>
        <td class="alignright"><?php print $my_data["divers"]["lune_pl"]; ?></td>
      </tr>
      <tr>
        <td class="grisf5f5e1" scope="row"> Nouvelle lune</td>
        <td class="alignright"><?php print $my_data["divers"]["lune_nl"]; ?></td>
      </tr>
      <tr>
        <td class="grisf5f5e1" scope="row"> Dernier quartier</td>
        <td class="alignright"><?php print $my_data["divers"]["lune_dq"]; ?><br/></td>
      </tr>
    </table>
  </div>
	<div id="alertes">
	  <h2><span>Alertes</span></h2>
	  <table border="0" cellspacing="0" cellpadding="0">
	    <tr>
	      <td class="grisf5f5e1" scope="row">Ozone</td>
	      <td><?php print $my_data["divers"]["ozone"]; ?></td>
	    </tr>
	    <tr>
	      <td class="grisf5f5e1" scope="row">Indice U.V</td>
	      <td><?php print $my_data["divers"]["uv"]; ?></td>
	    </tr>
	    <tr>
	      <td class="grisf5f5e1" scope="row">Pollen<br/></td>
	      <td><?php print $my_data["divers"]["pollen"]; ?></td>
	    </tr>
	    <tr>
	      <td class="grisf5f5e1" scope="row">Verglas</td>
	      <td><?php print $my_data["divers"]["verglas"]; ?></td>
	    </tr>
	    <tr>
	      <td class="grisf5f5e1" scope="row">C.O.</td>
	      <td><?php print $my_data["divers"]["co"]; ?></td>
	    </tr>
	  </table>
	</div> 
</div>
<?php 
//unset variables
unset($my_data);
?>