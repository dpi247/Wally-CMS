<?php

function wallyedit_callback_batchimportselector($prenode, $js = FALSE, $current_profile = 0){
  $js = $js ? 'ajax' : 0;
  module_load_include('inc', 'wallyedit', '/includes/callback_edition');
  if ($return = wallyedit_callback_node_edition($prenode, $js, $current_profile, FALSE, '/node/@cache_name/wydit_batchimportselector/@js/@current_profile', 'wydit_form_batchimportselector_form')) {
    return $return;
  }
}
