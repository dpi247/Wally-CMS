<?php

$html = "";

foreach ($my_data as $item ){
  if(is_array($item)){
  $html .="<tr>
       <td class=\"grisf5f5e1\" scope=\"row\">".$item['city']."</td>";
  
  foreach ($item['prices'] as $price){

    $html .= "<td>".$price." <abbr title=\"euro par litre\">&euro;/L</abbr></td>";
  } 
  $html .="</tr>";
  }  
  
}

?>
<div class="villes carburant service">
 <h2><span>Les stations les moins ch&egrave;res de Belgique</span></h2>
 <table border="0" cellpadding="0" cellspacing="0">
  <thead>
   <tr>
    <th class="th1" scope="col">Villes</th>
    <th class="grisb3b3b3" scope="col">Diesel</th>
    <th class="grisb3b3b3" scope="col">Eurosuper 95</th>
    <th class="grisb3b3b3" scope="col">Eurosuper 98</th>
    <th class="grisb3b3b3" scope="col">LPG</th>
   </tr>
 </thead>
  <?php print $html ; ?>
  </table>
</div>
<?php 
//ici unset variables
unset($my_data);
unset($html);
?>