<?php 

    foreach ($persons as $person) {

    	$content = "";
    	
    	$i = 0;

        $t = taxonomy_get_term($person->field_persontaxonomy[0]['value']);
        $taxo_path = taxonomy_term_path($t);

        if (isset($person->field_persontaxonomy)) {
          $title = "<a href='/".$taxo_path."'>".$person->title."</a>";
        } else {
          $title = $person->title;
        }
        
        				
        				
      	if (isset($person->field_photofile)) {
            $portrait = "<div class='protrait'>".theme('imagecache', 'sm_bispage_big', $person->field_photofile[0]["filepath"],$person->title)."</div>";
        } 
        
        if (isset($person->field_summary[0]["value"])) {
          $summary = "<span class='person_summary'>".$person->field_summary[0]["value"]."</span>";        	
        }
        
        if (isset($person->field_personnickname[0]["value"])) {
          $content .= "<li>Pseudo: ".$person->field_personnickname[0]["value"]."</li>";
        }

        if (isset($person->field_personbirthdate[0]["value"])) {
          $t = date('jS F Y',strtotime($person->field_personbirthdate[0]["value"]));
          $content .= "<li>Date de naissance: ".$t."</li>";
        }

        if (isset($person->field_personsex[0]["value"])) {
          $content .= "<li>Genre: ".$person->field_personsex[0]["value"]."</li>";
        }

        if (isset($person->field_personemail[0]["value"])) {
          $content .= "<li>Email: ".$person->field_personemail[0]["value"]."</li>";
        }

        if (isset($person->field_personwebsite[0]["url"])) {
          if (isset($person->field_personwebsite[0]["title"])) {
            $t = "<a href='".$person->field_personwebsite[0]["url"]."'>".$person->field_personwebsite[0]["title"]."</a>";
          } else {
            $t = "<a href='".$person->field_personwebsite[0]["url"]."'>".$person->field_personwebsite[0]["url"]."</a>";
          }
          $content .= "<li>Internet: ".$t."</li>";
        }

        if (isset($person->field_phonenumber[0]["value"])) {
          $content .= "<li>Téléphone: ".$person->field_phonenumber[0]["value"]."</li>";
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
