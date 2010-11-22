<?php

/**
 * Wally default template for rendering Packages Autors List 
 */
if (!function_exists('wally_tpl_authors_list')) {
  function wally_tpl_authors_list($authors) {
    $content = "";
    foreach ($authors as $author) {

      if (isset($author->field_persontaxonomy)) {
        $content .= "<a href='/taxonomy/term/".$author->field_persontaxonomy[0]['value']."'>".$author->title."</a>";
      } else {
        $content .= $author->title;
      }

      if (isset($author->field_personwebsite)) {
        $content .= " (<a href='".$author->field_personwebsite[0]["url"]."'>".$author->field_personwebsite[0]["title"]."</a>)";
        } 
    }
    return "<span class='thmr_call'>".$content."</span>"; 
  }
}  

?>
<span class="thmr_call">
<?php print wally_tpl_authors_list($authors); ?>
</span>
