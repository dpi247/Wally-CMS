<?php
// $Id: content-admin-display-overview-form.tpl.php,v 1.1.2.3 2008/10/09 20:58:26 karens Exp $
?>

<div>
  <?php print $help; ?>
</div>
<?php if ($rows): ?>
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
            <td><?php print isset($row->wallyedit) ? $row->wallyedit : ''; ?></td>
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
  <?php print $submit; ?>
 <?php endif; ?>
