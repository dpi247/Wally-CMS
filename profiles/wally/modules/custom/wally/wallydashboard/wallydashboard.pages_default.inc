<?php

/**
 * Implementation of hook_default_page_manager_pages().
 */
function wallydashboard_default_page_manager_pages() {

  $pages = array();
  $pages[$page->name] = $page;
  
  return $pages;
  
}
