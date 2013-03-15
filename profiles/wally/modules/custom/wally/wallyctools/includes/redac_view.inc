<?php

class redac_view extends view  {
  

 
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