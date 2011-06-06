<?php
/*
 *
 * 
 */
$block = FALSE;

$main_poll = $node->field_mainpoll_nodes[0];

if (!empty($main_poll->allowvotes) && ($block || empty($main_poll->show_results))) {
  $main_poll->content['body'] = array(
    '#value' => drupal_get_form('poll_view_voting', $main_poll, $block, $form_state),
  );
} else {
  $main_poll->content['body'] = array(
    '#value' => poll_view_results($main_poll, $teaser, $page, $block),
  );
}

?>

<?php print $main_poll->content['body']['#value']; ?>
