<?php 

    foreach ($entities as $entity) {

    	$content = "";
    	
    	$i = 0;

        $t = taxonomy_get_term($entity->field_entity[0]['value']);
        $taxo_path = taxonomy_term_path($t);

        if (isset($entity->field_entity)) {
          $title = "<a href='/".$taxo_path."'>".$entity->title."</a>";
        } else {
          $title = $entity->title;
        }
        
        				
        				
      	if (isset($entity->field_main_picture)) {
            $portrait = "<div class='protrait'>".theme('imagecache', 'sm_bispage_big', $entity->field_main_picture[0]["filepath"],$entity->title)."</div>";
        } 
        
        if (isset($entity->field_objectdescription[0]["value"])) {
          $summary = "<span class='person_summary'>".$entity->field_objectdescription[0]["value"]."</span>";        	
        }
        
        if (isset($entity->field_entityservices[0]["value"])) {
          $content .= "<li>Services: ".$entity->entityservices[0]["value"]."</li>";
        }

        if (isset($entity->externalreference[0]["value"])) {
          $t = $person->externalreference[0]["value"];
          $content .= "<li>External Reference: ".$t."</li>";
        }
        
        
?>
	<div class="fiche_personne brique">
  <h2><?php  print $title; ?></h2>
  <?php if ($block): ?>
    <h3><?php print $title; ?>:</h3>
  <?php endif; ?>  
  <div class="fiche_personne">
  	<?php print $portrait; ?>
  	<?php print $summary; ?>
    <ul><?php print $content; ?></ul>
  </div>
</div>
<?php 
    }	
 ?>
