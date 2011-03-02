<?php
function wallysolrplus_preprocess_search_result(&$variables) {
  $result = $variables['result'];
  //$variables['url'] = check_url($result['link']);
  $fields = $result['fields'];
  $variables['url'] = check_url($fields['url']);
  $variables['title'] = check_plain($result['title']);
  $info = array();
  if (!empty($result['type'])) {
    $info['type'] = check_plain($result['type']);
  }
  if (!empty($result['user'])) {
    $info['user'] = $result['user'];
  }
  if (!empty($result['date'])) {
    $info['date'] = format_date($result['date'], 'small');
  }
  if (isset($result['extra']) && is_array($result['extra'])) {
    $info = array_merge($info, $result['extra']);
  }
  // Check for existence. User search does not include snippets.
  $variables['snippet'] = isset($result['snippet']) ? $result['snippet'] : '';
  // Provide separated and grouped meta information..
  $variables['info_split'] = $info;
  $variables['info'] = implode(' - ', $info);
  // Provide alternate search result template.
  $variables['template_files'][] = 'search-result-'. $variables['type'];
}