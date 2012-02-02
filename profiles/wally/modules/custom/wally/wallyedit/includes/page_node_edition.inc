<?php

function wallyedit_page_node_edition($prenode, $js = FALSE, $current_profile = 0){
  $js = $js ? 'ajax' : 0;
  module_load_include('inc', 'wallyedit', '/includes/callback_edition');
  if ($return = wallyedit_callback_node_edition($prenode, $js, $current_profile, TRUE, '/node/@cache_name/edit2/@js/@current_profile')) {
    return $return;
  }
}
