<?php

/**
 * Render a list of destination path term. 
 *
 *  @param $separator
 *    An HTML string to place between terms. 
 *  @param $destinations
 *    An array of destinations cck fields.
 *  @param $prefix
 *    An HTML string to prefix the list.
 *  @param $suffix
 *    An HTML string to suffix the list.
 *  @param &$terms
 *    An array if needed to receive the term list. 
 * 
 *  @return string
 *    An HTML string of linked taxonomy terms.
 */

	foreach ($destinations as $destination) {
		$tempDest['taxID'] = $destination['tid'];
		$tempDest['tax'] = taxonomy_get_term($destination['tid']);
		$tempDest['dest'] = $tempDest['tax']->description;
		$tempDest['path'] = drupal_get_path_alias("/taxonomy/term/".$destination['tid']);
		$mainDest[] = $tempDest;
	}
  
  $count=1; 
  $html = ($prefix) ? $prefix : "";
  foreach($mainDest as $dest) {
    if ($count>1) $html .= $separator;  
		$html .= "<a href='".$dest['path']."' title='".$dest['taxID']."' rel='main destination'>".$dest['dest']."</a> ";
    $count++;
  }
  $html .= ($suffix) ? $suffix : "";
  
  print  $html;
?>
