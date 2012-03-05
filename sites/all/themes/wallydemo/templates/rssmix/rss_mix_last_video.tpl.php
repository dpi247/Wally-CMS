<?php

$class = "last-vids";
$target = "http://videos.sudpresse.be/top/?filter=2&period=3";
$title = "Nos dernières vidéos";
// détermine la taille du feed.
$last = count($feed)-1;

// bouclesur tous les elemnts du feed.
foreach ($feed as $k=>$item) {

  if ($k==$last) { 
    $classlast = " no"; } else { $classlast = ""; 
  }

  $item['titre'] = $item['MainStory']['Title']['value'];
  //dsm($item['EmbeddedContent']);
  $item['image_path'] = $item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['Thumbnail']['LocaleImage']['filepath'];

  if ($item['image_path'] != "") {
    $item['img'] = theme('imagecache', 'divers_68x45', $item['image_path']); 
  } else {
    $item['img'] = "<img src=\"/sites/all/themes/wallydemo/images/default_pic_last-vids.gif\">"; 
  }
  $content .= "<li class=\"item clearfix".$classlast."\">" ."<a href='".$item['ExternalURI']['value']."' target='_blank'>" .$item['img'] ."</a>" ."<h3><a href='".$item['ExternalURI']['value']."' target='_blank'>" .$item['titre'] ."</h3></a></li>";
  unset($item);
}
unset($feed);
// transformation d'une image "source" en thumbnail via image cache.
// theme('imagecache', 'divers_78x60', $item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath']);

?>

<div class="bloc-01 <?php print $class ; ?>">
  <h2><a target="_blank" href="<?php print $target; ?>"><?php print $title ; ?></a></h2>
  <ul class="clearfix">
    <?php print $content; ?>
    <li class="inner-bloc">
      <ul>
        <li><a href="http://videos.sudpresse.be/top/" target="_blank" class="last-vids novisited">Le top vidéos</a></li>
        <li><a href="http://videos.sudpresse.be/" target="_blank" class="last-vids novisited">Toutes les vidéos</a></li>
      </ul>
    </li>
  </ul>
</div>
