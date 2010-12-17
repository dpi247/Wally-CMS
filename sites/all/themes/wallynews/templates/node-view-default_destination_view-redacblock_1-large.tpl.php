<?php

  $title = $node->title;
  $node_path = drupal_get_path_alias("/node/".$node->nid);
?>
<h1><a href="<?php print $node_path; ?>"><?php print $title; ?></a></h1>
<h2>LARGE</h2>
