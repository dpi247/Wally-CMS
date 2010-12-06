<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
function wallycontenttypes_rssread_ctools_content_types() {
    return array(
      'single' => TRUE,
      'title' => t('Simple RSS Reader'),
      'icon' => 'icon_node.png',
      'description' => t('Fetch a RSS feed'),
//      'required context' => new ctools_context_required(t('Node'), 'node'),
      'category' => t('Wally Tools'),
   'defaults' => array(),
    );
}

function wallycontenttypes_rssread_content_type_edit_form(&$form, &$form_state) {
   
  $conf = $form_state['conf'];
              
  $form['uri'] = array (
    '#type'             => 'textfield',
    '#title'            => t('URL of the RSS FEED'),
    '#required'         => TRUE,
  );

  $form['maxitem'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max Item to fetch'),
    '#required'         => TRUE,
  );


  $form['override_theme'] = array (
    '#type'             => 'checkbox',
    '#default_value'    => '',
    '#id'               =>  'override-theme-checkbox',
    '#title'            => 'Override Theming Function',
  );

  $form['override_theme_text'] = array (
    '#type'             => 'textfield',
    '#default_value'    => '',
    '#id'               => 'override-theme-textfield',
    '#dependency'       => array('override-theme-checkbox'=>array('1')),
    '#dependency_type'  => 'disable',
    '#process'          => array('ctools_dependent_process'),
    '#title'            => 'Theming Function Name',
  );

  return $form;  
}

function wallycontenttypes_rssread_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['uri']=$form['uri']['#value'];
  $form_state['conf']['override_theme']=$form['override_theme']['#value'];
  $form_state['conf']['override_theme_text']=$form['override_theme_text']['#value'];
  $form_state['conf']['maxitem']=$form['maxitem']['#value'];
}

function wallycontenttypes_rssread_content_type_render($subtype, $conf, $panel_args, $context) {

  $feed = array();
  
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;

  $block = new stdClass();
  $block->module = 'wallycontenttypes';
  
  $rss_uri = $conf['uri'];
  $override_theme = $conf['override_theme'];
  $override_theme_text = $conf['override_theme_text'];
  if (is_numeric($conf['maxitem'])) {
    $maxitem = $conf['maxitem'];
  } else {
    $maxitem = 10;
  }
  
  $rss = new DOMDocument();
  $rss->load($rss_uri);
  
  $c = 1; 
  foreach ($rss->getElementsByTagName('item') as $node) {
      $item = array (
        'title'=>$node->getElementsByTagName('title')->item(0)->nodeValue,
        'link' => $node->getElementsByTagName('link')->item(0)->nodeValue,
      );
      array_push($feed, $item);
  if (($c++)>$maxitem) break;
  }

  $block->title = 'RSS reader.';
  $block->content = theme("wallyct_rss",$feed, $conf); 

  return $block;
}
