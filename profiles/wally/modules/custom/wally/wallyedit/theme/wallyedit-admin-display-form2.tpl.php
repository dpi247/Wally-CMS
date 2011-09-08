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

  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'wallyedit') .'/js/wallyedit.js');
  drupal_add_css(drupal_get_path('module', 'wallyedit') .'/css/wallyedit.css');
  foreach ($element_onglets as $region => $title) {
    drupal_add_tabledrag('blocks', 'match', 'sibling', 'my-element-onglet-lev1', 'my-element-onglet-lev1-'. $region, NULL, FALSE);
    drupal_add_tabledrag('blocks', 'order', 'sibling', 'element-weight-lev1', 'element-weight-lev1-'. $region);
  }
?>
<table id="blocks" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Element'); ?></th>
      <th><?php print t('Onglet'); ?></th>
      <th><?php print t('Weight'); ?></th>
      <th><?php print t('Widget'); ?></th>
      <th><?php print t('Operations'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
    <?php foreach ($element_onglets as $region => $title): ?>
      <tr class="onglet onglet-<?php print $region?>">
        <td colspan="5" class="onglet"><?php print $title; ?></td>
      </tr>
      <tr class="onglet-message onglet-<?php print $region?>-message <?php print empty($block_listing[$region]) ? 'region-empty' : 'region-populated'; ?>">
        <td colspan="5>"><em><?php print t('No blocks in this region'); ?></em></td>
      </tr>
      <?php foreach ($element_listing[$region] as $delta => $data): ?>
        <?php if($data->has_level2):?>
         <tr  class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?> <?php print $data->row_class ? ' '. $data->row_class : ''; ?>">
            <td class="block"><?php print $data->block_title; ?></td>
                 <td><?php print $data->region_select; ?></td>
                 <td><?php print $data->weight_select; ?></td>
                 <td><?php print $data->wallyedit_select; ?></td>
                 <td><?php print $data->configure_link; ?></td>
                 <td colspan="6">
             <table id="blocks_<?php print $delta?>">
             <?php   
               dsm($subdata_key);
               drupal_add_tabledrag('blocks_'.$delta, 'order', 'sibling', 'element-weight-lev2-'.$delta, 'element-weight-lev2-'. $delta);
             ?>
                  
               <thead> 
                <tr class="table-fake-thead">
                   <th><?php print t('Element'); ?></th>
                   <th><?php print t('Weight'); ?></th>
                   <th><?php print t('Widget'); ?></th>
                   <th><?php print t('Operations'); ?></th>
                 </tr>
               </thead>
               <tbody>
                 <?php foreach($data->level2 as $subdata_key=>$subdata) :?>
                   <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?> <?php print $subdata->row_class ? ' '. $subdata->row_class : ''; ?> ">
                     <td class="block"><?php print $subdata->block_title; ?></td>
                     <td><?php print $subdata->weight_select; ?></td>
                     <td><?php print $subdata->configure_link; ?></td>
                   </tr>
	            <?php endforeach;?>
			  </tbody>
			</table>
			</td>
          </td>
        </tr>
        <?php else:?>
          <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?> <?php print $data->row_class ? ' '. $data->row_class : ''; ?>">
            <td class="block"><?php print $data->block_title; ?></td>
            <td><?php print $data->region_select; ?></td>
            <td><?php print $data->weight_select; ?></td>
            <td><?php print $data->configure_link; ?></td>
            <td colspan="6">
            </td>
          </tr>
      <?php endif;?>
      <?php $row++; ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </tbody>
</table>

<?php print $form_submit; ?>
