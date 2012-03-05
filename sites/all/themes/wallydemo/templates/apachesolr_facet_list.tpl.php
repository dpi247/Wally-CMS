<!-- 
Bloc qui affiche les filtres 
-->

<div class="search">
  <ul>
    <?php
      drupal_add_js(drupal_get_path('module', 'apachesolr') . '/apachesolr.js'); 
      foreach($items as $item) {
        if(is_array($item)) echo $item["data"];
        else echo $item;
      }
      ?>
  </ul>
</div>
