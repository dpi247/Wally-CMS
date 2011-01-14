<?php
// $Id: views-view--fil-info--panel-pane-2.tpl.php,v 1.0 2011/01/12 ODM $
/**
 * @file views-view--fil-info--panel-pane-2.tpl.php
 * Default simple view template to display a list of rows.
 * 
 * Variables available:
 * - $view: The view object
 *
 * @ingroup views_templates
 */
$results = $view->result;
$pager = $view->pager;
$nodes = array();
$mainstories = array();
foreach ($results as $result) {
  $temp_node = node_load($result->nid);
  wallycontenttypes_packagepopulate($temp_node, '');
  $nodes[] = $temp_node;
}
?>
<h2>Le fil info</h2>

<div class="categories">
  <div>
    <img src="<?php print base_path().drupal_get_path('module', 'wallyfinfo').'/images/previoushoriz.png'; ?>" id="previnfos"/>
    <div id="catmenu" class="carousel"> 
      <ul>
        <li id="belgiqueinfos"><a href="#">Belgique</a></li>  
        <li id="mondeinfos"><a href="#">Monde</a></li> 
        <li id="sportinfos"><a href="#">Sport</a></li> 
        <li id="ecoinfos"><a href="#">Eco</a></li> 
        <li id="netinfos"><a href="#">Internet</a></li>
        <li id="sciencesinfos"><a href="#">Sciences et santé</a></li> 
        <li id="cultureinfos"><a href="#">Culture</a> </li>
        <li id="afficheinfos"><a href="#">A l'affiche</a> </li>
        <li id="electionsinfos"><a href="#">Elections 2010</a></li>
        <li id="diversinfos"><a href="#">Divers</a></li>
      </ul>
    </div>
    <img src="<?php print base_path().drupal_get_path('module', 'wallyfinfo').'/images/nexthoriz.png'; ?>" id="nextinfos"/>
  </div>
</div>

<div class="ttesinfos">
  <div id="infobel">
    <ul id="listebelgique" class="infoshideme">
      <?php foreach($nodes as $node) { ?>
      <li>
        <div class="belgique">
          <h2 class="title"><?php print check_plain($node->title); ?></h2>
          <p class="date"><?php print date('d M Y, H:m', $node->created); ?></p> 
          <?php print $node->field_mainstory_nodes[0]->field_textbody[0]['value']; ?>
        </div>
      </li>
      <?php } ?>
    </ul>
  </div>
</div>
