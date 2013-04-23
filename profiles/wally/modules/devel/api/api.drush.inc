<?php

/**
 * @file
 * Drush commands for the API module.
 */

/**
 * Implements hook_drush_command().
 */
function api_drush_command() {
  return array(
    'api-reparse' => array(
      'description' => 'Mark API files to reparse',
      'arguments' => array(
        'branch_or_file' => 'Optional branch ID number or file path relative to the common directory of its branch; omitted means mark everything',
      ),
    ),
    'api-list-branches' => array(
      'description' => 'List API branches',
    ),
    'api-reset-queue' => array(
      'description' => 'Reset the API parsing queue',
    ),
    'api-add-branch' => array(
      'description' => 'Add a branch to API',
      'arguments' => array(
        'project' => 'Project name',
        'project_title' => 'Project title',
        'branch' => 'Branch name',
        'branch_title' => 'Branch title',
        'directory' => 'Project directory',
      ),
    ),
  );
}

/**
 * Flags a branch, or all branches, to be reparsed on the next cron run.
 *
 * @param $branch_or_file
 *   (optional) Identifier of the branch to reparse, or name of a single file to
 *   reparse. If omitted all branches will be reparsed. File name must include
 *   the path relative to the common path to the directories indexed by this
 *   branch.
 */
function drush_api_reparse($branch_or_file = NULL) {
  $num = api_mark_for_reparse($branch_or_file);

  drush_log(dt('Marked @number files for reparsing.', array('@number' => $num)), 'ok');
}

/**
 * Resets the parsing queue.
 */
function drush_api_reset_queue() {
  api_reset_parse_queue();

  drush_log(dt('API parse queue reset. Necessary jobs will be added in the next cron run'), 'ok');
}

/**
 * Lists all API branches.
 */
function drush_api_list_branches() {
  $branches = api_get_branches();
  $rows = array(array(dt('ID'), dt('Project'), dt('Name'), dt('Type'), dt('Location')));
  foreach ($branches as $branch) {
    if (isset($branch->directories)) {
      $location = $branch->directories;
    }
    else {
      $location = isset($branch->summary) ? $branch->summary : '';
    }
    $rows[] = array(
      $branch->branch_id,
      $branch->project,
      $branch->branch_name,
      $branch->type,
      $location,
    );
  }
  drush_print_table($rows, TRUE);
}

/**
 * Adds or updates an API branch.
 */
function drush_api_add_branch($project, $project_title, $branch_name, $branch_title, $directory) {
  // Try to load the branch if possible.
  $branch = api_get_branch_by_name($project, $branch_name);
  if ($branch) {
    if (!empty($branch->data)) {
      $branch->data = unserialize($branch->data);
    }
    else {
      $branch->data = array();
    }
    // This is an update.
    $branch->project_title = $project_title;
    $branch->title = $branch_title;
    $branch->data['directories'] = $directory;
  }
  else {
    // This is a new branch.
    $branch = new stdClass();
    $branch->branch_id = 0;
    $branch->type = 'files';
    $branch->project = $project;
    $branch->project_title = $project_title;
    $branch->branch_name = $branch_name;
    $branch->title = $branch_title;
    $branch->data['directories'] = $directory;
    $branch->status = TRUE;
  }
  api_save_branch($branch);
  drush_log(dt('Saved branch.'), 'ok');
}
