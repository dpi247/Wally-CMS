<?php
//to delete
function wallytoolbox_arraytowallyxml_createfalsearray(){
  $wally_array = array('Packages' => array ());
  $wally_array['Packages'] = array('Package' => array());
  $wally_array['Packages']['Package'][0] = array(
    'PackageID' => 'pakageid',
  	'CreationDate' => '2011-07-28T15:57:28',
    'LastUpdateDate' => NULL,
    'type' => 'ArticlePackageType',
    'ProductID' => array('value' => 'productid'),
    'Destinations' => array('Destination' => array(
  0 => array(
    	'DocumentLayout' => 'small',
        'DestinationRank' => 0,
        'Position' => 'default_destination_block',
        'DestinationPath' => array('value' => 'ACTUALITé/MONDE'),
  ),
  1 => array(
    	'DocumentLayout' => 'small',
        'DestinationRank' => 0,
        'Position' => 'default_destination_block',
        'DestinationPath' => array('value' => 'ACCUEIL LAPROVINCE.BE')
  ),
  )),
    'Channels' => array('Channel' => array(
  0 => array( 'Name' => 'web'),
  1 => array( 'Name' => 'autre'),
  )),
    'Summary' => array('value' => 'summary blabla'),
    'Mainstory' => array(
      'ObjectID' => 'objectid',
      'CreationDate' => '2011-10-11T12:39:34',
      'Title' => array('value' => 'title'),
      'ExternalReference' => array('value' => 'externalreferece'),
      'Taxonomy' => array('Vocabulary' => array(
        0 => array (
          'Term' => array(
            0 => array(
              'type' => 'PersonType',
              'Name' => 'termname',
              'ID' => 'idterm',  
              'PersonLastName' => array('value' => 'Henin'),
            ),
          ),
          'VocabularyName' => 'vocabularyname',
          'VocabularyDescription' => 'vocabulardescr',
          'VocabularyType' => 'categorized',  
        ),
        1 => array (
          'Term' => array(
            0 => array(
              'Name' => 'termname',
              'ID' => 'idterm',  
            ),
          ),
          'VocabularyName' => 'Categories',
          'VocabularyDescription' => 'vocabulardescr',
          'VocabularyType' => 'categorized',  
        ),
        2 => array (
          'Term' => array(
            0 => array(
              'type' => 'LocationType',
              'Name' => 'termname',
              'ID' => 'idterm',  
              'Address' => array('AddressLine' => array( 0 => 'ruemacin'),'City' => 'ville'),
            ),
          ),
          'VocabularyName' => 'vocabularyname',
          'VocabularyDescription' => 'vocabulardescr',
          'VocabularyType' => 'categorized',  
        ),
    )),
  ),
  'EmbeddedContent' => array(
    'EmbeddedObjects' => array('Object' => array(
      0 => array(),  
  ))),
  );
  $valid_dom = new DomDocument('1.0', 'UTF-8');
  wallytoolbox_arraytowallyxml($wally_array,$valid_dom);
  $valid_dom->save('sites/default/files/export/test_xml.xml');
}


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
  dsm($wally_array);
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
        $linkslists_node = $package_node->appendChild($valid_dom->createElement('LinksLists'));
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
  $package_node->setAttribute('PublicationDate', str_replace(' ', 'T', $package_array['PublicationDate']));
  $package_node->setAttribute('CreationDate', str_replace(' ', 'T', $package_array['CreationDate']));
  if (isset($package_array['LastUpdateDate']) & $package_array['LastUpdateDate'] != NULL & $package_array['LastUpdateDate'] != ''){
    $package_node->setAttribute('LastUpdateDate', str_replace(' ', 'T', $package_array['LastUpdateDate']));
  }
  if (isset($package_array['EmbargoDate']) & $package_array['EmbargoDate'] != NULL & $package_array['EmbargoDate'] != ''){
    $package_node->setAttribute('EmbargoDate', str_replace(' ', 'T', $package_array['EmbargoDate']));
  }
  if (isset($package_array['UnPublishDate']) & $package_array['UnPublishDate'] != NULL & $package_array['UnPublishDate'] != ''){
    $package_node->setAttribute('UnPublishDate', str_replace(' ', 'T', $package_array['UnPublishDate']));
  }
  if (isset($package_array['Language']) & $package_array['Language'] != NULL & $package_array['Language'] != ''){
    $package_node->setAttribute('Language', $package_array['Language']);
  }
  if (isset($package_array['Version']) & $package_array['Version'] != NULL & $package_array['Version'] != ''){
    $package_node->setAttribute('Version', $package_array['Version']);
  }
  $package_node->appendChild($valid_dom->createElement('ProductID', $package_array['ProductID']['value']));
  $package_node->appendChild($valid_dom->createElement('Newsroom', ''));//champs vide
  $package_node->appendChild($valid_dom->createElement('StoryFolder', ''));//champs vide
  $package_node->appendChild($valid_dom->createElement('Desk1', ''));//champs vide

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
  if (isset($package_array['CommentsAllowed']) & $package_array['CommentsAllowed']['value'] != NULL & $package_array['CommentsAllowed']['value'] != ''){
    $package_node->appendChild($valid_dom->createElement('CommentsAllowed', $package_array['CommentsAllowed']['value']));
  }
  if (isset($package_array['FreeAccess']) & $package_array['FreeAccess']['value'] != NULL & $package_array['FreeAccess']['value'] != ''){
    $package_node->appendChild($valid_dom->createElement('FreeAccess', $package_array['FreeAccess']['value']));
  }
  if (isset($package_array['AutoPublish']) & $package_array['AutoPublish']['value'] != NULL & $package_array['AutoPublish']['value'] != ''){
    $package_node->appendChild($valid_dom->createElement('AutoPublish', $package_array['AutoPublish']['value']));
  }
  if (isset($package_array['PackageLayout']) & $package_array['PackageLayout']['value'] != NULL & $package_array['PackageLayout']['value'] != ''){
    $package_node->appendChild($valid_dom->createElement('PackageLayout', $package_array['PackageLayout']['value']));
  }
  $package_node->appendChild($valid_dom->createElement('Summary', $package_array['Summary']['value']));
  if (isset($package_array['ExternalURI']) & $package_array['ExternalURI']['value'] != NULL & $package_array['ExternalURI']['value'] != ''){
    $package_node->appendChild($valid_dom->createElement('ExternalURI', $package_array['ExternalURI']['value']));
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
  foreach ($package_array['Poll'] as $poll_array){
    $poll_node = $package_node->appendChild($valid_dom->createElement('Poll'));
    _wallytoolbox_arraytowallyxml_fillObjectType($valid_dom,$poll_node,$poll_array);
    _wallytoolbox_arraytowallyxml_fillPollObjectType($valid_dom,$poll_node,$poll_array);
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
  if (isset($linkslist_array['InternalReference']) & $linkslist_array['InternalReference'] != NULL & $linkslist_array['InternalReference'] != '')
    $linkslist_node->appendChild($valid_dom->createElement('InternalReference', $linkslist_array['InternalReference']['value']));
  $linkslist_node->appendChild($valid_dom->createElement('ExternalReference', $linkslist_array['ExternalReference']['value']));
  $linkslist_node->appendChild($valid_dom->createElement('Title', $linkslist_array['Title']['value']));
  if (isset($linkslist_array['Summary']) & $linkslist_array['Summary'] != NULL & $linkslist_array['Summary'] != '')
    $linkslist_node->appendChild($valid_dom->createElement('Summary', $linkslist_array['Summary']['value']));
  foreach ($linkslist_array['Links'] as $links_array){
    $links_node = $linkslist_node->appendChild($valid_dom->createElement('Links'));
    if (isset($links_array['Link'])){
      foreach ($links_array['Link'] as $link_array){
        $link_node = $links_node->appendChild($valid_dom->createElement('Link'));
        if (isset($link_array['InternalReference']) & $link_array['InternalReference']['value'] & $link_array['InternalReference']['value'] !=  '')
          $link_node->appendChild($valid_dom->createElement('InternalReference', $link_array['InternalReference']['value']));
        if (isset($link_array['ExternalReference']) & $link_array['ExternalReference']['value'] & $link_array['ExternalReference']['value'] !=  '')
          $link_node->appendChild($valid_dom->createElement('ExternalReference', $link_array['ExternalReference']['value']));  
        $link_node->appendChild($valid_dom->createElement('Title', $link_array['Title']['value']));  
        if (isset($link_array['Description']) & $link_array['Description']['value'] & $link_array['Description']['value'] !=  '')
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
  if (isset($object_array['ObjectDescription']) & $object_array['ObjectDescription'] != NULL & $object_array['ObjectDescription'] != ''){
    $object_node->setAttribute('ObjectDescription', $object_array['ObjectDescription']);
  }
  if (isset($object_array['PublicationDate']) & $object_array['PublicationDate'] != NULL & $object_array['PublicationDate'] != ''){
    $object_node->setAttribute('PublicationDate', $object_array['PublicationDate']);
  }
  $object_node->setAttribute('CreationDate', $object_array['CreationDate']);
  if (isset($object_array['LastUpdateDate']) & $object_array['LastUpdateDate'] != NULL & $object_array['LastUpdateDate'] != ''){
    $object_node->setAttribute('LastUpdateDate', $object_array['LastUpdateDate']);
  }
  $object_node->appendChild($valid_dom->createElement('Title', $object_array['Title']['value']));
  $object_node->appendChild($valid_dom->createElement('ExternalReference', $object_array['ExternalReference']['value']));
  if (isset($object_array['InternalReference']) & $object_array['InternalReference'] != NULL & $object_array['InternalReference'] != ''){
    $object_node->appendChild($valid_dom->createElement('InternalReference', $object_array['InternalReference']['value']));
  }

  if (isset($object_array['Taxonomy']) & count($object_array['Taxonomy']) != 0){
    _wallytoolbox_arraytowallyxml_createVocItems($valid_dom, $object_node, $object_array['Taxonomy']);
  }

  if (isset($object_array['Copyright']) & $object_array['Copyright']['value'] != NULL & $object_array['Copyright']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('Copyright', $object_array['Copyright']['value']));
  }
  if (isset($object_array['ByLine']) & $object_array['ByLine']['value'] != NULL & $object_array['ByLine']['value'] != ''){
    $object_node->appendChild($valid_dom->createElement('ByLine', $object_array['ByLine']['value']));
  }
  if (isset($object_array['EmbeddedObjectsOrder']) & $object_array['EmbeddedObjectsOrder'] != NULL & $object_array['EmbeddedObjectsOrder'] != ''){
    $object_node->appendChild($valid_dom->createElement('EmbeddedObjectsOrder', $object_array['EmbeddedObjectsOrder']['value']));
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
  if (isset($object_array['Caption']) & $object_array['Caption']['value'] != NULL & $object_array['Caption']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('Caption',$object_array['Caption']['value']));

  $object_node->appendChild($valid_dom->createElement('FileURI',$object_array['FileURI']['value']));
  $object_node->appendChild($valid_dom->createElement('FileSize',$object_array['FileSize']['value']));
  if (isset($object_array['Thumbnail']) & $object_array['Thumbnail']['value'] != NULL & $object_array['Thumbnail']['value'] != ''){
    $thumbnail_node = $object_node->appendChild($valid_dom->createElement('Thumbnail'));
    $thumbnail_node->setAttribute('URI', $object_array['Thumbnail']['URI']);
    if (isset($object_array['Thumbnail']['Type']) & $object_array['Thumbnail']['Type'] != NULL & $object_array['Thumbnail']['Type'] != '')
      $thumbnail_node->setAttribute('Type',$object_array['Thumbnail']['Type']);
    if (isset($object_array['Thumbnail']['Title']) & $object_array['Thumbnail']['Title'] != NULL & $object_array['Thumbnail']['Title'] != '')
      $thumbnail_node->setAttribute('Title',$object_array['Thumbnail']['Title']);
    if (isset($object_array['Thumbnail']['W']) & $object_array['Thumbnail']['W'] != NULL & $object_array['Thumbnail']['W'] != '')
      $thumbnail_node->setAttribute('W',$object_array['Thumbnail']['W']);
    if (isset($object_array['Thumbnail']['H']) & $object_array['Thumbnail']['H'] != NULL & $object_array['Thumbnail']['H'] != '')
      $thumbnail_node->setAttribute('H',$object_array['Thumbnail']['H']);
    
  }
  if (isset($object_array['Rating']) & $object_array['Rating']['value'] != NULL & $object_array['Rating']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('Rating',$object_array['Rating']['value']));
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
  if (isset($object_array['Crops'])){
    $crops_node = $object_node->appendChild($valid_dom->createElement('Crops'));
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
  }
  if (isset($object_array['LinkTitle']) & $object_array['LinkTitle']['value'] != NULL & $object_array['LinkTitle']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('LinkTitle',$object_array['LinkTitle']['value']));
  if (isset($object_array['LinkURI']) & $object_array['LinkURI']['value'] != NULL & $object_array['LinkURI']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('LinkURI',$object_array['LinkURI']['value']));
  if (isset($object_array['Width']) & $object_array['Width']['value'] != NULL & $object_array['Width']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('Width',$object_array['Width']['value']));
  if (isset($object_array['Height']) & $object_array['Height']['value'] != NULL & $object_array['Height']['value'] != '')
    $object_node->appendChild('Height',$object_array['Height']['value']);
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
  if (isset($object_array['AudioLength']) & $object_array['AudioLength']['value'] != NULL & $object_array['AudioLength']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('AudioLength',$object_array['AudioLength']['value']));
  if (isset($object_array['LinkTitle']) & $object_array['LinkTitle']['value'] != NULL & $object_array['LinkTitle']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('LinkTitle',$object_array['LinkTitle']['value']));
  if (isset($object_array['LinkURI']) & $object_array['LinkURI']['value'] != NULL & $object_array['LinkURI']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('LinkURI',$object_array['LinkURI']['value']));
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
  if (isset($object_array['VideoLength']) & $object_array['VideoLength']['value'] != NULL & $object_array['VideoLength']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('VideoLength',$object_array['VideoLength']['value']));
  if (isset($object_array['LinkTitle']) & $object_array['LinkTitle']['value'] != NULL & $object_array['LinkTitle']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('LinkTitle',$object_array['LinkTitle']['value']));
  if (isset($object_array['LinkURI']) & $object_array['LinkURI']['value'] != NULL & $object_array['LinkURI']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('LinkURI',$object_array['LinkURI']['value']));
  
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
  if (isset($object_array['Summary']) & $object_array['Summary']['value'] != NULL & $object_array['Summary']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('Summary',$object_array['Summary']['value']));
  $object_node->appendChild($valid_dom->createElement('DateOpen',$object_array['DateOpen']['value']));
  if (isset($object_array['Summary']) & $object_array['DateClose']['value'] != NULL & $object_array['DateClose']['value'] != '')
    $object_node->appendChild($valid_dom->createElement('DateClose',$object_array['DateClose']['value']));
  $pollquestions_node = $object_node->appendChild($valid_dom->createElement('PollQuestions'));
  foreach ($object_array['PollQuestions']['PollQuestion'] as $pollquestion){
    $pollquestion_node = $pollquestions_node->appendChild($valid_dom->createElement('PollQuestion'));
    $pollquestion_node->appendChild($valid_dom->createElement('Question',$pollquestion['Question']['value']));
    if (isset($pollquestion['Description']) & $pollquestion['Description']['value'] != NULL & $pollquestion['Description']['value'] != '') 
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

  if (isset($term_array['PersonLastName']) & $term_array['PersonLastName']['value'] != NULL & $term_array['PersonLastName']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('PersonLastName',$term_array['PersonLastName']['value']));
  if (isset($term_array['PersonFirstName']) & $term_array['PersonFirstName']['value'] != NULL & $term_array['PersonFirstName']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('PersonFirstName',$term_array['PersonFirstName']['value']));
  if (isset($term_array['PersonNickName']) & $term_array['PersonNickName']['value'] != NULL & $term_array['PersonNickName']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('PersonNickName',$term_array['PersonNickName']['value']));
  if (isset($term_array['PersonBirthDate']) & $term_array['PersonBirthDate']['value'] != NULL & $term_array['PersonBirthDate']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('PersonBirthDate',$term_array['PersonBirthDate']['value']));
  if (isset($term_array['PersonSex']) & $term_array['PersonSex']['value'] != NULL & $term_array['PersonSex']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('PersonSex',$term_array['PersonSex']['value']));
  if (isset($term_array['PersonEmail']) & $term_array['PersonEmail']['value'] != NULL & $term_array['PersonEmail']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('PersonEmail',$term_array['PersonEmail']['value']));
  if (isset($term_array['PersonWebSite']) & $term_array['PersonWebSite']['value'] != NULL & $term_array['PersonWebSite']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('PersonWebSite',$term_array['PersonWebSite']['value']));
  if (isset($term_array['PersonPhoneNumber']) & $term_array['PersonPhoneNumber']['value'] != NULL & $term_array['PersonPhoneNumber']['value'] != NULL)
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

  if (isset($term_array['EntityDescription']) & $term_array['EntityDescription']['value'] != NULL & $term_array['EntityDescription']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('EntityDescription',$term_array['EntityDescription']['value']));
  if (isset($term_array['EntityServices']) & $term_array['EntityServices']['value'] != NULL & $term_array['EntityServices']['value'] != NULL)
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

  if (isset($term_array['Address'])){
    $address_node = $term_node->appendChild($valid_dom->createElement('Address'));
    foreach ($term_array['Address']['AddressLine'] as $addressline){
      $address_node->appendChild($valid_dom->createElement('AddressLine',$addressline));
    }
    if (isset($term_array['Address']['City']) & $term_array['Address']['City']['value'] != NULL & $term_array['Address']['City']['value'] != NULL)
      $address_node->appendChild($valid_dom->createElement('City',$term_array['Address']['City']));
    if (isset($term_array['Address']['Zip']) & $term_array['Address']['Zip']['value'] != NULL & $term_array['Address']['Zip']['value'] != NULL)
      $address_node->appendChild($valid_dom->createElement('Zip',$term_array['Address']['Zip']));
    if (isset($term_array['Address']['State']) & $term_array['Address']['State']['value'] != NULL & $term_array['Address']['State']['value'] != NULL)
      $address_node->appendChild($valid_dom->createElement('State',$term_array['Address']['State']));
    if (isset($term_array['Address']['Country']) & $term_array['Address']['Country']['value'] != NULL & $term_array['Address']['Country']['value'] != NULL)
      $address_node->appendChild($valid_dom->createElement('Country',$term_array['Address']['Country']));
    
  }
  if (isset($term_array['LocationGeo']) & $term_array['LocationGeo']['value'] != NULL & $term_array['LocationGeo']['value'] != NULL)
    $term_node->appendChild($valid_dom->createElement('LocationGeo',$term_array['LocationGeo']['value']));
 
}