<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("RSS"),
  'description' => t('Control access by the existence of a substring in the domain of the url.'),
  'types'=>array('rss')

);


function test_rss_flow_settings_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  $form['channel_img'] = array(
    '#title' => t('Use of default image.'),
    '#type' => 'checkbox',
    '#default_value' => $form_state['link'],
    '#description' => t('Check here to link to the node author profile.'),
  );
  
}

function test_rss_flow_build(&$feed_infos){
 $feed_xml = drupal_http_request($feed_infos['uri']);
    if ($feed_xml) {
      list($valid_dom, $channel_last_build) = _test_createvaliddomfromfeed($feed_xml, $feed_infos);

      if ($channel_last_build > $feed_infos['last_build'] || true) {
        $tempFile = variable_get('wallymport_temp','').'/'.$feed_infos['flow_key'].'-'.$channel_last_build.'.tmp';
        $valid_dom->save($tempFile);
        $temp_return = wallymport_page_process_file($tempFile);
        file_delete($tempFile);

        if ($temp_return) {
          $object = array();
          $object = $feed_infos;
          $object['last_build'] = $channel_last_build;
          drupal_write_record('flowtonode_flows', $object, array('flow_key'));
        }
        
        $text_to_return = !empty($temp_return) ? 'Import done' : 'Import error';
        $text_to_return .= ' for file : '.$tempFile;
        $return[] = $text_to_return;
      } else {
        $return[] = is_string($valid_dom) ? $valid_dom : 'Channel not rebuild or no channel for the feed : '.$feed_infos['name'];
      }
    }
}




/**
 * Build a dom document based on an array. The array is given by the
 * rss_item_to_array function of wallytoolbox.
 * 
 * @param $feed_xml
 *   The entire feed obtain by drupal_http_request.
 * @param &$feed_infos
 *   The informations relative to this feed as stocked in de DB.
 * 
 * @return
 *   The dom document and the new build date of this feed.
 * 
 * @todo
 *   Replace default values by real values in the valid_dom
 */
function _test_createvaliddomfromfeed($feed_xml, &$feed_infos) {  
  $dom = new DomDocument();
  $dom->LoadXML($feed_xml->data);
  $channels = $dom->getElementsByTagName('channel');
  $rss_tags = $dom->getElementsByTagName('rss');
  $media_namespace = $rss_tags->item(0)->getAttributeNS('http://www.w3.org/2000/xmlns/', 'media');
  $channel_last_build = 0;
  
  if ($channels->item(0)) {
    $feed_destination = explode('/', $feed_infos['destination']);
    $sql_last_build = $feed_infos['last_build'];
    $feed_name = check_plain($feed_infos['name']);
    $channel_last_build = strtotime(_wallyflowtonode_checkifelementexists($channels->item(0), 'lastBuildDate'));
    
    if ($channel_last_build > $sql_last_build || empty($channel_last_build) || !isset($channel_last_build) || $channel_last_build == 0) {
      $feeds_md5 = isset($feed_infos['feeds_md5']) ? unserialize($feed_infos['feeds_md5']) : array();
      $new_feeds_md5 = array();
      $valid_dom = new DomDocument('1.0', 'UTF-8');
      $packages = $valid_dom->appendChild($valid_dom->createElement('Packages'));
      $packages->setAttribute('xmlns', 'http://rossel.audaxis.com');
      
      foreach ($channels->item(0)->getElementsByTagName('item') as $item) {
        $array_valid_dom = wallytoolbox_rss_item_to_array($channels->item(0), $item, $media_namespace, $feed_infos);

        $modif = FALSE;
        $elem_package = $array_valid_dom['Package'];
        $elem_md5 = md5($elem_package['PublicationDate'].$elem_package['PackageTitle']['value'].$elem_package['ExternalURI']['value'].$elem_package['Package']['MainStory']['TextBody']['value']);
        if (!isset($feeds_md5[$elem_md5])) {
          $modif = TRUE;
        }
        $new_feeds_md5[$elem_md5] = TRUE;
        
        if ($modif) {
          $package = $packages->appendChild($valid_dom->createElement('Package'));
          $package->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', 'ArticlePackageType');
          $package->setAttribute('PackageID', $array_valid_dom['Package']['PackageID']);
          $package->setAttribute('PublicationDate', $array_valid_dom['Package']['PublicationDate']);
          $package->setAttribute('CreationDate', $array_valid_dom['Package']['CreationDate']);
          $package->setAttribute('EmbargoDate', $array_valid_dom['Package']['EmbargoDate']);
          $package->setAttribute('Language', $array_valid_dom['Package']['Language']);
          $package->appendChild($valid_dom->createElement('PackageTitle', $array_valid_dom['Package']['PackageTitle']['value']));
          $package->appendChild($valid_dom->createElement('ExternalReference', $array_valid_dom['Package']['ExternalReference']['value']));
          $package->appendChild($valid_dom->createElement('ProductID', $array_valid_dom['Package']['ProductID']['value']));
          $destinations = $package->appendChild($valid_dom->createElement('Destinations'));
          $editions = $package->appendChild($valid_dom->createElement('Editions'));
          $chans = $package->appendChild($valid_dom->createElement('Channels'));
          $package->appendChild($valid_dom->createElement('PackageLayout', $feed_infos['package_layout']));
          $package->appendChild($valid_dom->createElement('Summary', $array_valid_dom['Package']['Summary']['value']));
          $package->appendChild($valid_dom->createElement('ExternalURI', $array_valid_dom['Package']['ExternalURI']['value']));
          $mainStory = $package->appendChild($valid_dom->createElement('MainStory'));
          
          $destination = $destinations->appendChild($valid_dom->createElement('Destination'));
          $destination->appendChild($valid_dom->createElement('DestinationPath', $array_valid_dom['Package']['Destinations']['Destination']['DestinationPath']['value']));
          $destination->setAttribute('DestinationRank', $array_valid_dom['Package']['Destinations']['Destination']['DestinationRank']);
          $destination->setAttribute('Position', $array_valid_dom['Package']['Destinations']['Destination']['Position']);
          $destination->setAttribute('DocumentLayout', $array_valid_dom['Package']['Destinations']['Destination']['DocumentLayout']);
          
          $edition = $editions->appendChild($valid_dom->createElement('Edition', $array_valid_dom['Package']['Editions']['Edition']['value']));
          $edition->setAttribute('EditionID', $array_valid_dom['Package']['Editions']['Edition']['EditionID']);
          
          $chan = $chans->appendChild($valid_dom->createElement('Channel'));
          $chan->setAttribute('Name', $array_valid_dom['Package']['Channels']['Channel']['Name']);
          
          $mainStory->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', 'TextObjectType');
          $mainStory->setAttribute('ObjectID', $array_valid_dom['Package']['MainStory']['ObjectID']);
          $mainStory->setAttribute('PublicationDate', $array_valid_dom['Package']['MainStory']['PublicationDate']);
          $mainStory->setAttribute('CreationDate', $array_valid_dom['Package']['MainStory']['CreationDate']);
          $mainStory->appendChild($valid_dom->createElement('Title', $array_valid_dom['Package']['MainStory']['Title']['value']));
          $mainStory->appendChild($valid_dom->createElement('ExternalReference', $array_valid_dom['Package']['MainStory']['ExternalReference']['value']));
          $taxonomy = $mainStory->appendChild($valid_dom->createElement('Taxonomy'));
          $vocabulary = $taxonomy->appendChild($valid_dom->createElement('Vocabulary'));
          $vocabulary->setAttribute('VocabularyName', $array_valid_dom['Package']['MainStory']['Taxonomy']['Vocabulary']['VocabularyName']);
          $vocabulary->setAttribute('VocabularyType', $array_valid_dom['Package']['MainStory']['Taxonomy']['Vocabulary']['VocabularyType']);
          foreach ($array_valid_dom['Package']['MainStory']['Taxonomy']['Vocabulary']['Term'] as $term) {
            $vocabulary->appendChild($valid_dom->createElement('Term', $term));
          }
          $mainStory->appendChild($valid_dom->createElement('Copyright', ($feed_infos['signature'] != '') ? $feed_infos['signature'] : $array_valid_dom['Package']['MainStory']['Copyright']['value']));
          $mainStory->appendChild($valid_dom->createElement('TextTitle', $array_valid_dom['Package']['MainStory']['TextTitle']['value']));
          $format_number = db_result(db_query("SELECT `format` FROM `filter_formats` WHERE `name` = '%s'", $feed_infos['body_html']));
          $cleaned_body = str_replace(array('<p>', '</p>'), '', check_markup($array_valid_dom['Package']['MainStory']['TextBody']['value'], $format_number));
          $mainStory->appendChild($valid_dom->createElement('TextBody', $cleaned_body));
  
          if (isset($array_valid_dom['Package']['EmbeddedContent']['EmbeddedObjects']['Object'])) {
            $embeddedContent = $package->appendChild($valid_dom->createElement('EmbeddedContent'));
            $embeddedObjects = $embeddedContent->appendChild($valid_dom->createElement('EmbeddedObjects'));
            foreach($array_valid_dom['Package']['EmbeddedContent']['EmbeddedObjects']['Object'] as $item_object) {
              $object = $embeddedObjects->appendChild($valid_dom->createElement('Object'));
              $object->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', $item_object['type']);
              $object->setAttribute('ObjectID', $item_object['ObjectID']);
              $object->setAttribute('PublicationDate', $item_object['PublicationDate']);
              $object->setAttribute('CreationDate', $item_object['CreationDate']);
              if (isset($item_object['MimeType'])) $object->setAttribute('MimeType', $item_object['MimeType']);
              $object->appendChild($valid_dom->createElement('Title', $item_object['Title']['value']));
              $object->appendChild($valid_dom->createElement('ExternalReference', $item_object['ExternalReference']['value']));
              if (isset($item_object['Copyright']['value'])) $object->appendChild($valid_dom->createElement('Copyright', $item_object['Copyright']['value']));
              $object->appendChild($valid_dom->createElement('FileURI', $item_object['FileURI']['value']));
              $object->appendChild($valid_dom->createElement('FileSize', $item_object['FileSize']['value']));
              if (isset($item_object['ThumbNail'])) {
                $thumbnail = $object->appendChild($valid_dom->createElement('Thumbnail'));
                $thumbnail->setAttribute('URI', $item_object['ThumbNail']['URI']);
                if (isset($item_object['ThumbNail']['W'])) $thumbnail->setAttribute('W', $item_object['ThumbNail']['W']);
                if (isset($item_object['ThumbNail']['H'])) $thumbnail->setAttribute('H', $item_object['ThumbNail']['H']);
              }
              if (isset($item_object['Rating'])) $object->appendChild($valid_dom->createElement('Rating', $item_object['Rating']['value']));
              if (isset($item_object['LinkTitle'])) $object->appendChild($valid_dom->createElement('LinkTitle', $item_object['LinkTitle']['value']));
              if (isset($item_object['LinkURI'])) $object->appendChild($valid_dom->createElement('LinkURI', $item_object['LinkURI']['value']));
            }
          }
        }
      }
      
      $feed_infos['feeds_md5'] = $new_feeds_md5;
    } else {
      drupal_set_message('The feed '.check_plain($feed_infos['name']).' wasn\'t rebuild since last run.', "notice");
      $valid_dom = 'Channel not rebuild for the feed : '.$feed_infos['name'];
    }
  } else {
    drupal_set_message('No channel in the '.check_plain($feed_infos['name']).' feed.', "notice");
    $valid_dom = 'No channel in the feed : '. $feed_infos['name'];
  }
  
  $return = array();
  $return[] = $valid_dom;
  $return[] = $channel_last_build;
  return $return;
}