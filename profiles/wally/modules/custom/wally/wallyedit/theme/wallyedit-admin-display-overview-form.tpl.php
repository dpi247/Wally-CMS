<?php
// $Id: content-admin-display-overview-form.tpl.php,v 1.1.2.3 2008/10/09 20:58:26 karens Exp $
require_once './'.drupal_get_path('module', 'content').'/includes/content.admin.inc';
$order = _content_overview_order($form, $form['#field_rows'], $form['#group_rows']);
$rows = array();

// Identify the 'new item' keys in the form, they look like
// _add_new_field, add_new_group.
$keys = array_keys($form);
$add_rows = array();
foreach ($keys as $key) {
  if (substr($key, 0, 4) == '_add') {
    $add_rows[] = $key;
  }
}
while ($order) {
  $key = reset($order);
  $element = &$form[$key];
  

  // Only display the 'Add' separator if the 'add' rows are still
  // at the end of the table.
  if (!isset($added_separator)) {
    $remaining_rows = array_diff($order, $add_rows);
    if (empty($remaining_rows) && empty($element['#depth'])) {
      $row = new stdClass();
      $row->row_type = 'separator';
      $row->class = 'tabledrag-leaf region';
      $rows[] = $row;
      $added_separator = TRUE;
    }
  }

  if (isset($element)) {
    $row = new stdClass();
    // Add target classes for the tabledrag behavior.
    $element['weight']['#attributes']['class'] = 'field-weight';
    $element['parent']['#attributes']['class'] = 'group-parent';
    $element['hidden_name']['#attributes']['class'] = 'field-name';
    // Add target classes for the update selects behavior.
    switch ($element['#row_type']) {
      case 'add_new_field':
        $element['type']['#attributes']['class'] = 'content-field-type-select';
        $element['widget_type']['#attributes']['class'] = 'content-widget-type-select';
        break;
      case 'add_existing_field':
        $element['field_name']['#attributes']['class'] = 'content-field-select';
        $element['widget_type']['#attributes']['class'] = 'content-widget-type-select';
        $element['label']['#attributes']['class'] = 'content-label-textfield';
        break;
    }
    foreach (element_children($element) as $child) {
      $row->{$child} = drupal_render($element[$child]);
    }
    $row->label_class = 'label-'. strtr($element['#row_type'], '_', '-');
    $row->row_type = $element['#row_type'];
    $row->indentation = theme('indentation', isset($element['#depth']) ? $element['#depth'] : 0);
    $row->class = 'draggable';
    $row->class .= isset($element['#disabled_row']) ? ' menu-disabled' : '';
    $row->class .= isset($element['#add_new']) ? ' content-add-new' : '';
    $row->class .= isset($element['#leaf']) ? ' tabledrag-leaf' : '';
    $row->class .= isset($element['#root']) ? ' tabledrag-root' : '';

    $rows[] = $row;
  }
  array_shift($order);
}
//dsm($form);
?>
<div>
  <?php print $help; ?>
</div>
<table id="content-field-overview" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Label'); ?></th>
      <th><?php print t('Name'); ?></th>
      <th><?php print t('Type'); ?></th>
      <th><?php print t('WallyEdit'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php
    $count = 0;
    foreach ($rows as $row): ?>
      <tr class="<?php print $count % 2 == 0 ? 'odd' : 'even'; ?> <?php print $row->class ?>">
      <?php
      switch ($row->row_type):
        case 'field': ?>
          <td>
            <?php print $row->indentation; ?>
            <span class="<?php print $row->label_class; ?>"><?php print $row->label; ?></span>
          </td>
          <td><?php print $row->field_name; ?></td>
          <td><?php print $row->type; ?></td>
          <td><?php print $row->wallyedit; ?></td>
          <?php break;
        case 'group': ?>
          <td>
            <?php print $row->indentation; ?>
            <span class="<?php print $row->label_class; ?>"><?php print $row->label; ?></span>
          </td>
          <td><?php print $row->group_name; ?></td>
          <td><?php print $row->group_type; ?></td>
          <td><?php print $row->wallyedit; ?></td>
          <?php break;
        case 'extra': ?>
          <td>
            <?php print $row->indentation; ?>
            <span class="<?php print $row->label_class; ?>"><?php print $row->label; ?></span>
          </td>
          <td colspan="2"><?php print $row->description; ?></td>
          <td><?php print $row->wallyedit; ?></td>
          <?php break;
        case 'separator': ?>
          <td colspan="5" class="region"><?php print t('Add'); ?></td>
          <?php break;
      endswitch; ?>
      </tr>
      <?php $count++;
    endforeach; ?>
  </tbody>
</table>
<?php print drupal_render($form['submit']); ?>
<?php print drupal_render($form['submsit']); ?>

<?php print $submit; ?>
