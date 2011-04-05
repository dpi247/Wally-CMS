<?php

function wallyedit_callback_preview($prenode){
  $prenode->ConvertToPopulate();
  return theme('node', $prenode->form_state[$prenode->form_state['type']]['#node']);
}
