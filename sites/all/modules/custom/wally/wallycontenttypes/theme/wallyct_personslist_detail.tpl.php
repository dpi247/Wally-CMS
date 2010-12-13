<?php
/**
 * Wally default template for rendering Presons Lists 
 * <ul><li></li></ul> style. 
 */

if (!function_exists('wally_tpl_persons_detail')) {
  function wally_tpl_persons_detail($persons) {

    $content = "";

    foreach ($persons as $person) {

        $i = 0;

        $t = taxonomy_get_term($person->field_persontaxonomy[0]['value']);
        $taxo_path = taxonomy_term_path($t);

        if (isset($person->field_persontaxonomy)) {
          $title = "<a href='".$taxo_path."'>".$person->title."</a>";
        } else {
          $title = $person->title;
        }

        $content .= "<table>";
        $content .= "<thead><tr><th colspan='3'>".$title."</th></tr></thead>";
        $content .= "<tbody>";

        if (isset($person->field_main_picture)) {
            $content .= "<tr class='".(($i++%2) ? "even" : "odd")."'><td><img src='/".$person->field_main_picture[0]["filepath"]."' width='50' align=tle'left'></td><td>".$person->field_personfirstname[0]["value"]."</td><td>".$person->field_personlastname[0]["value"]."</td></tr>";
        } else {
            $content .= "<tr class='".(($i++%2) ? "even" : "odd")."'><td><img src='/".path_to_theme()."/images/default_person_pic.jpg' width='50' align='left'></td><td>".$person->field_personfirstname[0]["value"]."</td><td>".$person->field_personlastname[0]["value"]."</td></tr>";
        }

        if (isset($person->field_personnickname[0]["value"])) {
          $content .= "<tr class='".(($i++%2) ? "even" : "odd")."'><td colspan='2' style='align:left'>Nickname:</td><td>".$person->field_personnickname[0]["value"]."</td></tr>";
        }

        if (isset($person->field_personbirthdate[0]["value"])) {
          $t = date('jS F Y',strtotime($person->field_personbirthdate[0]["value"]));
          $content .= "<tr class='".(($i++%2) ? "even" : "odd")."'><td colspan='2' style='align:left'>Birth date:</td><td>".$t."</td></tr>";
        }

        if (isset($person->field_personsex[0]["value"])) {
          $content .= "<tr class='".(($i++%2) ? "even" : "odd")."'><td colspan='2' style='align:left'>Genre:</td><td>".$person->field_personsex[0]["value"]."</td></tr>";
        }

        if (isset($person->field_personemail[0]["value"])) {
          $content .= "<tr class='".(($i++%2) ? "even" : "odd")."'><td colspan='2' style='align:left'>Email:</td><td>".$person->field_personemail[0]["value"]."</td></tr>";
        }

        if (isset($person->field_personwebsite[0]["url"])) {
          if (isset($person->field_personwebsite[0]["title"])) {
            $t = "<a href='".$person->field_personwebsite[0]["url"]."'>".$person->field_personwebsite[0]["title"]."</a>";
          } else {
            $t = "<a href='".$person->field_personwebsite[0]["url"]."'>".$person->field_personwebsite[0]["url"]."</a>";
          }
          $content .= "<tr class='".(($i++%2) ? "even" : "odd")."'><td colspan='2' style='align:left'>Website:</td><td>".$t."</td></tr>";
        }

        if (isset($person->field_phonenumber[0]["value"])) {
          $content .= "<tr class='".(($i++%2) ? "even" : "odd")."'><td colspan='2' style='align:left'>Phone:</td><td>".$person->field_phonenumber[0]["value"]."</td></tr>";
        }

        $content .= "</tbody></table>";
    }
    
    return $content; 
  }
}
print wally_tpl_persons_detail($persons);
?>
