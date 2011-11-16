<?php

/**
 * Transform an array in to a wallyXml (use in wallyflowtonode and wallyexport).
 *
 * @param $wally_array
 *   An array
 * @param valid_dom
 *   The dom to return fill with the array elements
 * @return
 *   $valid_dom
 */

function wallytoolbox_arraytowallyxml($wally_array, &$valid_dom, &$files_to_zip){
  $packages_node = $valid_dom->appendChild($valid_dom->createElement('Packages'));
  $packages_node->setAttribute('xmlns', 'http://rossel.audaxis.com');
  $packages_node->setAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');  
  $packages_node->setAttribute('xsi:schemaLocation', 'http://rossel.audaxis.com http://rossel.audaxis.com/xsd/packages.1.12.xsd');
  
  foreach ($wally_array['Packages']['Package'] as $package_array){
    $package_node = $packages_node->appendChild($valid_dom->createElement('Package'));
    //Package Type
    $package_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', $package_array['type']);
    
    $files_to_zip = array();
    //Set common fields
    _wallytoolbox_arraytowallyxml_fillPackageType($valid_dom, $package_node, $package_array);
    switch ($package_array['type']){
      case 'ArticlePackageType' :
        _wallytoolbox_arraytowallyxml_fillArticlePackageType($valid_dom, $package_node, $package_array);
        break;
      case 'GalleryPackageType' :
        _wallytoolbox_arraytowallyxml_fillGalleryPackageType($valid_dom, $package_node, $package_array, $files_to_zip);
        break;
      case 'PollPackageType' :
        _wallytoolbox_arraytowallyxml_fillPollPackageType($valid_dom,$package_node, $package_array);
        break;
    }
    if (isset($package_array['EmbeddedContent'])){
      $embeddedcontent_node = $package_node->appendChild($valid_dom->createElement('EmbeddedContent'));
      if (isset($package_array['EmbeddedContent']['EmbeddedObjects']['Object'])){
        $embeddedobjects_node = $embeddedcontent_node->appendChild($valid_dom->createElement('EmbeddedObjects'));
        foreach ($package_array['EmbeddedContent']['EmbeddedObjects']['Object'] as $object_array){
          $object_node = $embeddedobjects_node->appendChild($valid_dom->createElement('Object'));
          $object_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', $object_array['type']);
          _wallytoolbox_arraytowallyxml_fillObjectType($valid_dom, $object_node, $object_array);
          switch ($object_array['type']){
            case 'TextObjectType' :
              _wallytoolbox_arraytowallyxml_fillTextObjectType($valid_dom, $object_node, $object_array);
              break;
            case 'AudioObjectType':
              _wallytoolbox_arraytowallyxml_fillAudioObjectType($valid_dom, $object_node, $object_array, $files_to_zip);
              break;
            case 'VideoObjectType':
              _wallytoolbox_arraytowallyxml_fillVideoObjectType($valid_dom, $object_node, $object_array, $files_to_zip);
              break;
            case 'PhotoObjectType':
              _wallytoolbox_arraytowallyxml_fillPhotoObjectType($valid_dom, $object_node, $object_array, $files_to_zip);
              break;
            case 'PollObjectType':
              _wallytoolbox_arraytowallyxml_fillPollObjectType($valid_dom, $object_node, $object_array);
              break;
          }
        }
      }  
      if (isset($package_array['EmbeddedContent']['LinksLists']['LinksList'])){
        $linkslists_node = $embeddedcontent_node->appendChild($valid_dom->createElement('LinksLists'));
        foreach ($package_array['EmbeddedContent']['LinksLists']['LinksList'] as $linkslist_array){
          $linkslist_node = $linkslists_node->appendChild($valid_dom->createElement('LinksList'));
          _wallytoolbox_arraytowallyxml_filllinkslist($valid_dom, $linkslist_node, $linkslist_array);
        }
      }
    }
  }
  return $valid_dom;
}
/**
 * Set the common Package elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillPackageType(&$valid_dom, &$package_node, $package_array){

  $package_node->setAttribute('PackageID', $package_array['PackageID']);
  $package_node->setAttribute('PublicationDate', str_replace(' ','T',$package_array['PublicationDate']));
  $package_node->setAttribute('CreationDate', str_replace(' ','T',$package_array['CreationDate']));
  $package_node->setAttribute('LastUpdateDate', str_replace(' ','T',$package_array['LastUpdateDate']));
  $package_node->setAttribute('EmbargoDate', str_replace(' ','T',$package_array['EmbargoDate']));
  $package_node->setAttribute('UnPublishDate', str_replace(' ','T',$package_array['UnPublishDate']));
  $package_node->setAttribute('Language', $package_array['Language']);
  $package_node->setAttribute('Version', $package_array['Version']);
  $package_node->setAttribute('Action', 'CreateUpdate');
  
  $package_node->appendChild($valid_dom->createElement('ProductID', $package_array['ProductID']['value']));
  
  $package_node->appendChild($valid_dom->createElement('PackageTitle', $package_array['PackageTitle']['value']));
  $package_node->appendChild($valid_dom->createElement('PackageDescription', $package_array['PackageDescription']['value']));
  $package_node->appendChild($valid_dom->createElement('InternalReference', $package_array['InternalReference']['value']));
  $package_node->appendChild($valid_dom->createElement('ExternalReference', $package_array['ExternalReference']['value']));
  $destinations_node = $package_node->appendChild($valid_dom->createElement('Destinations'));

  foreach ($package_array['Destinations']['Destination'] as $pack_dest) {
    $destination_node = $destinations_node->appendChild($valid_dom->createElement('Destination'));
    $destination_node->appendChild($valid_dom->createElement('DestinationPath', $pack_dest['DestinationPath']['value']));
    $destination_node->setAttribute('DestinationRank', $pack_dest['DestinationRank']);
    $destination_node->setAttribute('Position', $pack_dest['Position']);
    $destination_node->setAttribute('DocumentLayout', $pack_dest['DocumentLayout']);
  }
  $channels_node = $package_node->appendChild($valid_dom->createElement('Channels'));
  foreach ($package_array['Channels']['Channel'] as $channel_array){
    $channel_node = $channels_node->appendChild($valid_dom->createElement('Channel'));
    $channel_node->setAttribute('Name', $channel_array['Name']);
  }
  $package_node->appendChild($valid_dom->createElement('CommentsAllowed', $package_array['CommentsAllowed']['value']));
  if (isset($package_array['FreeAccess']) & $package_array['FreeAccess']['value'] != NULL & $package_array['FreeAccess']['value'] != ''){
    $package_node->appendChild($valid_dom->createElement('FreeAccess', $package_array['FreeAccess']['value']));
  } else {
    $freeaccess_node = $package_node->appendChild($valid_dom->createElement('FreeAccess'));
    $freeaccess_node->setAttribute('xsi:nil', 'true');
  }
  $package_node->appendChild($valid_dom->createElement('PackageLayout', $package_array['PackageLayout']['value']));
  
  $package_node->appendChild($valid_dom->createElement('Summary', $package_array['Summary']['value']));
  if (isset($package_array['ExternalURI']) & $package_array['ExternalURI']['value'] != NULL & $package_array['ExternalURI']['value'] != ''){
    $package_node->appendChild($valid_dom->createElement('ExternalURI', $package_array['ExternalURI']['value']));
  } else {
    $externaluri_node = $package_node->appendChild($valid_dom->createElement('ExternalURI'));
    $externaluri_node->setAttribute('xsi:nil', 'true');
  }
  

}
/**
 * Set the Article Package elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillArticlePackageType(&$valid_dom, &$package_node, $package_array){

  $mainstory_node = $package_node->appendChild($valid_dom->createElement('MainStory'));
  $mainstory_array = $package_array['MainStory'];
  $mainstory_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', 'TextObjectType');

  _wallytoolbox_arraytowallyxml_fillObjectType($valid_dom, $mainstory_node, $mainstory_array);

  _wallytoolbox_arraytowallyxml_fillTextObjectType($valid_dom, $mainstory_node,$mainstory_array);

}
/**
 * Set the Gallery Package elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillGalleryPackageType(&$valid_dom, &$package_node, $package_array, &$files_to_zip){
  $mainobject_node = $package_node->appendChild($valid_dom->createElement('MainObject'));
  _wallytoolbox_arraytowallyxml_fillObjectType($valid_dom,$mainobject_node,$package_array['MainObject']);
  switch ($package_array['MainObject']['type']){
    case 'AudioObjectType':
      _wallytoolbox_arraytowallyxml_fillAudioObjectType($valid_dom, $mainobject_node, $package_array['MainObject'], $files_to_zip);
      break;
    case 'VideoObjectType':
      _wallytoolbox_arraytowallyxml_fillVideoObjectType($valid_dom, $mainobject_node, $package_array['MainObject'], $files_to_zip);
      break;
    case 'PhotoObjectType':
      _wallytoolbox_arraytowallyxml_fillPhotoObjectType($valid_dom, $mainobject_node, $package_array['MainObject'], $files_to_zip);
      break;
  }
}
/**
 * Set the Gallery Package elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillPollPackageType(&$valid_dom, &$package_node, $package_array){
  if (isset($package_array['Poll'])){
    foreach ($package_array['Poll'] as $poll_array){
      $poll_node = $package_node->appendChild($valid_dom->createElement('Poll'));
      _wallytoolbox_arraytowallyxml_fillObjectType($valid_dom,$poll_node,$poll_array);
      _wallytoolbox_arraytowallyxml_fillPollObjectType($valid_dom,$poll_node,$poll_array);
    }  
  }
}
/**
 * Set the LinksList elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return$link_node->append
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_filllinkslist(&$valid_dom, &$linkslist_node, $linkslist_array){
  $linkslist_node->appendChild($valid_dom->createElement('InternalReference', $linkslist_array['InternalReference']['value']));
  $linkslist_node->appendChild($valid_dom->createElement('ExternalReference', $linkslist_array['ExternalReference']['value']));
  $linkslist_node->appendChild($valid_dom->createElement('Title', $linkslist_array['Title']['value']));
  $linkslist_node->appendChild($valid_dom->createElement('Summary', $linkslist_array['Summary']['value']));
  foreach ($linkslist_array['Links'] as $links_array){
    $links_node = $linkslist_node->appendChild($valid_dom->createElement('Links'));
    if (isset($links_array['Link'])){
      foreach ($links_array['Link'] as $link_array){
        $link_node = $links_node->appendChild($valid_dom->createElement('Link'));
        $link_node->appendChild($valid_dom->createElement('InternalReference', $link_array['InternalReference']['value']));
        $link_node->appendChild($valid_dom->createElement('ExternalReference', $link_array['ExternalReference']['value']));  
        $link_node->appendChild($valid_dom->createElement('Title', $link_array['Title']['value']));  
        $link_node->appendChild($valid_dom->createElement('Description', $link_array['Description']['value']));  
        $link_node->appendChild($valid_dom->createElement('URI', $link_array['URI']['value']));     
      }
    }
  }
}
/**
 * Set the Object elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillObjectType(&$valid_dom, &$object_node, $object_array){

  $object_node->setAttribute('ObjectID', $object_array['ObjectID']);
  $object_node->setAttribute('ObjectDescription', $object_array['ObjectDescription']);
  $object_node->setAttribute('PublicationDate', $object_array['PublicationDate']);
  $object_node->setAttribute('CreationDate', $object_array['CreationDate']);
  $object_node->setAttribute('LastUpdateDate', $object_array['LastUpdateDate']);
  
  $object_node->appendChild($valid_dom->createElement('Title', $object_array['Title']['value']));
  $object_node->appendChild($valid_dom->createElement('ExternalReference', $object_array['ExternalReference']['value']));
  $object_node->appendChild($valid_dom->createElement('InternalReference', $object_array['InternalReference']['value']));
  

  if (isset($object_array['Taxonomy']) & count($object_array['Taxonomy']) != 0){
    _wallytoolbox_arraytowallyxml_createVocItems($valid_dom, $object_node, $object_array['Taxonomy']);
  }

  $object_node->appendChild($valid_dom->createElement('Copyright', $object_array['Copyright']['value'])); 
  $object_node->appendChild($valid_dom->createElement('ByLine', $object_array['ByLine']['value']));
  
  if (isset($object_array['EmbeddedObjectsOrder']) & $object_array['EmbeddedObjectsOrder'] != NULL & $object_array['EmbeddedObjectsOrder'] != ''){
    $object_node->appendChild($valid_dom->createElement('EmbeddedObjectsOrder', $object_array['EmbeddedObjectsOrder']['value']));
  } else {
    $embeddedobjectorder = $object_node->appendChild($valid_dom->createElement('EmbeddedObjectsOrder'));
    $embeddedobjectorder->setAttribute('xsi:nil','true');
  }
}

/**
 * Set the Text Object elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillTextObjectType(&$valid_dom, &$mainstory_node, $mainstory_array){
  $text_barette_node = $mainstory_node->appendChild($valid_dom->createElement('TextBarette'));
  $text_barette_node->appendChild($valid_dom->createCDATASection($mainstory_array['TextBarette']['value']));
  $text_foretitle_node = $mainstory_node->appendChild($valid_dom->createElement('TextForeTitle'));
  $text_foretitle_node->appendChild($valid_dom->createCDATASection($mainstory_array['TextForeTitle']['value']));
  $text_title_node = $mainstory_node->appendChild($valid_dom->createElement('TextTitle'));
  $text_title_node->appendChild($valid_dom->createCDATASection($mainstory_array['TextTitle']['value']));
  $text_subtitle_node = $mainstory_node->appendChild($valid_dom->createElement('TextSubTitle'));
  $text_subtitle_node->appendChild($valid_dom->createCDATASection($mainstory_array['TextSubTitle']['value']));
  $text_chapo_node = $mainstory_node->appendChild($valid_dom->createElement('TextChapo'));
  $text_chapo_node->appendChild($valid_dom->createCDATASection($mainstory_array['TextChapo']['value']));
  $text_body_node = $mainstory_node->appendChild($valid_dom->createElement('TextBody'));
  $text_body_node->appendChild($valid_dom->createCDATASection($mainstory_array['TextBody']['value']));
  $text_extmed_node = $mainstory_node->appendChild($valid_dom->createElement('ExtractMedium'));
  $text_extmed_node->appendChild($valid_dom->createCDATASection($mainstory_array['ExtractMedium']['value']));
  $text_extshort_node = $mainstory_node->appendChild($valid_dom->createElement('ExtractShort'));
  $text_extshort_node->appendChild($valid_dom->createCDATASection($mainstory_array['ExtractShort']['value']));

}
/**
 * Set the Text PhotoObject elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillMultimediaObjectType(&$valid_dom, &$object_node, $object_array){
  $object_node->appendChild($valid_dom->createElement('Caption',$object_array['Caption']['value']));
  $object_node->appendChild($valid_dom->createElement('FileURI',$object_array['FileURI']['value']));
  
  if ($object_array['FileSize']['value'] == '' | $object_array['FileSize']['value'] == NULL){
    $object_array['FileSize']['value'] = 0;
  }
  $object_node->appendChild($valid_dom->createElement('FileSize',$object_array['FileSize']['value']));
  $thumbnail_node = $object_node->appendChild($valid_dom->createElement('Thumbnail'));
  $thumbnail_node->setAttribute('URI', $object_array['Thumbnail']['URI']);
  if (isset($object_array['Thumbnail']) & $object_array['Thumbnail']['value'] != NULL & $object_array['Thumbnail']['value'] != ''){
    
    if (isset($object_array['Thumbnail']['Type']) & $object_array['Thumbnail']['Type'] != NULL & $object_array['Thumbnail']['Type'] != '')
      $thumbnail_node->setAttribute('Type',$object_array['Thumbnail']['Type']);
    if (isset($object_array['Thumbnail']['Title']) & $object_array['Thumbnail']['Title'] != NULL & $object_array['Thumbnail']['Title'] != '')
      $thumbnail_node->setAttribute('Title',$object_array['Thumbnail']['Title']);
    if (isset($object_array['Thumbnail']['W']) & $object_array['Thumbnail']['W'] != NULL & $object_array['Thumbnail']['W'] != '')
      $thumbnail_node->setAttribute('W',$object_array['Thumbnail']['W']);
    if (isset($object_array['Thumbnail']['H']) & $object_array['Thumbnail']['H'] != NULL & $object_array['Thumbnail']['H'] != '')
      $thumbnail_node->setAttribute('H',$object_array['Thumbnail']['H']);
  } else {
    $thumbnail_node->setAttribute('xsi:nil', 'true');
  }
  if (isset($object_array['Rating']) & $object_array['Rating']['value'] != NULL & $object_array['Rating']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('Rating',$object_array['Rating']['value']));
  } else {
    $rating_node = $object_node->appendChild($valid_dom->createElement('Rating'));
    $rating_node->setAttribute('xsi:nil','true');
  }
    
  if (isset($object_array['MimeType']) & $object_array['MimeType'] != NULL & $object_array['MimeType'] != '')
    $object_node->setAttribute('MimeType',$object_array['MimeType']);
  
}
/**
 * Set the Text PhotoObject elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillPhotoObjectType(&$valid_dom, &$object_node, $object_array, &$files_to_zip){
  _wallytoolbox_arraytowallyxml_fillMultimediaObjectType($valid_dom,$object_node, $object_array);
  if (isset($object_array['PhotoFile'])){
    $files_to_zip[] = $object_array['PhotoFile'];
  }
  $crops_node = $object_node->appendChild($valid_dom->createElement('Crops'));
  if (isset($object_array['Crops'])){
    foreach ($object_array['Crops']['Crop'] as $crop_array){
      $crop_node = $crops_node->appendChild($valid_dom->createElement('Crop'));
      if (isset($crop_array['Type']) & $crop_array['Type'] != '' & $crop_array['Type'] != NULL)
        $crop_node->setAttribute('Type',$crop_array['Type']);
      if (isset($crop_array['X']) & $crop_array['X'] != '' & $crop_array['X'] != NULL)
        $crop_node->setAttribute('X',$crop_array['X']);
      if (isset($crop_array['Y']) & $crop_array['Y'] != '' & $crop_array['Y'] != NULL)
        $crop_node->setAttribute('Y',$crop_array['Y']);
      if (isset($crop_array['W']) & $crop_array['W'] != '' & $crop_array['W'] != NULL)
        $crop_node->setAttribute('W',$crop_array['W']);
      if (isset($crop_array['H']) & $crop_array['H'] != '' & $crop_array['H'] != NULL)
        $crop_node->setAttribute('H',$crop_array['H']);
    }
  } else {
    $crops_node->setAttribute('xsi:nil', 'true');
  }
  $object_node->appendChild($valid_dom->createElement('LinkTitle',$object_array['LinkTitle']['value']));
  if (isset($object_array['LinkURI']) & $object_array['LinkURI']['value'] != NULL & $object_array['LinkURI']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('LinkURI',$object_array['LinkURI']['value']));
  } else {
    $linkuri_node = $object_node->appendChild($valid_dom->createElement('LinkURI'));
    $linkuri_node->setAttribute('xsi:nil', 'true');
  }
  if (isset($object_array['Width']) & $object_array['Width']['value'] != NULL & $object_array['Width']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('Width',$object_array['Width']['value']));
  } else {
    $width_node = $object_node->appendChild($valid_dom->createElement('Width'));
    $width_node->setAttribute('xsi:nil', 'true');
  }
  if (isset($object_array['Height']) & $object_array['Height']['value'] != NULL & $object_array['Height']['value'] != ''){
    $object_node->appendChild('Height',$object_array['Height']['value']);
  } else {
    $height_node = $object_node->appendChild($valid_dom->createElement('Height'));
    $height_node->setAttribute('xsi:nil', 'true');
  }
}
/**
 * Set the Text AudioObject elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillAudioObjectType(&$valid_dom, &$object_node, $object_array, &$files_to_zip){
  _wallytoolbox_arraytowallyxml_fillMultimediaObjectType($valid_dom,$object_node, $object_array);
  if (isset($object_array['AudioFile'])){
    $files_to_zip[] = $object_array['AudioFile'];
  }
  if (isset($object_array['AudioLength']) & $object_array['AudioLength']['value'] != NULL & $object_array['AudioLength']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('AudioLength',$object_array['AudioLength']['value']));
  } else {
    $audiolength_node = $object_node->appendChild($valid_dom->createElement('AudioLength'));
    $audiolength_node->setAttribute('xsi:nil', 'true');
  }
  $object_node->appendChild($valid_dom->createElement('LinkTitle',$object_array['LinkTitle']['value']));
  if (isset($object_array['LinkURI']) & $object_array['LinkURI']['value'] != NULL & $object_array['LinkURI']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('LinkURI',$object_array['LinkURI']['value']));
  } else {
    $linkuri_node = $object_node->appendChild($valid_dom->createElement('LinkURI'));
    $linkuri_node->setAttribute('xsi:nil', 'true');
  }
}
/**
 * Set the Text VideoObject elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillVideoObjectType(&$valid_dom, &$object_node, $object_array, &$files_to_zip){
  _wallytoolbox_arraytowallyxml_fillMultimediaObjectType($valid_dom,$object_node, $object_array);
  if (isset($object_array['VideoFile'])){
    $files_to_zip[] = $object_array['VideoFile'];
  }
  if (isset($object_array['VideoLength']) & $object_array['VideoLength']['value'] != NULL & $object_array['VideoLength']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('VideoLength',$object_array['VideoLength']['value']));
  } else {
    $videolength_node = $object_node->appendChild($valid_dom->createElement('VideoLength'));
    $videolength_node->setAttribute('xsi:nil', 'true');
  }
  if (isset($object_array['LinkTitle']) & $object_array['LinkTitle']['value'] != NULL & $object_array['LinkTitle']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('LinkTitle',$object_array['LinkTitle']['value']));
  if (isset($object_array['LinkURI']) & $object_array['LinkURI']['value'] != NULL & $object_array['LinkURI']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('LinkURI',$object_array['LinkURI']['value']));
  } else {
    $linkuri_node = $object_node->appendChild($valid_dom->createElement('LinkURI'));
    $linkuri_node->setAttribute('xsi:nil', 'true');
  }
  
}
/**
 * Set the Text PollObject elements
 *
 * @param $valid_dom
 *   Dom
 * @param $package_node
 *   the dom node package
 * @param $package_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillPollObjectType($valid_dom, $object_node, $object_array){
  $object_node->appendChild($valid_dom->createElement('Summary',$object_array['Summary']['value']));
  $object_node->appendChild($valid_dom->createElement('DateOpen',$object_array['DateOpen']['value']));
  if (isset($object_array['DateClose']) & $object_array['DateClose']['value'] != NULL & $object_array['DateClose']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('DateClose',$object_array['DateClose']['value']));
  } else {
    $dateclose_node = $object_node->appendChild($valid_dom->createElement('DateClose'));
    $dateclose_node->setAttribute('xsi:nil', 'true');
  }
  $pollquestions_node = $object_node->appendChild($valid_dom->createElement('PollQuestions'));
  foreach ($object_array['PollQuestions']['PollQuestion'] as $pollquestion){
    $pollquestion_node = $pollquestions_node->appendChild($valid_dom->createElement('PollQuestion'));
    $pollquestion_node->appendChild($valid_dom->createElement('Question',$pollquestion['Question']['value']));
    $pollquestion_node->appendChild($valid_dom->createElement('Description',$pollquestion['Description']['value']));
    $answers_node = $pollquestion_node->appendChild($valid_dom->createElement('Answers'));
    foreach($pollquestion['Answers']['Answer'] as $answer){
      $answers_node->appendChild($valid_dom->createElement('Answer'), $answer['value']);
    }
    if (isset($pollquestion['Choice']) & $pollquestion['Choice'] != NULL & $pollquestion['Choice'] != ''){
      $pollquestion_node->setAttribute('Choice', $pommequestion['Choice']);
    }
  }
}
/**
 * Set the Taxonomy elements
 *
 * @param $valid_dom
 *   Dom
 * @param $object_node
 *   the dom node object
 * @param $taxonomy_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_createVocItems(&$valid_dom, &$object_node, $taxonomy_array){
  $taxonomy_node = $object_node->appendChild($valid_dom->createElement('Taxonomy'));
  
  foreach ($taxonomy_array['Vocabulary'] as $vocabulary_array){
    $vocabulary_node = $taxonomy_node->appendChild($valid_dom->createElement('Vocabulary'));
    $vocabulary_node->setAttribute('VocabularyName',$vocabulary_array['VocabularyName']);
    if (isset($vocabulary_array['VocabularyDescription']) & $vocabulary_array['VocabularyDescription'] != NULL & $vocabulary_array['VocabularyDescription'] != ''){
      $vocabulary_node->setAttribute('VocabularyDescription',$vocabulary_array['VocabularyDescription']);
    }
    if (isset($vocabulary_array['VocabularyType']) & $vocabulary_array['VocabularyType'] != NULL & $vocabulary_array['VocabularyType'] != ''){
      $vocabulary_node->setAttribute('VocabularyType',$vocabulary_array['VocabularyType']);
    }
    if (isset($vocabulary_array['Term'])){
      foreach($vocabulary_array['Term'] as $term_array){
        $term_node = $vocabulary_node->appendChild($valid_dom->createElement('Term'));
        $term_type = NULL;
        if (isset($term_array['type']) & $term_array['type'] != NULL & $term_array['type'] != ''){
          $term_node->setAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:type', $term_array['type']);
          $term_type = $term_array['type'];
        }
        if (isset($term_array['Name']) & $term_array['Name'] != NULL & $term_array['Name'] != ''){
          $term_node->setAttribute('Name',$term_array['Name']);
        }
        if (isset($term_array['ID']) & $term_array['ID'] != NULL & $term_array['ID'] != ''){
          $term_node->setAttribute('ID',$term_array['ID']);
        }
        if (isset($term_array['ConfidenceScore']) & $term_array['ConfidenceScore'] != NULL & $term_array['ConfidenceScore'] != ''){
          $term_node->setAttribute('ConfidenceScore',$term_array['ConfidenceScore']);
        }
        if (isset($term_array['RelevancyScore']) & $term_array['RelevancyScore'] != NULL & $term_array['RelevancyScore'] != ''){
          $term_node->setAttribute('RelevancyScore',$term_array['RelevancyScore']);
        }
        if (isset($term_array['Variations']) & $term_array['Variations'] != NULL & $term_array['Variations'] != ''){
          $variations_node = $term_node->appendChild($valid_dom->createElement('Variations'));
          if (isset($term_array['Variations']['Variation'])){
            foreach ($term_array['Variations']['Variation'] as $variation_array){
              if ($variation_array['value'] != '' & $variation_array['value'] != NULL){
                $variations_node->appendChild($valid_dom->createElement('Variation', $variation_array['value']));
              }
            }
          }
        }
        switch ($term_type){
          case 'PersonType':
            _wallytoolbox_arraytowallyxml_fillPersonType($valid_dom,$term_node,$term_array);
            break;
          case 'EntityType':
            _wallytoolbox_arraytowallyxml_fillEntityType($valid_dom,$term_node,$term_array);
            break;
          case 'LocationType':
            _wallytoolbox_arraytowallyxml_fillLocationType($valid_dom,$term_node,$term_array);
            break;
        }
      }
    }
  }
}
/**
 * Set the Person elements
 *
 * @param $valid_dom
 *   Dom
 * @param $term_node
 *   the dom term object
 * @param $term_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillPersonType(&$valid_dom,&$term_node,$term_array){

  $term_node->appendChild($valid_dom->createElement('PersonLastName',$term_array['PersonLastName']['value']));
  $term_node->appendChild($valid_dom->createElement('PersonFirstName',$term_array['PersonFirstName']['value']));
  $term_node->appendChild($valid_dom->createElement('PersonNickName',$term_array['PersonNickName']['value']));
  if (isset($term_array['PersonBirthDate']) & $term_array['PersonBirthDate']['value'] != NULL & $term_array['PersonBirthDate']['value'] != ''){
    list($birthdate,$birthhour) = explode(' ',$term_array['PersonBirthDate']['value']);
    $term_node->appendChild($valid_dom->createElement('PersonBirthDate',$birthdate));
  } else {
    $personbirthdate_node = $term_node->appendChild($valid_dom->createElement('PersonBirthDate'));
    $personbirthdate_node->setAttribute('xsi:nil', 'true');
  }
  if (isset($term_array['PersonSex']) & $term_array['PersonSex']['value'] != NULL & $term_array['PersonSex']['value'] != ''){
    $term_node->appendChild($valid_dom->createElement('PersonSex',$term_array['PersonSex']['value']));
  } else {
    $personsex_node = $term_node->appendChild($valid_dom->createElement('PersonSex'));
    $personsex_node->setAttribute('xsi:nil', 'true');
  }
  $term_node->appendChild($valid_dom->createElement('PersonEmail',$term_array['PersonEmail']['value']));
  $term_node->appendChild($valid_dom->createElement('PersonWebSite',$term_array['PersonWebSite']['value']));
  $term_node->appendChild($valid_dom->createElement('PersonPhoneNumber',$term_array['PersonPhoneNumber']['value']));

}
/**
 * Set the Entity elements
 *
 * @param $valid_dom
 *   Dom
 * @param $term_node
 *   the dom term object
 * @param $term_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillEntityType(&$valid_dom,&$term_node,$term_array){

  $term_node->appendChild($valid_dom->createElement('EntityDescription',$term_array['EntityDescription']['value']));
  $term_node->appendChild($valid_dom->createElement('EntityServices',$term_array['EntityServices']['value']));
 
}
/**
 * Set the Location elements
 *
 * @param $valid_dom
 *   Dom
 * @param $term_node
 *   the dom term object
 * @param $term_array
 *   The array representing the node or the rss
 *
 * @return
 *   $valid_dom
 */
function _wallytoolbox_arraytowallyxml_fillLocationType(&$valid_dom,&$term_node,$term_array){
  $address_node = $term_node->appendChild($valid_dom->createElement('Address'));
  if (isset($term_array['Address'])){
    foreach ($term_array['Address']['AddressLine'] as $addressline){
      $address_node->appendChild($valid_dom->createElement('AddressLine',$addressline));
    }
    $address_node->appendChild($valid_dom->createElement('City',$term_array['Address']['City']['value']));
    $address_node->appendChild($valid_dom->createElement('Zip',$term_array['Address']['Zip']['value']));
    $address_node->appendChild($valid_dom->createElement('State',$term_array['Address']['State']['value']));
    $address_node->appendChild($valid_dom->createElement('Country',$term_array['Address']['Country']['value']));
  } else {
    $address_node->setAttribute('xsi:nil','true');
  }
  if (isset($term_array['LocationGeo']) & $term_array['LocationGeo']['value'] != NULL & $term_array['LocationGeo']['value'] != NULL){
    $term_node->appendChild($valid_dom->createElement('LocationGeo',$term_array['LocationGeo']['value']));
  } else {
    $locationgeo_node = $term_node->appendChild($valid_dom->createElement('LocationGeo'));
    $locationgeo_node->setAttribute('xsi:nil','true');
  } 
}