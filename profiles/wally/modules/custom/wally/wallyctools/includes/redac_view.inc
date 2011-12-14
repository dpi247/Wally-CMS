<?php

class redac_view extends view  {
  

  /**
   * Execute the view's query.
   */
  function execute($display_id = NULL) {
    if (empty($this->built)) {
      if (!$this->build($display_id)) {
        return FALSE;
      }
    }

    if (!empty($this->executed)) {
      return TRUE;
    }

    // Let modules modify the view just prior to executing it.
    foreach (module_implements('views_pre_execute') as $module) {
      $function = $module . '_views_pre_execute';
      $function($this);
    }

    $query = db_rewrite_sql($this->build_info['query'], $this->base_table, $this->base_field, array('view' => &$this));
    $count_query = db_rewrite_sql($this->build_info['count_query'], $this->base_table, $this->base_field, array('view' => &$this));

    $args = $this->build_info['query_args'];

    vpr($query);


    // Check for already-cached results.
    if (!empty($this->live_preview)) {
      $cache = FALSE;
    }
    else {
      $cache = $this->display_handler->get_cache_plugin();
    }
    if ($cache && $cache->cache_get('results')) {
      vpr('Used cached results');
    }
    else {
      $items = array();
      if ($query) {
        $replacements = module_invoke_all('views_query_substitutions', $this);
        $query = str_replace(array_keys($replacements), $replacements, $query);
        $count_query = 'SELECT COUNT(*) FROM (' . str_replace(array_keys($replacements), $replacements, $count_query) . ') count_alias';

        if (is_array($args)) {
          foreach ($args as $id => $arg) {
            $args[$id] = str_replace(array_keys($replacements), $replacements, $arg);
          }
        }

        // Allow for a view to query an external database.
        if (isset($this->base_database)) {
          db_set_active($this->base_database);
          $external = TRUE;
        }

        $start = views_microtime();
        if (!empty($this->pager['items_per_page'])) {
          // We no longer use pager_query() here because pager_query() does not
          // support an offset. This is fine as we don't actually need pager
          // query; we've already been doing most of what it does, and we
          // just need to do a little more playing with globals.
          if (!empty($this->pager['use_pager']) || !empty($this->get_total_rows)) {
            $this->total_rows = db_result(db_query($count_query, $args)) - $this->pager['offset'];
          }

          if (!empty($this->pager['use_pager'])) {
            // dump information about what we already know into the globals
            global $pager_page_array, $pager_total, $pager_total_items;
            // total rows in query
            $pager_total_items[$this->pager['element']] = $this->total_rows;
            // total pages
            $pager_total[$this->pager['element']] = ceil($pager_total_items[$this->pager['element']] / $this->pager['items_per_page']);

            // What page was requested:
            $pager_page_array = isset($_GET['page']) ? explode(',', $_GET['page']) : array();

            // If the requested page was within range. $this->pager['current_page']
            // defaults to 0 so we don't need to set it in an out-of-range condition.
            if (!empty($pager_page_array[$this->pager['element']])) {
              $page = intval($pager_page_array[$this->pager['element']]);
              if ($page > 0 && $page < $pager_total[$this->pager['element']]) {
                $this->pager['current_page'] = $page;
              }
            }
            $pager_page_array[$this->pager['element']] = $this->pager['current_page'];
          }
          if(isset($this->pager['total_items_per_page']) and $this->pager['total_items_per_page']!=""){
            $offset = $this->pager['current_page'] * $this->pager['total_items_per_page'] + $this->pager['offset'];
          }
          else{
            $offset = $this->pager['current_page'] * $this->pager['items_per_page'] + $this->pager['offset'];
          }
          $result = db_query_range($query, $args, $offset, $this->pager['items_per_page']);

        }
        else {
          $result = db_query($query, $args);
        }

        $this->result = array();
        while ($item = db_fetch_object($result)) {
          $this->result[] = $item;
        }

        // If we already know how many items we have even if we did not run the
        // count query, go ahead and set that value:
        if (empty($this->pager['items_per_page'])) {
          $this->total_rows = count($this->result);
        }

        if (!empty($external)) {
          db_set_active();
        }
        $this->execute_time = views_microtime() - $start;
      }
      if ($cache) {
        $cache->cache_set('results');
      }
    }

    $this->executed = TRUE;
  }

  /**
   * Load a view from the database based upon either vid or name.
   *
   * This is a static factory method that implements internal caching for
   * view objects.
   *
   * @param $arg
   *  The name of the view or its internal view id (vid)
   * @param $reset
   *  If TRUE, reset this entry in the load cache.
   * @return A view object or NULL if it was not available.
   */
  function &load($arg, $reset = FALSE) {
    static $cache = array();

    // We want a NULL value to return TRUE here, so we can't use isset() or empty().
    if (!array_key_exists($arg, $cache) || $reset) {
      $where = (is_numeric($arg) ? "vid =  %d" : "name = '%s'");
      $data = db_fetch_object(db_query("SELECT * FROM {views_view} WHERE $where", $arg));
      if (empty($data)) {
        $cache[$arg] = NULL;
      }
      else {
        $view = new redac_view();
        $view->load_row($data);
        $view->type = t('Normal');
        // Load all of our subtables.
        foreach ($view->db_objects() as $key) {
          $object_name = "views_$key";
          $result = db_query("SELECT * FROM {{$object_name}} WHERE vid = %d ORDER BY position", $view->vid);

          while ($data = db_fetch_object($result)) {
            $object = new $object_name(FALSE);
            $object->load_row($data);

            // Because it can get complicated with this much indirection,
            // make a shortcut reference.
            $location = &$view->$key;

            // If we have a basic id field, load the item onto the view based on
            // this ID, otherwise push it on.
            if (!empty($object->id)) {
              $location[$object->id] = $object;
            }
            else {
              $location[] = $object;
            }
          }
        }

        $view->loaded = TRUE;
        $cache[$arg] = $view;
      }
    }

    return $cache[$arg];
  }

}