<?php
$sub_element=$form[0];
$type_name=$sub_element['type']["#value"];

module_load_include("inc",'wallyedit','includes/page_form_display_tabs');
$tabs=wyditadmin_get_fields_tree(wallyedit_get_default_profile(), $type_name);

$type=wydit_get_infos_type($type_name);
$cck_fields = $type['fields'];

$meta_tab_name="meta_".$profile_id.'_'.$node_type;
$no_tab_name="no_tab";
?>
<div id="accordion_container">
  <?php  foreach($tabs as $onglet=>$onglet_content):?>
    <div id="accordion-tab-<?php print $onglet?>" class="accordion-tab">
      <h2 class="accordion-tab-title"> Accordion Tab title</h2>
      <?php  foreach($tabs[$onglet]['elements'] as $group_id=>$group_content):?>
        <div class="accordion-group">
          <h2 class="accordion_group_title"><?php print $tabs[$onglet]['elements'][$group_id]["label"]?></h2>
          <?php  foreach($tabs[$onglet]['elements'][$group_id]['fields'] as $element_name=>$element_content):?>
            <?php if(isset($cck_fields[$element_name]['display_settings']['parent']) && !empty($cck_fields[$element_name]['display_settings']['parent'])):?>
              <?php print drupal_render($form[0][$cck_fields[$element_name]['display_settings']['parent']][$element_name])?>
            <?php else:?>
              <?php print drupal_render($form[0][$element_name])?>
            <?php endif;?>
          <?php endforeach;?>
        </div>
      <?php endforeach;?>
    </div>   
  <?php endforeach;?>
</div>
