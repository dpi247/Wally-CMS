<?php

if (isset($_GET['nid']) && is_numeric($_GET['nid'])) {
  
  function wallystat_cache_entry($cid, $datas) {
    $cache_inc = variable_get('cache_inc', './includes/cache.inc');

    if (true||strstr($cache_inc, '/memcache.inc')) {
      $cached_lock = cache_get('wallystat_lock');
      if ($cached_lock && isset($cached_lock->data) && !empty($cached_lock->data) && is_numeric($cached_lock->data)) {
        $lock = $cached_lock->data;
      } else {
        $lock = 1;
      }

      $cached_entries_index = cache_get('wallystat_entries_index_'.$lock);
      if ($cached_entries_index && isset($cached_entries_index->data) && !empty($cached_entries_index->data)) {
        $entries_index = $cached_entries_index->data;
      } else {
        $entries_index = '';
      }
      $entries_index .= $cid.'--';
      cache_set('wallystat_entries_index_'.$lock, $entries_index);
    }

    cache_set($cid, $datas, 'cache_wallystat_tempdata');
  }

  function wallystat_updater_shutdown() {
    if ($last_error = error_get_last()) {
      if (function_exists('cache_set') && $last_error['type'] <= E_PARSE) {
        wallystat_cache_entry(uniqid('error_'), $last_error);
      }
    }
  }

  register_shutdown_function('wallystat_updater_shutdown');

  //Ceci n'est pas encore utilisé dans la v1... le but est de créer les trending topics.
  if (isset($_GET['terms'])){
    //ON s'assure que tous les termes ici sont bien des id numériques et on recrée $safe_terms avec les valeurs vérifiées.
    $term_array = explode(' ', $_GET['terms']);
    if (is_array($term_array)) {
      $safe_terms = array();
      foreach ($term_array as $term) {
        if (is_numeric($term)) {
          $safe_terms[] = $term;
        }
      }
      $terms = implode(' ', $safe_terms);
    }
  }

  $wally_install_root = $_SERVER['DOCUMENT_ROOT'];
  chdir($wally_install_root);
  include_once ('./includes/bootstrap.inc');
  drupal_bootstrap(DRUPAL_BOOTSTRAP_CONFIGURATION);
  $cache_inc = variable_get('cache_inc', './includes/cache.inc');
  switch (true) { // memcache.inc needs less bootstrap than cache.inc
    case strstr($cache_inc, '/memcache.inc') :
      drupal_bootstrap(DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE);
      break;
    case strstr($cache_inc, '/cache.inc') :
    default :
      drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
      break;
  }

  $datas = array();
  $datas['nid'] = $_GET['nid'];
  $datas['title'] = $_GET['title'];
  for ($i = 1; $i <= 10; $i++) {
    $id = ($i == 10) ? 'param'.$i : 'param0'.$i;
    $datas[$id] = $_GET[$id];
  }
  if (isset($terms) && $terms != '') {
    $datas['terms'] = $terms;
  }

  $cid = uniqid('wallystat_'.$_GET['nid'].'_');
  wallystat_cache_entry($cid, $datas);
}
