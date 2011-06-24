<?php

function cckdestinations_batch_destinations() {
  $view = views_get_view('batch_destinations');
  $view->set_display('default');
  $view->override_path = $_GET['q'];
  $view->pager['use_pager'] = TRUE;
  $view->pager['items_per_page'] = 20;

  $return = $view->render();
  $return .= theme('pager', NULL, 20, 0);
  return $return;
}
