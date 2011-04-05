<?php

function wallyedit_callback_preview($prenode){
  $node = $prenode->ReturnPopulated();
  return theme('node', $node);
}
