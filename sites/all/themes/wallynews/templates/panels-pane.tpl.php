<?php
// $Id: panels-pane.tpl.php,v 1.1.2.1 2009/10/13 21:38:52 merlinofchaos Exp $
/**
 * @file panels-pane.tpl.php
 * Main panel pane template
 *
 * Variables available:
 * - $pane->type: the content type inside this pane
 * - $pane->subtype: The subtype, if applicable. If a view it will be the
 *   view name; if a node it will be the nid, etc.
 * - $title: The title of the content
 * - $content: The actual content
 * - $links: Any links associated with the content
 * - $more: An optional 'more' link (destination only)
 * - $admin_links: Administrative links associated with the content
 * - $feeds: Any feed icons or associated with the content
 * - $display: The complete panels display object containing all kinds of
 *   data including the contexts and all of the other panes being displayed.
 */
 drupal_add_css(drupal_get_path('themes', 'wallynews').'/css/rotator.css');
 drupal_add_js(drupal_get_path('themes', 'wallynews').'/scripts/jquery.js');
 drupal_add_js(drupal_get_path('themes', 'wallynews').'/scripts/rotator.js');
?>
<?php
    
  if ($classes && $id) {
    print "<div class='".$classes."' ".$id.">";
    if($pane->type=="views")
      print  "<h2>$title</h2>";
    
    print $content;
    print "</div>";
  } else {
    print "<div class='box'>";
    if($pane->type=="views")
      print  "<h2>$title</h2>";
    
    print $content;
    print "</div>";
  }
?>
