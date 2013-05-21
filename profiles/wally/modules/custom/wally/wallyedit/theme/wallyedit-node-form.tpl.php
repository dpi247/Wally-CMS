<?php
  $meta_tab_name = 'meta_'.$profile_id.'_'.$node_type;
  $no_tab_name = 'no_tab';
?>

<div id="column-main-left">
  <div id="scroller-header">
      <?php foreach($onglets_struct as $onglet => $onglet_content): ?>
        <?php if($onglet != $meta_tab_name && $onglet != $no_tab_name): ?>
          <a href="#onglet-<?php print $onglet; ?>" rel="onglet"><?php print $onglet_content['label']; ?></a>
        <?php endif;?>
      <?php endforeach;?>
  </div>
  <div id="scroller-body">
    <div id="mask">
      <div id="onglet">
        <?php foreach($onglets_struct as $onglet => $onglet_content): ?>
          <?php if($onglet != $meta_tab_name && $onglet != $no_tab_name): ?>
          <div class="content-tabs" id="onglet-<?php print $onglet; ?>">
             
            <?php if(count($onglets_struct[$onglet]['elements']['no_group']['fields']) > 0):?>
            <div class="group">
              <div class="group_content">
              <?php  foreach($onglets_struct[$onglet]['elements']['no_group']['fields'] as $element_name => $element_content): ?>
                <?php print drupal_render($form[$node_type][$element_name]); ?>
              <?php endforeach;?>
              </div>
            </div>
            <?php endif;?>
             
            <?php foreach($onglets_struct[$onglet]['elements'] as $group_id => $group_content): ?>
              <?php if($group_id != 'no_group'): ?>
              <div class="group">
                <h2 class="title  title-group "><span><?php print $onglets_struct[$onglet]['elements'][$group_id]["label"]; ?></span></h2>
                <div class="group_content">
                  <?php  foreach($onglets_struct[$onglet]['elements'][$group_id]['fields'] as $element_name => $element_content): ?>
                    <?php print drupal_render($form[$node_type][$element_name]); ?>
                  <?php endforeach; ?>
                  <div class="clear"></div>
                </div>
              </div>
              <?php endif; ?>
            <?php endforeach; ?>
          </div>
          <?php endif; ?>
        <?php endforeach; ?>
      </div>
    </div>
  </div>
</div>

<div id="column-side-right">
<?php if (isset($onglets_struct[$meta_tab_name])) : ?>
  <div id="meta-header">
    <span><?php print $onglets_struct[$meta_tab_name]['label']; ?></span>
  </div>
  
  <div id="upper-buttons">
    <?php print $buttons;?>
  </div>

  <?php if(count($onglets_struct[$meta_tab_name]['elements']['no_group']['fields'])>0):?>
  <div class="group">
    <?php foreach($onglets_struct[$meta_tab_name]['elements']['no_group']['fields'] as $element_name => $element_content):?>
      <?php print drupal_render($form[$node_type][$element_name])?>
    <?php endforeach;?>
  </div>
  <?php endif;?>

  <?php foreach($onglets_struct[$meta_tab_name]['elements'] as $group_id => $group_content):?>
  <?php if($group_id != 'no_group'):?>
    <div class="group">
      <h2 class="title title-group "><span><?php print $onglets_struct[$meta_tab_name]['elements'][$group_id]["label"]?></span></h2>
      <div class="group_content">
        <?php foreach($onglets_struct[$meta_tab_name]['elements'][$group_id]['fields'] as $element_name => $element_content): ?>
          <?php print drupal_render($form[$node_type][$element_name])?>
        <?php endforeach;?>
        <div class="clear"></div>
      </div>
    </div>
  <?php endif;?>
  <?php endforeach;?>

<?php endif; ?>
</div>

<?php
  unset($form['current_tab']);
  unset($form[$node_type]);
  print drupal_render($form);
?>

<div id="buttons">
  <?php print $buttons;?>
</div>

<div id="profile_selector">
  <?php print $profile_selector; ?>
</div>

<div style="display:none;">
  <?php print $no_display ?>
</div>
