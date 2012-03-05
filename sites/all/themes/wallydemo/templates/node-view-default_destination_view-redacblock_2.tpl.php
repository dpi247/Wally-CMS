<?php 


// Give the index of the row into the view.
//$row_index=$variables["view"]->row_index;
$row_index=$node->global_index;

// Le package -> $node 

/* Récupération de l'id du package -> $node_id
 * 
 */
$node_id = $node->nid;

/* Récupération de l'alias de l'url du package -> $node_path
 * 
 * print($node_path);
 */
$node_path = drupal_get_path_alias("node/".$node->nid);

/* Récupération du path vers notre theme -> $theme_path
 * 
 * print($theme_path);
 */

$theme_path = drupal_get_path('theme','wallydemo');

/* Récupération du mainstory et de la photo principale du package.
 * Le package peut être articlePackage ou galleryPackage
 * 
 * Objet principal du package -> $mainstory
 * 
 * Photo principale du package -> $photoObject
 * Path vers cette image -> $photoObject_path
 * Taille de la photo sur le serveur -> $photoObject_size
 * 
 * S'il y a bien une image à afficher -> $photo==TRUE
 * 
 * Pour l'affichage de la photo via son preset imagecache :
 * 
 * $photoObject_img = theme('imagecache', '???presetImagecache???', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], array('class'=>'postimage2')); ?>
 * print($photoObject_img);
 */
$photo = FALSE;
$photoObject_path = "";
if($node->type == "wally_articlepackage"){
  $mainstory = $node->field_mainstory_nodes[0];
} else {  
  $mainstory = $node->field_mainobject_nodes[0];
  $mainstory_type = $mainstory->type;
  if($mainstory_type == "wally_photoobject"){ 
    $photoObject_path = $mainstory->field_photofile[0]['filepath'];
    $photoObject_summary = $mainstory->field_summary[0]['value'];
    $photoObject_filename = $mainstory->field_photofile[0]["filename"];
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size == $mainstory->field_photofile[0]['filesize'];
    if (isset($photoObject_path) && $photoObject_size > 0) {
    	$photo = TRUE;
    }
  }
}

if ($photoObject_path == ""){
  $embeded_objects = $node->field_embededobjects_nodes;
  $photoObject = wallydemo_get_first_photoEmbededObject_from_package($embeded_objects);
  If ($photoObject) {
    $photoObject_path = $photoObject->field_photofile[0]['filepath'];
    $photoObject_summary = $photoObject->field_summary[0]['value'];
    $photoObject_filename = $photoObject->field_photofile[0]["filename"];
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
    if (isset($photoObject_path) && $photoObject_size > 0) {
      $photo = TRUE;
    }    
  }
}


/* Récupération du titre de l'object principal donc du package à l'affichage -> $title
 * 
 * print($title);
 */  
$title = $mainstory->title;

/*  Récupération de la date de publication du package -> $node_publi_date
 */
$node_publi_date = strtotime($node->field_publicationdate[0]['value']);

/* Affichage de la date au format souhaité
 * Les formats sont:
 * 
 * 'filinfo' -> '00h00'
 * 'unebis' -> 'jeudi 26 mai 2011, 15:54'
 * 'default' -> 'publié le 26/05 à 15h22'
 * 
 * print($date_edition);
 */ 
 
switch($row_index){
	case 0:
	case 1:
	case 6:
	case 7:
	case 12:
	case 13:
	case 18:
		$date_edition = _wallydemo_date_edition_diplay($node_publi_date, 'default');
	break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 8:
	case 9:
	case 10:
	case 11:
	case 14:
	case 15:
	case 16:
	case 17:
		$date_edition = _wallydemo_date_edition_diplay($node_publi_date, 'date_courte');
	break;	
}
/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */
$strapline_length = 0;
$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);

/* Récupération du nombre de réactions sur la package
 * 
 */
$nb_comment = $node->comment_count;
if ($nb_comment == 0) $reagir = "réagir";
else if ($nb_comment == 1) $reagir = $nb_comment."&nbsp;réaction";
else $reagir = $nb_comment."&nbsp;réactions";

/* Récupération des liens associés au package
 * 
 */

$links = _wallydemo_get_sorted_links($node);
$embeds = wallydemo_bracket_embeddedObjects_from_package($node);
$links_html = "";
if(count($links)>0 || count($embeds['videos'])>0 || count($embeds['audios'])>0 || count($embeds['digital'])>0){
	$links_html = "<ul>";
	if(count($embeds['videos'])>0 || count($embeds['audios'])>0 || count($embeds['digital'])>0){
	 if(count($embeds['videos']>0)){
	   foreach($embeds['videos'] as $video){
	   	$link_nid = $video->nid;
	   	$link_title = $video->title;
	   	$links_html .= "<li class=\"media-video\"><a href=\"".$node_path."#".$link_nid."\">".$link_title."</a></li>";
	   }
	 }
	 if(count($embeds['audios'])>0){
	    foreach($embeds['audios'] as $son){
    	 $link_nid = $son->nid;
		 $link_title = $son->title;
      	 $links_html .= "<li class=\"media-audio\"><a href=\"".$node_path."#".$link_nid."\">".$link_title."</a></li>";
	   }	   
	 }
	 if(count($embeds['digital'])>0){
    	foreach($embeds['digital'] as $dig){
      	 $link_nid = $dig->nid;
      	  $link_title = $dig->title; 
		  $type_digital = $embeds ['digital'][0] -> field_object3rdparty[0]['provider'];
			if($type_digital == "coveritlive"){
		      $links_html .= "<li class=\"media-live\"><a href=\"".$node_path."#".$link_nid."\">".$link_title."</a></li>";
				}
		    else {
      		  $links_html .= "<li class=\"arrow\"><a href=\"".$node_path."#".$link_nid."\">".$link_title."</a></li>";
	  		}
    	}    
   	}	 
	}

	if(count($links)>0){
		foreach($links as $linksList){
			if(isset($linksList["links"])){
				foreach($linksList["links"] as $link){
					$link_url = $link["url"];
					$link_title = $link["title"];
					$link_target = $link["target"];
					$link_type = $link["type"];
					if ($link["packagelayout"] == 'Article Wiki') {
						$links_html .= "<li class=\"media-dossier\">" ."<a class=\"novisited\" href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
					}
					else {
						$links_html .= "<li class=\"".$link_type ."\"><a href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
					}
				}
			}
		}
	}
  $links_html .= "</ul>";
}
/*
 * Génération du breadcrumb
 */
$breadcrumb = _wallydemo_breadcrumb_display($node->field_destinations[0]["tid"],'une');
?>

<?php 

switch ($row_index) {
	case 0:
?>
<div class="article gd clearfix">
  <p class="time time_une"><?php print $date_edition; ?></p>
  <span class="ariane_une"><?php print $breadcrumb; ?></span>
  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
  <a href="/<?php print check_url($node_path); ?>">
  <?php if($photo == TRUE){ 
  $photoObject_img = theme('imagecache', 'une_manchette_217x145', $photoObject_path, $photoObject_summary, $photoObject_summary);
  			} 
		else { 
  $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"217\" height=\"145\" />";
  			} 
  print $photoObject_img;
		?>
  </a>

  <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
  <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print $reagir; ?></a></p>
  <?php if($links_html != ""){
  print $links_html;
  } ?>
</div>

<?php 
	break;
	case 1:
	case 7:
	case 13:
?>
<div class="article md clearfix">
  <p class="time time_une"><?php print $date_edition; ?></p>
  <span class="ariane_une"><?php print $breadcrumb; ?></span>
  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
  <a href="/<?php print check_url($node_path); ?>">
  <?php if($photo == TRUE){ 
  $photoObject_img = theme('imagecache', 'une_medium_127x85', $photoObject_path, $photoObject_summary, $photoObject_summary);
  			} 
		else { 
  $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"127\" height=\"85\" />";
  			} 
  print $photoObject_img; 
		?>
  </a>

  <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
  <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print $reagir; ?></a></p>
  <?php if($links_html != ""){
  print $links_html;
  } ?>
</div>
<?php 
	break;
	case 6:
	case 12:
	case 18:
?>
<div class="article md clearfix noborder">
  <p class="time time_une"><?php print $date_edition; ?></p>
  <span class="ariane_une"><?php print $breadcrumb; ?></span>
  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
  <a href="/<?php print check_url($node_path); ?>">
  <?php if($photo == TRUE){ 
  $photoObject_img = theme('imagecache', 'une_medium_127x85', $photoObject_path, $photoObject_summary, $photoObject_summary);
  			} 
		else { 
  $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"127\" height=\"85\" />";
  			} 
  print $photoObject_img; 
		?>
  </a>

  <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
  <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print $reagir; ?></a></p>
  <?php if($links_html != ""){
  print $links_html;
  } ?>
</div>

    <?php 
	break;
	case 2:
	case 8:
	case 14:
?>
<div class="wrap-columns clearfix">
  <div class="intern-col first column limit">
    <div class="article md clearfix">
      <p class="time time_une"><?php print $date_edition; ?></p>
      <span class="ariane_une"><?php print $breadcrumb; ?></span>
		  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
          <a href="/<?php print check_url($node_path); ?>">
          <?php if($photo == TRUE){ 
          $photoObject_img = theme('imagecache', 'une_small_78x52', $photoObject_path, $photoObject_summary, $photoObject_summary);
                    } 
                else { 
          $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"78\" height=\"52\" />";
                    } 
          print $photoObject_img; 
                ?>
          </a>
          
		  <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
		  <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print $reagir; ?></a></p>
		  <?php if($links_html != ""){
			print $links_html;
			} ?>
    </div>
    
	<?php 
	break;
	case 3:
	case 9:
	case 15:
?>
    <div class="article md clearfix noborder">
      <p class="time time_une"><?php print $date_edition; ?></p>
      <span class="ariane_une"><?php print $breadcrumb; ?></span>
		  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
          <a href="/<?php print check_url($node_path); ?>">
          <?php if($photo == TRUE){ 
          $photoObject_img = theme('imagecache', 'une_small_78x52', $photoObject_path, $photoObject_summary, $photoObject_summary);
                    } 
                else { 
          $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"78\" height=\"52\" />";
                    } 
          print $photoObject_img; 
                ?>
          </a>
          
      <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
      <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print $reagir; ?></a></p>
      <?php if($links_html != ""){
      print $links_html;
      } ?>
    </div>
  </div>
    <?php 
	break;
	case 4:
	case 10:
	case 16:
?>
  <div class="intern-col last column">
    <div class="article md clearfix">
      <p class="time time_une"><?php print $date_edition; ?></p>
      <span class="ariane_une"><?php print $breadcrumb; ?></span>
		  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
          <a href="/<?php print check_url($node_path); ?>">
          <?php if($photo == TRUE){ 
          $photoObject_img = theme('imagecache', 'une_small_78x52', $photoObject_path, $photoObject_summary, $photoObject_summary);
                    } 
                else { 
          $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"78\" height=\"52\" />";
                    } 
          print $photoObject_img; 
                ?>
          </a>
          
      <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
      <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print $reagir; ?></a></p>
      <?php if($links_html != ""){
      print $links_html;
      } ?>
    </div>
    <?php 
	break;
	case 5:
	case 11:
	case 17:
?>
    <div class="article md clearfix noborder">
      <p class="time time_une"><?php print $date_edition; ?></p>
      <span class="ariane_une"><?php print $breadcrumb; ?></span>
		  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
          <a href="/<?php print check_url($node_path); ?>">
          <?php if($photo == TRUE){ 
          $photoObject_img = theme('imagecache', 'une_small_78x52', $photoObject_path, $photoObject_summary, $photoObject_summary);
                    } 
                else { 
          $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"78\" height=\"52\" />";
                    } 
          print $photoObject_img; 
                ?>
          </a>
        
      <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
      <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print $reagir; ?></a></p>
      <?php if($links_html != ""){
      print $links_html;
      } ?>
    </div>
  </div>
</div>

<?php 
	break;
/*	default:
	?>
<?php*/ 
}
//ici unset des variables
unset($node);
unset($mainstory);
unset($photoObject);
unset($links);
unset($embeds);
unset($embeds_audios);
unset($embeds_videos);
unset($embeds_photos);
unset($embeds_digital);
?>
