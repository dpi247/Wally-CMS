<?php

/**
 * @file
 * Pages (except administrative) for the API module.
 */

/**
 * Page callback: Redirects to an appropriate search page.
 *
 * @param ...
 *   Search terms.
 *
 * @see api_menu()
 */
function api_search_redirect() {
  $args = func_get_args();
  if (count($args) === 0 && strpos($_GET['q'], 'apis/') !== 0) {
    // Handling 404.
    $tail = $_REQUEST['q'];
  }
  else {
    $tail = implode('/', $args);
  }
  $branch = api_get_branch_by_id();
  if (!isset($branch)) {
    $branch = api_get_active_branch();
  }
  if (isset($branch)) {
    $_REQUEST['destination'] = 'api/search/'. $branch->branch_name .'/'. $tail;
    drupal_goto();
  }
}

/**
 * Page callback: Prints out OpenSearch plugin XML output.
 *
 * @param $branch_name
 *   Name of the branch to search.
 *
 * @see https://developer.mozilla.org/en/Creating_OpenSearch_plugins_for_Firefox
 * @see api_menu()
 */
function api_opensearch($branch_name) {
  if (!db_result(db_query("SELECT 1 FROM {api_branch} WHERE branch_name = '%s'", $branch_name))) {
    return drupal_not_found();
  }

  drupal_set_header('Content-Type: text/xml; charset=utf-8');

  $short_name = t('Drupal API @branch', array('@branch' => $branch_name));
  $description = t('Drupal @branch API documentation', array('@branch' => $branch_name));
  // We need to use a theme function, so initialize the theme system.
  init_theme();
  if ($image = theme_get_setting('favicon')) {
    // Get rid of base_path that theme_get_setting() added.
    $image = substr($image, strlen(base_path()));
  }
  else {
    // Fall back on default favicon if the theme didn't provide one.
    $image = 'misc/favicon.ico';
  }
  $image = url($image, array('absolute' => TRUE));
  $search_url = url('api/search/'. $branch_name, array('absolute' => TRUE)) .'/{searchTerms}';
  $suggest_url = url('api/suggest/'. $branch_name, array('absolute' => TRUE)) .'/{searchTerms}';
  $search_form_url = url('api', array('absolute' => TRUE));
  $self_url = url($_GET['q'], array('absolute' => TRUE));

  print <<<EOD
<OpenSearchDescription xmlns="http://a9.com/-/spec/opensearch/1.1/"
                       xmlns:moz="http://www.mozilla.org/2006/browser/search/">
<ShortName>$short_name</ShortName>
<Description>$description</Description>
<InputEncoding>UTF-8</InputEncoding>
<Image width="16" height="16" type="image/x-icon">$image</Image>
<Url type="text/html" method="GET" template="$search_url" />
<Url type="application/x-suggestions+json" template="$suggest_url"/>
<Url type="application/opensearchdescription+xml" rel="self" template="$self_url" />
<moz:SearchForm>$search_form_url</moz:SearchForm>
</OpenSearchDescription>
EOD;
}

/**
 * Page callback: Prints JSON-formatted potential matches for OpenSearch.
 *
 * @param $branch
 *   Name of the branch to list.
 * @param ...
 *   The string to search for.
 *
 * @see http://www.opensearch.org/Specifications/OpenSearch/Extensions/Suggestions/1.0
 * @see api_menu()
 */
function api_suggest($branch) {
  $matches = array();
  $search = func_get_args();
  array_shift($search);
  $result = db_query_range("SELECT d.title FROM {api_documentation} d INNER JOIN {api_branch} b ON d.branch_id = b.branch_id WHERE d.title LIKE '%%%s%%' AND b.branch_name = '%s' ORDER BY LENGTH(d.title)", implode('/', str_replace('_', '\_', $search)), $branch, 0, 10);
  while ($r = db_fetch_object($result)) {
    $matches[] = $r->title;
  }
  print drupal_json(array($search, array_unique($matches)));
}

/**
 * Page callback: Prints a list of functions in text format, for use in IDEs.
 *
 * @param $branch_name
 *   Name of the branch to list functions for.
 *
 * @see api_menu()
 */
function api_page_function_dump($branch_name) {
  $result = db_query("SELECT d.title, d.summary, f.signature FROM {api_documentation} d INNER JOIN {api_function} f ON d.did = f.did INNER JOIN {api_branch} b ON d.branch_id = b.branch_id  WHERE b.branch_name = '%s' AND d.object_type = 'function'", $branch_name);
  while ($object = db_fetch_object($result)) {
    print($object->signature);
    print(' ### '. $object->summary ."\n");
  }
}

/**
 * Page callback: Performs a search.
 *
 * @param $branch_name
 *   Name of the branch to search.
 * @param ...
 *   The string to search for.
 *
 * @return
 *   HTML output for the search results. If there is exactly one result,
 *   redirects to that page instead of displaying the results page.
 *
 * @see api_menu()
 */
function api_search_listing($branch_name) {
  $search_text = func_get_args();
  array_shift($search_text);
  $search_text = implode('/', $search_text);
  $title = t('Search for %search', array('%search' => $search_text));
  drupal_set_title($title);
  $branches = api_get_branches_by_name($branch_name);
  if (isset($branches[0])) {
    $branch = $branches[0];
    $breadcrumb = array(
      l(t('Home'), '<front>'),
      l(t('API reference'), 'api/' . $branch->project),
      l($branch->title, 'api/' . $branch->project . '/' . $branch->branch_name),
    );
    drupal_set_breadcrumb($breadcrumb);
    $plain_title = t('Search for @search', array('@search' => $search_text));
    $page_title = array(
      check_plain($plain_title),
      check_plain($branch->title),
      check_plain(variable_get('site_name', 'Drupal')),
    );
    api_set_html_page_title(implode(' | ', $page_title));
  }

  $count = db_result(db_query("SELECT count(*) FROM {api_branch} b INNER JOIN {api_documentation} ad ON b.branch_id = ad.branch_id WHERE b.branch_name = '%s' AND ad.title = '%s'", $branch_name, $search_text));
  if ((int) $count === 1) {
    // Exact match.
    $item = db_fetch_object(db_query("SELECT b.branch_name, b.project, b.project_title, ad.* FROM {api_branch} b INNER JOIN {api_documentation} ad ON b.branch_id = ad.branch_id WHERE b.branch_name = '%s' AND ad.title = '%s'", $branch_name, $search_text));
    drupal_goto(api_url($item));
  }
  else {
    // Wildcard search.
    $result = pager_query("SELECT b.branch_id, b.branch_name, b.project, b.project_title, ad.* FROM {api_branch} b INNER JOIN {api_documentation} ad ON b.branch_id = ad.branch_id WHERE b.branch_name = '%s' AND ad.title LIKE '%%%s%%' ORDER BY ad.object_type, ad.title", 50, 0, NULL, $branch_name, str_replace(array('_', '%'), array('\_', '\%'), $search_text));
    return api_render_listing($result, t('No search results found.'), TRUE, TRUE) . theme('pager', NULL, 50, 0);
  }
}

/**
 * Page callback: Displays a list of links to projects using a pager.
 *
 * @return
 *   Themed HTML output for the page.
 */
function api_page_projects() {
  $num_per_page = 50;

  $result = pager_query("SELECT project_title, project FROM {api_branch} WHERE status = 1 GROUP BY project ORDER BY project_title", $num_per_page, 0, "SELECT COUNT(DISTINCT(project)) FROM {api_branch}");

  $links = array();
  while ($project = db_fetch_object($result)) {
    $links[$project->project] = array(
      'title' => $project->project_title,
      'href' => 'api/' . $project->project,
    );
  }

  return theme('links', $links, array('class' => 'api-project-links')) . theme('pager', NULL, $num_per_page);
}

/**
 * Page callback: Displays documentation for a file.
 *
 * @param $file
 *   Documentation object representing the file to display.
 *
 * @return
 *   HTML output for the file documentation page.
 *
 * @see api_menu()
 */
function api_page_file($file) {
  $branch = api_get_branch_by_id($file->branch_id);

  api_object_title_and_breadcrumb($branch, $file);

  $documentation = api_link_documentation($file->documentation, $branch);
  $see = api_link_documentation($file->see, $branch, NULL, TRUE);
  $related_topics = api_related_topics($file->did, $branch);
  $code = api_link_code($file->code, $branch);
  $defined = $file->file_name;

  $objects = api_render_listing(db_query("SELECT branch_id, title, object_name, summary, object_type, file_name, did FROM {api_documentation} WHERE file_name = '%s' AND branch_id = %d AND object_type IN ('constant', 'global', 'function', 'interface', 'class') AND class_did = 0 ORDER BY title", $file->object_name, $file->branch_id));

  // If this is a theme template, make reference link.
  $links = array();
  $theme_count = api_find_references($file->title, $branch, 'theme_invokes');
  if ($theme_count > 0) {
    $links[] = theme('api_function_reference_link', 'theme_invokes', $theme_count, $file);
  }

  $output = theme('api_file_page', $file, $documentation, $objects, $code, $see, $related_topics, $defined, $links);
  $output .= _api_add_comments($file);
  return $output;
}

/**
 * Page callback: Generates the default documentation page for a branch.
 *
 * @param $branch
 *   Branch object giving the branch to display documentation for.
 *
 * @return
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_branch($branch) {
  $result = db_query("SELECT documentation FROM {api_documentation} WHERE branch_id = %d AND object_type = 'mainpage'", $branch->branch_id);
  if ($docs = db_fetch_object($result)) {
    return api_link_documentation($docs->documentation, $branch);
  }
  else {
    return theme('api_branch_default_page', $branch);
  }
}

/**
 * Page callback: Generates a listing page for an object type.
 *
 * @param $branch
 *   Object representing branch to generate the listing for.
 * @param $object_type
 *   Type of object to list ('file', 'group', etc.).
 * @param $switch_link
 *   TRUE to include link to switch projects; FALSE to not include it.
 *
 * @return
 *   HTML for the listing page.
 */
function api_page_listing($branch, $object_type, $switch_link = TRUE) {

  // Set the HTML page title and breadcrumb.
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('API reference'), 'api/' . $branch->project),
    l($branch->title, 'api/' . $branch->project . '/' . $branch->branch_name),
  );
  drupal_set_breadcrumb($breadcrumb);
  $title = drupal_get_title();
  $page_title = array(
    check_plain($title),
    check_plain($branch->title),
    check_plain(variable_get('site_name', 'Drupal')),
  );
  api_set_html_page_title(implode(' | ', $page_title));

  // Use the correct plural form to construct the URLs for the other branches.
  // @todo Create a unit test for this to ensure there's no regression.
  $plural_type = $object_type . 's';
  $show_headings = FALSE;

  if ($object_type === 'group') {
    $result = pager_query("SELECT * FROM {api_documentation} WHERE branch_id = %d AND object_type = '%s' ORDER BY title", 50, 0, NULL, $branch->branch_id, $object_type);
  }
  elseif ($object_type === 'class') {
    $plural_type = 'classes';
    $show_headings = TRUE;
    $result = pager_query("SELECT * FROM {api_documentation} WHERE branch_id = %d AND (object_type = 'class' OR object_type = 'interface') AND class_did = 0 ORDER BY title", 50, 0, NULL, $branch->branch_id);
  }
  elseif ($object_type === 'file') {
    // Order by path + file name, not base name of file.
    $result = pager_query("SELECT * FROM {api_documentation} WHERE branch_id = %d AND object_type = '%s' AND class_did = 0 ORDER BY object_name", 50, 0, NULL, $branch->branch_id, $object_type);
  }
  else {
    $result = pager_query("SELECT * FROM {api_documentation} WHERE branch_id = %d AND object_type = '%s' AND class_did = 0 ORDER BY title", 50, 0, NULL, $branch->branch_id, $object_type);
  }

  $suffix = '';
  if ($switch_link) {
    $suffix = api_other_projects_link();
  }
  return api_render_listing($result, NULL, $show_headings, $object_type !== 'file' && $object_type !== 'group') . theme('pager', NULL, 50, 0) . $suffix;
}

/**
 * Generates a link to the projects page if there is more than one project.
 *
 * @return
 *   Link to the API projects page, inside a P tag, or an empty string if there
 *   would only be one project to display on that page.
 */
function api_other_projects_link() {
  // Figure out how many different projects there are.
  $count = db_result(db_query('SELECT COUNT(DISTINCT(project)) FROM {api_branch} WHERE status=1'));

  if ($count > 1) {
    return '<p class="api-switch"><strong>' . l(t('Other projects'), 'api/projects') . '</strong></p>';
  }

  return '';
}

/**
 * Page callback: Displays documentation for a function.
 *
 * @param $function
 *   Documentation object representing the function to display.
 *
 * @return
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_function($function) {
  $branches = api_get_branches();
  $branch = $branches[$function->branch_id];

  api_object_title_and_breadcrumb($branch, $function);

  // Make the signatures section, which lists all the other function signatures
  // (branches and duplicate function names) that we can find.
  $last_signature = '';
  $signatures = array();
  $n = 0;
  $result = db_query("SELECT d.branch_id, d.file_name, d.object_type, d.object_name, f.signature, d.class_did FROM {api_documentation} d INNER JOIN {api_function} f ON f.did = d.did INNER JOIN {api_branch} b ON d.branch_id = b.branch_id WHERE d.object_type = 'function' AND d.title = '%s' AND b.status = 1 ORDER BY b.weight", $function->title);
  while ($signature = db_fetch_object($result)) {
    $signature_branch = $branches[$signature->branch_id];
    $branch_label = $signature_branch->branch_name . ' ' . basename($signature->file_name);
    $active = ($signature->branch_id === $function->branch_id && $signature->file_name == $function->file_name);

    // Build up the function prefix/class/name. Note that token_get_all() does
    // not work unless the code starts with the open PHP token.
    $tokens = token_get_all('<' . '?php '. $signature->signature);

    // Remove open PHP token.
    array_shift($tokens);

    // Get the prefix -- everything up to the function name, such as "public",
    // etc.
    $name = '';
    while ($tokens[1] !== '(') {
      if (is_array($tokens[0])) {
        $name .= $tokens[0][1];
      }
      else {
        $name .= $tokens[0];
      }
      array_shift($tokens);
    }

    // Add the class name, if there is one. This is not part of the signature.
    if ($signature->class_did !== '0') {
      $class = api_object_load((int) $signature->class_did, $signature_branch, array('interface', 'class'), $signature->file_name);
      if (isset($class)) {
        $name .= l($class->object_name, api_url($class)) . '::';
      }
    }

    // Add the actual function name, plain (active branch) or link (not
    // active branch).
    if ($active) {
      $name .= $tokens[0][1];
    }
    else {
      $name .= l($tokens[0][1], api_url($signature));
    }

    // Store this much for the second pass.
    $signature_info[$n] = array(
      'branch_label' => $branch_label,
      'object' => $signature,
      'active' => $active,
      'arguments' => array(),
      'other' => array(),
      'name' => $name,
    );

    // Collect the argument list, separating it out into the argument names
    // and other info (such as default values).
    $start = TRUE;
    $depth = 0;
    $arg_index = -1;
    $signature_info[$n]['other'][$arg_index] = '';
    foreach ($tokens as $token) {
      $depth += in_array($token, array('(', '{', '[')) - in_array($token, array(')', '}', ']'));
      if ($depth == 1 && $start && is_array($token) && $token[0] == T_VARIABLE) {
        // New argument
        $arg_index += 1;
        $signature_info[$n]['arguments'][$arg_index] = $token[1];
        $signature_info[$n]['other'][$arg_index] = '';
        $start = FALSE;
      }
      elseif ($depth >= 1 && is_array($token)) {
        // Non-arg other information.
        $signature_info[$n]['other'][$arg_index] .= $token[1];
      }
      elseif ($depth >= 1) {
        // Non-arg other information.
        $signature_info[$n]['other'][$arg_index] .= $token;
        // Start looking for a new argument if we see a comma.
        $start = $start || ($depth == 1 && $token == ',');
      }
    }
    $last_signature = $signature->signature;
    $n++;
  }

  // Format the argument lists, keeping track of when arguments come and go
  // through the branches.
  foreach ($signature_info as $n => $info) {
    $new = array();
    if (isset($signature_info[$n - 1])) {
      $new = array_diff($info['arguments'], $signature_info[$n - 1]['arguments']);
    }
    $old = array();
    if (isset($signature_info[$n + 1])) {
      $old = array_diff($info['arguments'], $signature_info[$n + 1]['arguments']);
    }

    $signature = $info['name'] . $info['other'][-1];
    foreach ($signature_info[$n]['arguments'] as $key => $argument) {
      if (in_array($argument, $old)) {
        $signature .= '<del>'. $argument .'</del>';
      }
      elseif (in_array($argument, $new)) {
        $signature .= '<ins>'. $argument .'</ins>';
      }
      else {
        $signature .= $argument;
      }
      $signature .= $info['other'][$key];
    }
    $signature .= ')';
    $signatures[$info['branch_label']] = array(
      'signature' => $signature,
      'active' => $info['active'],
      'url' => api_url($info['object']),
    );
  }

  // Build the page sections.
  $documentation = api_link_documentation($function->documentation, $branch, $function->class_did);
  $parameters = api_link_documentation($function->parameters, $branch, $function->class_did, TRUE);
  $return = api_link_documentation($function->return_value, $branch, $function->class_did, TRUE);
  $see = api_link_documentation($function->see, $branch, $function->class_did, TRUE);
  $throws = api_link_documentation($function->throws, $branch, $function->class_did, TRUE);
  $code = api_link_code($function->code, $branch, $function->class_did);
  $related_topics = api_related_topics($function->did, $branch);

  // Build reference links.
  $links = array();
  foreach (array('calls', 'references', 'implementations', 'invokes', 'theme_invokes') as $type) {
    $call_count = api_find_references($function->object_name, $branch, $type, TRUE, $function->did);
    if ($call_count > 0) {
      $links[] = theme('api_function_reference_link', $type, $call_count, $function);
    }
  }

  // Put it all together and theme the output.
  $output = theme('api_function_page', $branch, $function, $signatures, $documentation, $parameters, $return, $related_topics, $links, $code, $see, $throws);
  $output .= _api_add_comments($function);
  return $output;
}

/**
 * Counts or lists references to a function or file.
 *
 * @param $name
 *   Name of the function/file to check (see $type).
 * @param $branch
 *   Object representing the branch to check.
 * @param $type
 *   Type of reference. One of:
 *   - 'calls': $name is the name of a function, find function calls.
 *   - 'implementations': $name is the name of a hook, find implementations.
 *   - 'invokes': $name is the name of a hook, find invocations.
 *   - 'theme_invokes': $name is the name of a theme function or file, find
 *     theme calls.
 *   - 'references': $name is the name of a function, find string references.
 * @param $count
 *   If TRUE (default), return a count. If FALSE, return an array.
 * @param $exclude_did
 *   (optional) Document ID to exclude from the query.
 *
 * @return
 *   The number of references or an array of references.
 */
function api_find_references($name, $branch, $type, $count = TRUE, $exclude_did = 0) {
  $branch_id = $branch->branch_id;
  $base_query = '';
  $args = array();

  if ($type == 'calls') {
    // Use reference storage to find functions that call this one.
    $base_query = "FROM {api_reference_storage} r LEFT JOIN {api_documentation} d ON r.from_did = d.did WHERE d.branch_id = %d AND d.did <> %d AND (r.object_type = 'function' OR r.object_type = 'member-class' OR r.object_type = 'computed-member') AND r.object_name = '%s'";
    $args = array($branch_id, $exclude_did, $name);
  }
  elseif ($type == 'references') {
    // Use reference storage to find functions that use this name as a string.
    $base_query = "FROM {api_reference_storage} r INNER JOIN {api_documentation} d ON r.from_did = d.did WHERE r.object_type = 'potential callback' AND r.object_name = '%s' AND d.branch_id = %d AND d.did <> %d";
    $args = array($name, $branch_id, $exclude_did);
  }
  elseif ($type == 'implementations') {
    // Use pattern matching to find functions that look like implementations of
    // this one. i.e. something_hookname, where "something" doesn't start with
    // an underscore. Note that LIKE uses _ as "match any one character", so
    // _ has to be escaped in this query.
    if (strpos($name, 'hook_') === 0) {
      $hook_name = substr($name, 5);
      $base_query = "FROM {api_documentation} d WHERE d.object_name LIKE '%%\_%s' AND d.object_name NOT LIKE '\_%%' AND d.branch_id = %d AND d.did <> %d AND d.object_type = 'function'";
      $args = array($hook_name, $branch_id, $exclude_did);
    }
  }
  elseif ($type == 'theme_invokes') {
    // Use reference storage to find functions that call this theme.
    $theme_name = '';
    if (strpos($name, 'theme_') === 0) {
      // It's presumably a theme function.
      $theme_name = substr($name, 6);
    }
    elseif (strpos($name, '.tpl.php') == strlen($name) - 8) {
      // It's presumably a theme template file.
      $name = basename($name);
      $theme_name = str_replace('-', '_', substr($name, 0, strlen($name) - 8));
    }

    if (strlen($theme_name) > 0) {
      // We could have calls to things like theme('name') or theme('name__sub')
      // recorded in the references. Match either. Note _ escaping for LIKE
      // queries.
      $base_query = "FROM {api_reference_storage} r INNER JOIN {api_documentation} d ON r.from_did = d.did WHERE (r.object_name = '%s' OR r.object_name LIKE '%s\_\_%%') AND r.object_type = 'potential theme' AND d.branch_id = %d AND d.did <> %d";
      $args = array($theme_name, $theme_name, $branch_id, $exclude_did);
    }
  }
  elseif ($type == 'invokes') {
    // Use reference storage to find functions that invoke this hook.
    // The reference storage holds the string that was actually found inside
    // the invoking function. So $name could be "hook_$string",
    // "hook_field_$string", "field_default_$string", "hook_user_$string", or
    // "hook_$string_alter". In all cases but the last, {reference_storage} has
    // object_type 'potential hook'; in the last case it is 'potential alter'.
    // So, we need to build a query that will find these matches between
    // $string (field object_name) and $name (passed into this function).
    $or_clauses = array();
    $args = array($branch_id, $exclude_did);

    if (strpos($name, 'hook_') === 0) {
      $hook_name = substr($name, 5);
      $or_clauses[] = "( r.object_type = 'potential hook' AND r.object_name = '%s' )";
      $args[] = $hook_name;
      if (strpos($hook_name, 'user_') === 0) {
        $sub_hook_name = substr($hook_name, 5);
        $or_clauses[] = "( r.object_type = 'potential hook' AND r.object_name = '%s' )";
        $args[] = $sub_hook_name;
      }
      elseif (strpos($hook_name, 'field_') === 0) {
        $sub_hook_name = substr($hook_name, 6);
        $or_clauses[] = "( r.object_type = 'potential hook' AND r.object_name = '%s' )";
        $args[] = $sub_hook_name;
      }
      elseif (strrpos($hook_name, '_alter') === strlen($hook_name) - 6) {
        $sub_hook_name = substr($hook_name, 0, strlen($hook_name) - 6);
        $or_clauses[] = "( r.object_type = 'potential alter' AND r.object_name = '%s' )";
        $args[] = $sub_hook_name;
      }
    }
    elseif (strpos($name, 'field_default_') === 0) {
      $hook_name = substr($name, 14);
      $or_clauses[] = "( r.object_type = 'potential hook' AND r.object_name = '%s' )";
      $args[] = $hook_name;
    }

    // If we found at least one match, run this query we've built.
    if (count($or_clauses)) {
      $base_query = " FROM {api_reference_storage} r INNER JOIN {api_documentation} d ON r.from_did = d.did WHERE d.branch_id = %d AND d.did <> %d AND (" . implode(' OR ', $or_clauses) . ")";
    }
  }

  // See if we built a query to execute.
  if (!strlen($base_query)) {
    if ($count) {
      return 0;
    }
    else {
      return array();
    }
  }

  // Execute the query.
  if ($count) {
    // If we want a count, just return it.
    return db_result(db_query("SELECT COUNT(*) $base_query", $args));
  }

  // If we get here, we want to return a list.
  $result = db_query("SELECT d.branch_id, d.object_name, d.title, d.summary, d.file_name, d.object_type $base_query ORDER BY d.title", $args);
  $list = array();
  while ($object = db_fetch_object($result)) {
    $list[] = array(
      'function' => l($object->title, api_url($object)),
      'file' => api_file_link($object),
      'description' => api_link_documentation($object->summary, $branch),
    );
  }

  return $list;
}

/**
 * Page callback: Displays a list of instances where a function is called.
 *
 * @param $function
 *   Documentation object representing the function to display.
 * @param $type
 *   Type of page to generate, one of:
 *   - 'calls': Functions that call this function.
 *   - 'implementations': Functions that implement this hook.
 *   - 'theme_invokes': Functions that invoke this theme hook.
 *   - 'invokes': Functions that invoke this hook.
 *
 * @return
 *   HTML output for the page.
 */
function api_page_function_calls($function, $type) {
  $branch = api_get_branch_by_id($function->branch_id);

  $call_count = api_find_references($function->object_name, $branch, $type, TRUE, $function->did);
  $call_title = api_reference_text($type, $call_count, $function);
  api_object_title_and_breadcrumb($branch, $function, $call_title, TRUE);

  $call_functions = api_find_references($function->object_name, $branch, $type, FALSE, $function->did);

  $note = '';
  if ($type == 'implementations') {
    $note = '<p>' . t('Note: this list is generated by pattern matching, so it may include some functions that are not actually implementations of this hook.') . '</p>';
  }

  return $note . theme('api_functions', $call_functions);
}

/**
 * Page callback: Displays documentation for an item (constant, global, etc.).
 *
 * This function can be used to display "simple" items (items without listings,
 * and functions are a special case as well, because they display signatures
 * and variations).
 *
 * @param $item
 *   Documentation object representing the item to display.
 * @param $type
 *   Type of item, one of: 'constant', 'global', 'property'.
 *
 * @return
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_simple_item($item, $type) {
  $branch = api_get_branch_by_id($item->branch_id);

  api_object_title_and_breadcrumb($branch, $item);

  $documentation = api_link_documentation($item->documentation, $branch, $item->class_did);
  $code = api_link_code($item->code, $branch, $item->class_did);
  $related_topics = api_related_topics($item->did, $branch);
  $see = api_link_documentation($item->see, $branch, $item->class_did, TRUE);
  $var = '';
  if ($type == 'property') {
    $var = api_link_name($item->var, $branch, '', '', $item->class_did);
  }

  $theme_hooks = array(
    'constant' => 'api_constant_page',
    'global' => 'api_global_page',
    'property' => 'api_property_page',
  );

  $output = theme($theme_hooks[$type], $branch, $item, $documentation, $code, $related_topics, $see, $var);
  $output .= _api_add_comments($item);
  return $output;
}

/**
 * Page callback: Displays documentation for a class.
 *
 * @param $class
 *   Documentation object representing the class to display.
 *
 * @return
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_class($class) {
  $branch = api_get_branch_by_id($class->branch_id);

  api_object_title_and_breadcrumb($branch, $class);

  $documentation = api_link_documentation($class->documentation, $branch, $class->did);
  $related_topics = api_related_topics($class->did, $branch);
  $code = api_link_code($class->code, $branch, $class->did);
  $see = api_link_documentation($class->see, $branch, $class->did, TRUE);

  // Figure out the class hierarchy, and what classes implement if this is
  // an interface.
  $hierarchy = api_class_hierarchy($class);
  $implements = array();
  if ($class->object_type === 'interface') {
    $result = db_query("SELECT ad.did, ad.branch_id, ad.file_name, ad.object_type, ad.object_name FROM {api_reference_storage} ars INNER JOIN {api_documentation} ad ON ad.did = ars.from_did WHERE ars.to_did = %d AND ars.object_type = 'interface' GROUP BY ad.did ORDER BY ad.object_name", $class->did);
    while ($object = db_fetch_object($result)) {
      $implements[] = l($object->object_name, api_url($object));
    }
    $implements = theme('item_list', $implements);
  }

  $objects = api_render_listing(db_query("SELECT ad.branch_id, ad.title, ad.object_name, ad.summary, ad.object_type, ad.file_name, ao.overrides_did, ad.did FROM {api_members} am INNER JOIN {api_documentation} ad ON ad.did = am.did LEFT JOIN {api_overrides} ao ON ao.did = am.did WHERE am.class_did = %d ORDER BY title", $class->did));
  $output = theme('api_class_page', $branch, $class, $documentation, $implements, $hierarchy, $objects, $code, $related_topics, $see);
  $output .= _api_add_comments($class);

  return $output;
}

/**
 * Page callback: Displays documentation for a group (topic).
 *
 * @param $group
 *   Documentation object representing the group (topic) to display.
 *
 * @return
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_group($group) {
  $branch = api_get_branch_by_id($group->branch_id);

  api_object_title_and_breadcrumb($branch, $group);

  $documentation = api_link_documentation($group->documentation, $branch);
  $see = api_link_documentation($group->see, $branch, NULL, TRUE);

  $objects = api_render_listing(db_query("SELECT d.branch_id, d.object_name, d.title, d.summary, d.file_name, d.object_type, d.did FROM {api_reference_storage} r INNER JOIN {api_documentation} d ON r.from_did = d.did WHERE r.object_type = 'group' AND r.to_did = %d ORDER BY d.object_name", $group->did));

  $output = theme('api_group_page', $branch, $group, $documentation, $objects, $see);
  $output .= _api_add_comments($group);
  return $output;
}

/**
 * Preprocesses theme('api_defined').
 *
 * Sets up variables for the file link and summary line.
 */
function api_preprocess_api_defined(&$variables) {
  $object = $variables['object'];
  $branch = $variables['branch'];
  $variables['file_link'] = api_file_link($object);
  $file_info = api_filename_load($object->file_name, $branch->project, $branch->branch_name);
  if (isset($file_info)) {
    $variables['file_summary'] = $file_info->summary;
  }
}

/**
 * Preprocesses a documentation item page.
 */
function api_preprocess_api_object_page(&$variables) {
  // Note: branch is not defined for File pages, so don't try to create the
  // 'defined' section of the page.
  if (isset($variables['branch'])) {
    $variables['defined'] = theme('api_defined', $variables['branch'], $variables['object']);
  }

  // Find alternative versions in different files and branches.
  $object = $variables['object'];
  $branches = api_get_branches();
  $alternatives = array(
    '#prefix' => '<ol id="api-alternatives">',
    '#suffix' => '</ol>',
  );
  // Try to find objects of the same type with the same object name. But for
  // files, the object name includes the path, so the matching needs to go on
  // the title instead of the object_name.
  if ($object->object_type == 'file') {
    $result = db_query("SELECT ad.branch_id, ad.file_name, ad.object_type, ad.object_name FROM {api_documentation} ad WHERE ad.object_type = '%s' AND ad.title = '%s'", $variables['object']->object_type, $variables['object']->title);
  }
  else {
    $result = db_query("SELECT ad.branch_id, ad.file_name, ad.object_type, ad.object_name FROM {api_documentation} ad WHERE ad.object_type = '%s' AND ad.object_name = '%s'", $variables['object']->object_type, $variables['object']->object_name);
  }

  while ($alternative = db_fetch_object($result)) {
    // Group by project.
    if (!isset($alternatives[$branches[$alternative->branch_id]->project])) {
      $alternatives[$branches[$alternative->branch_id]->project] = array(
        '#prefix' => '<li><strong>' . $branches[$alternative->branch_id]->project_title . '</strong><ol>',
        '#suffix' => '</ol></li>',
      );
    }

    // Construct link label.
    $label = $branches[$alternative->branch_id]->branch_name . ' ' . $alternative->file_name;

    $alternatives[$branches[$alternative->branch_id]->project][] = array(
      '#prefix' => '<li>',
      '#value' => l($label, api_url($alternative)),
      '#weight' => $branches[$alternative->branch_id]->weight,
      '#suffix' => '</li>',
    );
  }
  $variables['alternatives'] = drupal_render($alternatives);

  // See if this is an override of another method/etc.
  $variables['override'] = '';
  if (!empty($object->overrides_did)) {
    $overrides = api_object_load((int) $object->overrides_did, $branches[$object->branch_id], array('function', 'property', 'constant'));
    if (isset($overrides)) {
      $variables['override'] = '<p class="api-override">' . t('Overrides !link', array('!link' => l($overrides->title, api_url($overrides)))) . '</p>';
    }
  }
}

/**
 * Renders an overview of documentation objects in a table.
 *
 * @param $result
 *   A database query result object.
 * @param $empty_message
 *   An optional string to display instead of an empty table.
 * @param $show_headings
 *   If you expect only one object type, you might not want the provided
 *   heading.
 * @param $link_file
 *   Boolean : toggles the display of the file link column.
 *
 * @return
 *   Rendered HTML for the listing.
 */
function api_render_listing($result, $empty_message = NULL, $show_headings = TRUE, $link_file = FALSE) {
  // Group query result by object type.
  $list = array();
  while ($object = db_fetch_object($result)) {
    $list[$object->object_type][$object->did] = $object;
  }

  if (count($list) === 0) {
    return is_null($empty_message) ? '' : '<p><em>'. $empty_message .'</em></p>';
  }

  $branches = api_get_branches();
  $header = array(t('Name'));
  if ($link_file) {
    $header[] = t('Location');
  }
  $header[] = t('Description');

  $tables = array();
  foreach ($list as $type => $objects) {
    $rows = array();
    foreach ($objects as $object) {
      $title = $object->title;
      if ($type == 'file') {
        $title = $object->object_name;
      }
      $row = array(l($title, api_url($object)));
      if ($link_file) {
        $row[] = '<small>' . api_file_link($object) . '</small>';
      }
      $summary = api_link_documentation($object->summary, $branches[$object->branch_id]);
      if (!empty($object->overrides_did)) {
        $overrides = api_object_load((int) $object->overrides_did, $branches[$object->branch_id], array('function', 'property', 'constant'));
        if (isset($overrides)) {
          $summary .= ' <em class="api-inheritance-source">' . t('Overrides !link', array('!link' => l($overrides->title, api_url($overrides)))) . '</em>';
        }
      }
      $row[] = $summary;
      $rows[] = $row;
    }
    $tables[$type] = theme('table', $header, $rows);
  }

  $headings = array(
    'function' => t('Functions & methods'),
    'property' => t('Properties'),
    'group' => t('Groups'),
    'global' => t('Globals'),
    'constant' => t('Constants'),
    'file' => t('Files'),
    'interface' => t('Interfaces'),
    'class' => t('Classes'),
  );
  $output = '';
  foreach ($tables as $key => $table) {
    if ($show_headings) {
      $output .= '<h3>' . $headings[$key] . '</h3>';
    }
    $output .= $table;
  }
  return $output;
}

/**
 * Sets the page title and breadcrumb for an object display page.
 *
 * @param $branch
 *   Object representing the branch.
 * @param $object
 *   Object representing the documentation item on the current page.
 * @param $title
 *   A string to be used as a replacement for the title (optional).
 * @param $object_name_in_breadcrumb
 *   A boolean indicating whether to add the object name to the breadcrumb.
 */
function api_object_title_and_breadcrumb($branch, $object, $title = NULL, $object_name_in_breadcrumb = FALSE) {
  // Allow the title to be overridden.
  if (empty($title)) {
    $title = check_plain($object->title);
  }
  drupal_set_title($title);

  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('API reference'), 'api/' . $branch->project),
    l($branch->title, 'api/' . $branch->project . '/' . $branch->branch_name),
  );
  $page_title = array(
    check_plain(variable_get('site_name', 'Drupal')),
    check_plain($branch->title),
  );

  if ($object->object_type !== 'file') {
    $breadcrumb[] = l(basename($object->file_name), api_url($object, TRUE));
    $page_title[] = check_plain(basename($object->file_name));
  }
  if (!empty($object->class_did)) {
    $branch = api_get_branch_by_id($object->branch_id);
    $class = api_object_load((int) $object->class_did, $branch, array('interface', 'class'), $object->file_name);
    if (isset($class)) {
      $breadcrumb[] = l($class->object_name, api_url($class));
    }
    // Note that this is not needed in the page title, since the object name
    // includes the class.
  }

  // Add the object name to the breadcrumb. This is used for function calls and
  // hook implementations listing pages.
  if ($object_name_in_breadcrumb) {
    $breadcrumb[] = l($object->object_name, api_url($object));
  }

  $page_title[] = $title;

  drupal_set_breadcrumb($breadcrumb);
  api_set_html_page_title(implode(' | ', array_reverse($page_title)));
}

/**
 * Renders a class hierarchy, showing ancestors, siblings, and direct children.
 *
 * @param $class
 *   Class object with at least did, branch_id, file_name, object_type, and
 *   object_name properties.
 *
 * @return
 *   HTML string containing the class hierarchy, or an empty string if the
 *   only thing to display would be the class itself.
 */
function api_class_hierarchy($class) {
  // Keep track of whether this class has anything interesting in the
  // hierarchy.
  $interesting = FALSE;

  // See if this class has any children.
  $child_output = '';
  $children = api_class_children($class);
  if (count($children)) {
    $child_output = theme('item_list', $children);
    $interesting = TRUE;
  }

  // Find the direct-line extends ancestors of this class.
  $parent = $class;
  $first_parent = NULL;
  $parents = array();
  while ($parent = db_fetch_object(db_query_range("SELECT ad.did, ad.branch_id, ad.file_name, ad.object_type, ad.object_name as object_name FROM {api_reference_storage} ars INNER JOIN {api_documentation} ad ON ad.did = ars.to_did WHERE ars.from_did = %d AND ars.object_type = 'class'", $parent->did, 0, 1))) {
    $interesting = TRUE;
    if (!isset($first_parent)) {
      $first_parent = $parent;
    }
    $parents[] = api_class_hierarchy_line($parent);
  }

  // If there was a parent class, find all of its children. These are this
  // class's siblings.
  if (isset($first_parent)) {
    $siblings = api_class_children($first_parent);
  }
  else {
    $siblings = array($class->did => api_class_hierarchy_line($class, $interesting));
    // Check and see if there was anything interesting to display.
    if (!strlen($siblings[$class->did])) {
      return '';
    }
  }

  // Add this class's children to this class's line in the siblings.
  $siblings[$class->did] .= $child_output;

  // Put it all together as a set of nested lists.
  $output = theme('item_list', $siblings);
  foreach ($parents as $parent) {
    $output = theme('item_list', array( $parent . $output));
  }

  return $output;
}

/**
 * Finds and renders the children of a class.
 *
 * @param $class
 *   Documentation object for the class.
 *
 * @return
 *   Array of children. Keys are class names, and values are output
 *   of api_class_hierarchy_line().
 */
function api_class_children($class) {
  $children = array();
  $result = db_query("SELECT ad.did, ad.branch_id, ad.file_name, ad.object_type, ad.object_name FROM {api_reference_storage} ars INNER JOIN {api_documentation} ad ON ad.did = ars.from_did WHERE ars.to_did = %d AND ars.object_type = 'class' GROUP BY ars.from_did ORDER BY ad.object_name", $class->did);
  while ($object = db_fetch_object($result)) {
    $children[$object->did] = api_class_hierarchy_line($object);
  }

  return $children;
}

/**
 * Renders the class hierarchy line for a single class.
 *
 * @param $class
 *   Documentation object for the class to render.
 * @param $even_if_uninteresting
 *   If TRUE (default), generate a line even if it only displays this class.
 *   If FALSE, return an empty string if there is only this class to display.
 *
 * @return
 *   HTML for this class in the class hierarchy.
 */
function api_class_hierarchy_line($class, $even_if_uninteresting = TRUE) {
  // See if this class implements any interfaces.
  $interfaces = array();
  $result = db_query("SELECT ad.did, ad.branch_id, ad.file_name, ad.object_type, ad.object_name as object_name, ars.object_name as ref_name FROM {api_reference_storage} ars LEFT JOIN {api_documentation} ad ON ad.did = ars.to_did WHERE ars.from_did = %d AND ars.object_type = 'interface' GROUP BY ars.to_did ORDER BY ad.object_name", $class->did);
  while ($object = db_fetch_object($result)) {
    if (isset($object->did) && $object->did) {
      // Interface exists as an object in the database, make a link.
      $interfaces[] = l($object->object_name, api_url($object));
    }
    else {
      // This class was declared to implement this interface, but it didn't get
      // defined (probably a built-in PHP interface or from another project).
      // So just display the name.
      $interfaces[] = $object->ref_name;
    }
  }

  // See if this class extends another class that isn't defined in our DB.
  $extends = '';
  $parent = db_fetch_object(db_query_range("SELECT ad.did, ars.object_name FROM {api_reference_storage} ars LEFT JOIN {api_documentation} ad ON ad.did = ars.to_did WHERE ars.from_did = %d AND ars.object_type = 'class'", $class->did, 0, 1));
  if (isset($parent) && is_object($parent) && is_null($parent->did)) {
    $extends = ' extends ' . $parent->object_name;
  }

  if (!$even_if_uninteresting && !strlen($extends) && !count($interfaces)) {
    return '';
  }

  $output = l($class->object_name, api_url($class)) . $extends;
  if (count($interfaces) > 0) {
    $output .= ' implements ' . implode(', ', $interfaces);
  }

  return $output;
}

/**
 * Renders comments for a documentation object.
 *
 * @param $documentation_object
 *   Object to render comments for.
 *
 * @return
 *   Rendered comments to display with the object.
 */
function _api_add_comments($documentation_object) {
  $output = '';

  if (module_exists('comment') && user_access('access comments') && variable_get('comment_api', COMMENT_NODE_READ_WRITE) != COMMENT_NODE_DISABLED) {
    $output .= comment_render(node_load($documentation_object->did));
    if (user_access('post comments')) {
      $output .= comment_form_box(array('nid' => $documentation_object->did), t('Post new comment'));
    }
    elseif (array_key_exists(DRUPAL_AUTHENTICATED_RID, user_roles(TRUE, 'post comments'))) {
      // If authenticated users can post comments.
      $options = array(
        'query' => drupal_get_destination() . urlencode('#comment-form'),
      );
      if (variable_get('user_register', 1)) {
        // Users can register themselves.
        $output .= t('<a href="@login">Login</a> or <a href="@register">register</a> to post comments', array('@login' => url('user/login', $options), '@register' => url('user/register', $options)));
      }
      else {
        // Only admins can add new users, no public registration.
        $output .= t('<a href="@login">Login</a> to post comments', array('@login' => url('user/login', $options)));
      }
    }
  }

  return $output;
}

/**
 * Lists the topics (groups) that contain the documentation object.
 *
 * @param $did
 *   ID of the documentation object to find topics for.
 * @param $branch
 *   Branch object to find topics in.
 *
 * @return
 *   List of related topics, rendered as HTML.
 */
function api_related_topics($did, $branch) {
  $header = array(
    t('Name'),
    t('Description'),
  );
  $topics = array();
  $result = db_query("SELECT d.branch_id, d.object_name, d.file_name, d.object_type, d.title, d.summary FROM {api_reference_storage} r INNER JOIN {api_documentation} d ON r.to_did = d.did AND d.object_type = 'group' WHERE r.from_did = %d", $did);
  while ($group = db_fetch_object($result)) {
    $topics[l($group->title, api_url($group))] = api_link_documentation($group->summary, $branch);
  }
  if (count($topics) > 0) {
    return theme('api_related_topics', $topics);
  }
  return '';
}

/**
 * Returns a link to the file a documentation object is in.
 *
 * @param $object
 *   Documentation object.
 *
 * @return
 *   Formatted link to the file the object is in.
 */
function api_file_link($object) {
  return str_replace('/', '/<wbr>', dirname($object->file_name)) . '/<wbr>' . l(basename($object->file_name), api_url($object, TRUE));
}

/**
 * Returns HTML for a reference link on a function or file page.
 *
 * This theme hook is used for the "N functions call function()" and "N
 * functions implement hook()" links on function pages.
 *
 * @param $type
 *   The type of reference link, one of:
 *   - 'calls': Function calls.
 *   - 'implementations': Hook implementations.
 *   - 'theme_invokes': Theme calls to a function or file.
 *   - 'invokes': Calls to module_invokes and similar functions.
 *   - 'references': String references to a function name.
 * @param $count
 *   The number of referenced items.
 * @param $function
 *   The function or file object being referenced.
 *
 * @ingroup themeable
 */
function theme_api_function_reference_link($type, $count, $function) {
  if (!isset($function->branch_id)) {
    return '';
  }

  $link_title = api_reference_text($type, $count, $function);

  // Create the link path.
  $processed_file_name = str_replace(API_FILEPATH_SEPARATOR, API_FILEPATH_SEPARATOR_REPLACEMENT, $function->file_name);
  $branch = api_get_branch_by_id($function->branch_id);

  if ($function->object_type == 'file') {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/' . $type . '/'. $branch->branch_name;
  }
  else {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/function/' . $type . '/' . $function->object_name . '/' . $branch->branch_name;
  }

  return '<h3>'. l($link_title, $link_path) .'</h3>';
}

/**
 * Returns the page title or link text for a references page/link.
 *
 * @param $type
 *   The type of reference link, one of:
 *   - 'calls': Function calls.
 *   - 'implementations': Hook implementations.
 *   - 'theme_invokes': Theme calls to a function or file.
 *   - 'invokes': Calls to module_invokes and similar functions.
 *   - 'references': String references to a function name.
 * @param $count
 *   The number of referenced items.
 * @param $function
 *   The function or file object being referenced.
 *
 * @return
 *   Text to be used as the link to the reference listing page, or the title
 *   of the page.
 */
function api_reference_text($type, $count, $function) {
  $name_to_use = check_plain($function->title);
  if ($type == 'references') {
    $name_to_use = "'" . $name_to_use . "'";
  }
  elseif ($function->object_type == 'function') {
    $name_to_use .= '()';
  }

  if ($type == 'calls') {
    return format_plural($count, '1 call to !name', '@count calls to !name', array('!name' => $name_to_use));
  }
  elseif ($type == 'implementations') {
    return format_plural($count, '1 function implements !name', '@count functions implement !name', array('!name' => $name_to_use));
  }
  elseif ($type == 'theme_invokes') {
    return format_plural($count, '1 theme call to !name', '@count theme calls to !name', array('!name' => $name_to_use));
  }
  elseif ($type == 'invokes') {
    return format_plural($count, '1 invocation of !name', '@count invocations of !name', array('!name' => $name_to_use));
  }
  if ($type == 'references') {
    return format_plural($count, '1 string reference to !name', '@count string references to !name', array('!name' => $name_to_use));
  }

  return '';
}
