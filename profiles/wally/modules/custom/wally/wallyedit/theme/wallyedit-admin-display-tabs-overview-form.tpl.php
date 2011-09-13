<?php

/**
 * @file block-admin-display-form.tpl.php
 * Default theme implementation to configure blocks.
 *
 * Available variables:
 * - $block_regions: An array of regions. Keyed by name with the title as value.
 * - $block_listing: An array of blocks keyed by region and then delta.
 * - $form_submit: Form submit button.
 * - $throttle: TRUE or FALSE depending on throttle module being enabled.
 *
 * Each $block_listing[$region] contains an array of blocks for that region.
 *
 * Each $data in $block_listing[$region] contains:
 * - $data->region_title: Region title for the listed block.
 * - $data->block_title: Block title.
 * - $data->region_select: Drop-down menu for assigning a region.
 * - $data->weight_select: Drop-down menu for setting weights.
 * - $data->throttle_check: Checkbox to enable throttling.
 * - $data->configure_link: Block configuration link.
 * - $data->delete_link: For deleting user added blocks.
 *
 * @see template_preprocess_block_admin_display_form()
 * @see theme_block_admin_display()
 */
?>
<?php
foreach ($tabs_infos as $tab_id => $tab) {
  drupal_add_js('misc/tableheader.js');
  drupal_add_tabledrag('blocks', 'match', 'sibling', 'my-element-tab-lev1', 'my-element-tab-lev1-'. $tab_id, NULL, FALSE);
  drupal_add_tabledrag('blocks', 'order', 'sibling', 'element-weight-lev1', 'element-weight-lev1-'. $tab_id);
}
?>
<table id="blocks" class="sticky-enabled">
	<thead>
		<tr>
			<th><?php print t('Element'); ?></th>
			<th><?php print t('Tab'); ?></th>
			<th><?php print t('Group'); ?></th>
			<th><?php print t('Weight'); ?></th>
			<th><?php print t('Display'); ?></th>
		</tr>
	</thead>
	<tbody>
	<?php $row = 0; ?>
	<?php foreach ($tabs_infos as $tab_id=>$tab):  ?>
		<tr class="tab tab-<?php print $tab_id?>">
			<td colspan="6" class="tab"><b><?php print $tab['label']; ?> </b></td>
		</tr>
		
		<tr
			class="tab-message tab-<?php print $tab['tid']?>-message <?php print empty($tabs_elements[$tab['tid']]) ? 'tab-empty' : 'tab-populated'; ?>">
			<td colspan="6"><em><?php print t('No tabs in this region'); ?> </em>
			</td>
		</tr>
		
		<?php if(isset($tabs_elements[$tab['tid']])):?>
		<?php foreach ($tabs_elements[$tab['tid']] as $delta => $data): ?>
		<tr
			class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?> <?php print $data->row_class ? ' '. $data->row_class : ''; ?>">
			<td><?php print $data->label; ?></td>
			<td><?php print $data->region_select; ?></td>
			<td><?php print $data->group_select; ?></td>
			<td><?php print $data->weight_select; ?></td>
			<td><?php print $data->wallyedit_select; ?></td>
			<td colspan="6"></td>
		</tr>
		<?php $row++; ?>
		<?php endforeach; ?>
		<?php endif;?>
		<?php endforeach; ?>
	</tbody>
</table>

<?php print $form_submit; ?>
