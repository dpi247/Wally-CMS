<?php
    $path = base_path() . path_to_theme();
    $s_title = $node->title;
    $s_path =  $_SERVER["SERVER_NAME"].url("node/".$node->nid);
?>

<div class="socialicons">
  <a href="http://del.icio.us/post?url=<?php print $s_path; ?>&amp;title=<?php print $s_title; ?>" title="Share this post on Delicious"><img src="<?php print $path; ?>/images/social/delicious.png" alt="Share this post on Delicious"></a>
  <a href="http://www.stumbleupon.com/submit?url=<?php print $s_path; ?>&amp;title=<?php print $s_title; ?>" title="StumbleUpon this post"><img src="<?php print $path; ?>/images/social/stumble.png" alt="StumbleUpon this post"></a>
  <a href="http://digg.com/submit?phase=2?url=<?php print $s_path; ?>&amp;title=<?php print $s_title; ?>" title="Share this post on Digg"><img src="<?php print $path; ?>/images/social/digg.png" alt="Share this post on Digg"></a>
  <a href="http://twitthis.com/twit?url=<?php print $s_path; ?>" title="Tweet about this post"><img src="<?php print $path; ?>/images/social/twitter.png" alt="Tweet about this post"></a>
  <a href="http://www.mix.com/submit?page_url=<?php print $s_path; ?>&amp;title=<?php print $s_title; ?>" title="Share this post on Mixx"><img src="<?php print $path; ?>/images/social/mixx.png" alt="Share this post on Mixx"></a>
  <a href="http://technorati.com/faves?add=<?php print $s_path; ?>" title="Share this post on Technorati"><img src="<?php print $path; ?>/images/social/technorati.png" alt="Share this post on Technorati"></a>
  <a href="http://www.facebook.com/sharer.php?u=<?php print $s_path; ?>&amp;t=<?php print $s_title; ?>" title="Share this post on Facebook"><img src="<?php print $path; ?>/images/social/facebook.png" alt="Share this post on Facebook"></a>
  <a href="http://newsvine.com/_tools/seed&amp;save?u=<?php print $s_path; ?>&amp;h=<?php print $s_title; ?>" title="Share this post on ShareThis"><img src="<?php print $path; ?>/images/social/newsvine.png" alt="Share this post on NewsVine"></a>
  <a href="http://reddit.com/submit?url=<?php print $s_path; ?>&amp;title=<?php print $s_title; ?>" title="Share this post on Reddit"><img src="<?php print $path; ?>/images/social/reddit.png" alt="Share this post on Reddit"></a>
  <a href="http://linkedin.com/shareArticle?mini=true&amp;url=<?php print $s_path; ?>&amp;title=<?php print $s_title; ?>" title="Share this post on LinkedIn"><img src="<?php print $path; ?>/images/social/linkedin.png" alt="Share this post on LinkedIn"></a>
</div>
