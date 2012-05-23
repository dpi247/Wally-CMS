<?php

function wallyedit_page_node_creation($type, $js = FALSE, $current_profile = 0){
  if ($current_profile == 0) {
    drupal_set_message(t('Profile not set.'), 'error');
    drupal_goto('node/add/'.$type);
  }

  module_load_include('inc', 'wallyedit', 'includes/Prenode');
  $node->type = str_replace('-', '_', $type);
  $prenode = new Prenode(arg(5), FALSE, $current_profile);
  $prenode->setNode($node);

  $js = $js ? 'ajax' : 0;
  module_load_include('inc', 'wallyedit', 'includes/callback_edition');
  if ($return = wallyedit_callback_node_edition($prenode, $js, $current_profile, FALSE, '/node/add2/@type/@js/@current_profile/@cache_name')) {
    return $return;
  }
}
