<?php

/**
 * This hook is called when a taxonomy term is elected as destination,
 * when an elected term is updated or when it is deleted.
 * 
 * @param string $op
 *   The operation performed on the term ('created', 'updated' or 'deleted')
 * @param array $dest_term
 *   The term in the destination vocabulary
 * @param array $taxonomy_term
 *   The elected term from any vocabulary
 */
function hook_topic_destination($op, $dest_term, $taxonomy_term) {
  switch($op) {
    case 'create':
      // Do something when the new destination term is created, for example :      
      // Add the newly created destination to every node taged with the taxonomy term
      $taged_nodes = get_taged_nodes($taxonomy_term);
      foreach ($taged_nodes as $taged_node) {
        $taged_node->field_destinations[] = array(
          'tid' => $dest_term['tid'],
          'target' => 'default_destination_block',
          'layout' => 'medium',
          'rank' => 'DNC0',
        );
        node_save($taged_node);
      }
      
      break;
      
    case 'update':
      break;
      
    case 'delete':
      // Do something when the destination term is deleted, for example :
      // Store nodes in this destination in a storage destination
      $storage_destination = $dest_term;
      $storage_destination['name'] .= ' storage';
      unset($storage_destination['tid']);
      taxonomy_save_term($storage_destination);
      
      $dest_nodes = get_dest_nodes($dest_term);
      foreach ($dest_nodes as $dest_node) {
        foreach ($dest_node->field_destination as &$field_dest) {
          if ($field_dest['tid'] == $dest_node['tid']) {
            $field_dest['tid'] = $storage_destination['tid'];
            break;
          }
        }
        node_save($dest_node);
      }
      
      break;
  }
}
