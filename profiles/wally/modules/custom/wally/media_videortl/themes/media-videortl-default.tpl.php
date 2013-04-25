<?php 
	$video_id = $variables["variables"]["video_id"]; 
	$key = $variables["variables"]["key"];
	$domain = $variables["variables"]["domain"];
?>
<iframe frameborder="0" width="600" height="338" src="http://www.rtl.be/<?php print $domain ?>/page/rtl-video-en-embed/640.aspx?VideoID=<?php print $video_id ?>&key=<?php print $key ?>"></iframe>
