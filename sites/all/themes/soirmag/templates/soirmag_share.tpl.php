<?php
    $path = base_path() . path_to_theme();
    $s_title = $story->title;
    $s_path =  $_SERVER["SERVER_NAME"].url("node/".$story->nid);
?>
<p>Partager cet article sur :</p>
  <ul>
      <li><a title="sur Facebook" href="http://www.facebook.com/share.php?u=<?php print $s_path; ?>"><img src="<?php print $path; ?>/mediastore/elements/icons/facebook.gif" alt="Facebook" width="16" height="16" /></a></li>
			<li><a title="sur Twitter" href="http://twitter.com/home?status=<?php print $s_title; ?> - <?php print $s_path; ?>"><img src="<?php print $path; ?>/mediastore/elements/icons/twitter.gif" alt="Twitter" width="16" height="16" /></a></li>
			<li><a title="sur Del.icio.us" href="http://del.icio.us/post?url=<?php print $s_path; ?>"><img src="<?php print $path; ?>/mediastore/elements/icons/delicious.gif" alt="Delicious" width="16" height="16" /></a></li>
			<li><a title="sur Digg" href="http://www.digg.com/submit?phase=2&amp;url=<?php print $s_path; ?>"><img src="<?php print $path; ?>/mediastore/elements/icons/digg.gif" alt="Digg" width="16" height="16" /></a></li>
			<li><a title="sur StumbleUpon" href="http://www.stumbleupon.com/submit?url=<?php print $s_path; ?>"><img src="<?php print $path; ?>/mediastore/elements/icons/stumble.gif" alt="StumbleUpon" width="16" height="16" /></a></li>
			<li><a title="sur Tumblr" href="http://www.tumblr.com/share?s=<?php print $s_path; ?>&amp;t=<?php print $s_title; ?>"><img src="<?php print $path; ?>/mediastore/elements/icons/tumblr.gif" alt="Tumblr" width="16" height="16" /></a></li>
  </ul>
<p>par mail :</p>
  <ul>
    <li><a class="par-mail" title="Envoyer cette page par E-mail" href="javascript:void(0)"><img src="<?php print $path; ?>/mediastore/elements/icons/email.gif" alt="E-mail" width="16" height="16" /></a></li>
  </ul>
<p>ou imprimez-le :</p>
  <ul>
    <li><a href="javascript:print();"><img src="<?php print $path; ?>/mediastore/elements/icons/printer.gif" alt="imprimer" width="10" height="10" /></a></li>
  </ul>
