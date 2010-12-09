<?php

/**
 * Implementation of hook_user_default_roles().
 */
function wallyrolespermissions_user_default_roles() {
  $roles = array();

  // Exported role: editor user
  $roles['editor user'] = array(
    'name' => 'editor user',
  );

  return $roles;
}
