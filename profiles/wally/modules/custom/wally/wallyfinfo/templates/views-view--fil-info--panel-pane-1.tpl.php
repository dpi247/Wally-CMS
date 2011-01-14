<?php
// $Id: views-view--fil-info--panel-pane-1.tpl.php,v 1.0 2011/01/12 ODM $
/**
 * @file views-view--fil-info--panel-pane-1.tpl.php
 * Default simple view template to display a list of rows.
 * 
 * Variables available:
 * - $view: The view object
 *
 * @ingroup views_templates
 */
drupal_add_js(_wallyfinfo_getjsforfilinfotheme(), 'inline');
drupal_add_js(drupal_get_path('module', 'wallyfinfo').'/scripts/carousel.js');
drupal_add_css(drupal_get_path('module', 'wallyfinfo').'/css/packfilinfoscarousel.css');
$results = $view->result;
$pager = $view->pager;
?>

<div class="packfil">
  <div id="colinfos">
    <h2>Le fil info</h2>

    <div id="carbel" class="carousel">
      <ul id="listebelgique" class="hideme">
        <?php foreach ($results as $result) { ?>
        <li>
          <div class="belgique">
            <a href="<?php print drupal_get_path_alias("/node/".$result->nid); ?>"><small><?php print date('H:m', check_plain($result->node_created)); ?></small><h2><?php print check_plain($result->node_title); ?></h2></a>
          </div>
        </li>
        <?php } ?>
      </ul>
    </div>
    
    <div>
    <?php
    if($view->total_rows > $pager['items_per_page']) { 
      for($i=0; $i<5; $i++) {
        if($i==$pager['current_page']) {
    ?>
      <span><?php print $i; ?></span>
    <?php
        } else {
    ?>
      <span><a href="?page=<?php print $i; ?>"><?php print $i; ?></a></span>
    <?php
        }
      }
    }
    ?>
    </div>

    <div id="btn_infos">
      <img src="<?php print base_path().drupal_get_path('module', 'wallyfinfo').'/images/previous.png'; ?>" class="prev"/>
      <img src="<?php print base_path().drupal_get_path('module', 'wallyfinfo').'/images/next.png'; ?>" class="next"/>
    </div>
  </div>
</div>
