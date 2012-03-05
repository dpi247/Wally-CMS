<?php

drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/services.css','file','screen');

/* Récupération du path
 * 
 */
$theme_path = drupal_get_path('theme', 'wallydemo');



$html_carburant = "";
$html_combustible = "";

  if(is_array($my_data['items'])){
    foreach ($my_data['items'] as $item){
        if ($item['id'] < 5){
          $html_carburant .= "<tr>
                  <td class=\"grisf5f5e1\" scope=\"row\">".$item['name']."</td>
                  <td>".$item['price']." &euro; <img src=\"".$theme_path."/images/services/ico_carbu_".$item['img'].".gif\" width=\"12\" height=\"13\" alt=\"".$item['alt']."\" /></td>
          </tr>";
        }else{
          $html_combustible .= "<tr>
                  <td class=\"grisf5f5e1\" scope=\"row\">".$item['name']."</td>
                  <td>".$item['price']." &euro; <img src=\"".$theme_path."/images/services/ico_carbu_".$item['img'].".gif\" width=\"12\" height=\"13\" alt=\"".$item['alt']."\" /></td>
          </tr>"; 
        }
    }
  }
?>

<div class="column prix carburant service">
  <h2><span>Prix officiels du <?php print $my_data['date'] ; ?></span></h2>
  <table border="0" cellpadding="0" cellspacing="0">
   <thead>
    <tr>
     <th class="th1" scope="col">Carburants</th>
     <th class="grisb3b3b3" scope="col">Prix</th>
    </tr>
   </thead>
   <?php print $html_carburant; ?>
   </table>
  <table border="0" cellpadding="0" cellspacing="0">
   <thead>
    <tr>
     <th class="th1" scope="col">Combustibles</th>
     <th class="grisb3b3b3" scope="col">Prix</th>
    </tr>
   </thead>
   <?php print $html_combustible; ?>   
  </table>   
</div>
<?php 
//ici unset variables
unset($my_data);
unset($html_carburant);
unset($html_combustible);
?>