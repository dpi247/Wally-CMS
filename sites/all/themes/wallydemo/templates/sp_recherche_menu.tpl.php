<li class="services recherche_menu">
  <?php 
    variable_set('apachesolr_panels_search_page', variable_get('apachesolr_path', '/solr'));
    //print str_replace('Search','OK',drupal_get_form("apachesolr_panels_search_block"));
    print drupal_get_form("apachesolr_panels_search_block");
  ?>
</li> 