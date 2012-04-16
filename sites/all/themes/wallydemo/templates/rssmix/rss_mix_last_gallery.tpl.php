<?php 
$class = "galerie";
$target = "http://portfolio.sudpresse.be/main.php";
$title = "Nos galeries photos";

// détermine la taille du feed.
$last = count($feed)-1;

// bouclesur tous les elemnts du feed.
foreach ($feed as $k=>$item) {

  if ($k==$first) { 
    $classlast = " first"; 
  } else if ($k==$last) { 
    $classlast = " last"; 
  } else { 
    $classlast = ""; 
  }

  $item['titre'] = $item['MainStory']['Title']['value'];
  $item['image_path'] = $item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'];
  if ($item['image_path'] != "") {
    $item['img'] = theme('imagecache', 'une_small_78x52', $item['image_path'],$item['titre'],$item['titre']); 
  } else {
    $item['img'] = "<img src=\"/sites/all/themes/custom_sp/images/default_pic_last-vids.gif\">"; 
  }
  $content .= "<li class=\"".$classlast."\">" ."<a href='".$item['ExternalURI']['value']."' target='_blank'>" .$item['img'] ."</a>" ."<h3><a href='".$item['ExternalURI']['value']."' target='_blank'>" .$item['titre'] ."</h3></a></li>";
  unset($item);
}
unset($feed);
// transformation d'une image "source" en thumbnail via image cache.
// theme('imagecache', 'divers_78x60', $item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath']);
?>

<div class="bloc-01 <?php print $class ; ?>">
  <h2><a target="_blank" href="<?php print $target; ?>"><?php print $title ; ?></a></h2>
  <ul class="clearfix gal-inner">
    <?php print $content; ?>
  </ul>
    <div class="inner-bloc">
      <ul>
        <li><a href="http://portfolio.sudpresse.be/main.php" target="_blank" class="novisited">Toutes les galeries</a></li>
      </ul>
    </div>
</div>
