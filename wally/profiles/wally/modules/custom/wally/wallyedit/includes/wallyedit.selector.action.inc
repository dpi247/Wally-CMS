<?php

function wallyedit_selector_action_info() {
  return array('wallyedit_selector_action' => array(
    'type' => 'node',
    'description' => t('Add those node to the prenode'),
    'configurable' => FALSE,
  ));
}
/*
function wallyedit_selector_action_form($context) {
  $roles = selector(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);  // Can't edit authenticated role.
  if (module_exists('role_delegation')) {
    foreach ($roles as $rid => $role) {
      if (!user_access('assign all roles') && !user_access(_role_delegation_make_perm($role))) {
        unset($roles[$rid]);
      }
    }
  }

  $form['add_roles'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Add roles'),
    '#description' => t('Choose one or more roles you would like to assign to the selected users.'),
    '#options' => $roles,
    '#size' => 5
  );
  $form['remove_roles'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Remove roles'),
    '#description' => t('Choose one or more roles you would like to remove from the selected users.'),
    '#options' => $roles,
    '#size' => 5
  );
  return $form;
}

function wallyedit_selector_action_validate($form, $form_state) {
  if (!$form_state['values']['add_roles'] && !$form_state['values']['remove_roles']) {
    form_set_error('add_roles', t('You have not chosen any role to add or remove. Please select something to do.'));
  }
}

function wallyedit_selector_action_submit($form, $form_state) {
  return array(
    'add_roles' => array_filter($form_state['values']['add_roles']),
    'remove_roles' => array_filter($form_state['values']['remove_roles']),
  );
}
*/

function wallyedit_selector_action($node, $context) {
  /*
  $roles = $user->roles;
  $selected = (is_array($context['add_roles']) ? $context['add_roles'] : array()) +
              (is_array($context['remove_roles']) ? $context['remove_roles'] : array());
  $result = db_query("SELECT rid, name FROM {role} WHERE rid IN (%s)", implode(',', array_keys($selected)));
  while ($role = db_fetch_object($result)) {
    if (isset($context['add_roles'][$role->rid])) {
      $add_roles[$role->rid] = $role->name;
    }
    if (isset($context['remove_roles'][$role->rid])) {
      $remove_roles[$role->rid] = $role->name;
    }
  }
  if (!empty($add_roles)) {
    $roles += $add_roles;
  }
  if (!empty($remove_roles)) {
    $roles = array_diff($roles, $remove_roles);
  }
  user_save($user, array('roles' => $roles));
  */
}

