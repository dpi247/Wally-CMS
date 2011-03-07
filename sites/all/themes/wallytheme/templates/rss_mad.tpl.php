<div id="mad-bloc" class="brique">
					<h2><img src="/sites/all/themes/custom_soirmag/mediastore/elements/logo-mad.gif" width="58" height="20" alt="Le Mad" /></h2>
					<ul>

<?php



foreach ($feed as $k=>$item) {
	$img_path = imagecache_create_url('sm_mad_thumb', $item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'] );
	$link = $item['Package']['ExternalURI']['value'];
	$info = $item['Package']['MainStory']["TextBody"]["value"];
	$title = $item['Package']['MainStory']['Title']['value'];
 $content .= "<li><h3><a href=\"".$link."\">scenes</a></h3>";
 $content .= "<div><a href='".$link."'><img src=\"".$img_path."\" alt=\"".check_plain($title)."\" width=\"70\" height=\"70\" /></a>";
 $content .= "<h4><a href='".$link."'>".check_plain($title)."</a></h4>";
 $content .= "<p class=\"info\">".$info."</p></div></li>"; 
}
echo $content;
?>

					</ul>
</div>