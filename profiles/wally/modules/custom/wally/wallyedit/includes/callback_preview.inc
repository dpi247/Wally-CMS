<?php

function wallyedit_callback_preview($prenode){
  $node = $prenode->ConvertToPopulate();
  return theme('node', $node);
}
