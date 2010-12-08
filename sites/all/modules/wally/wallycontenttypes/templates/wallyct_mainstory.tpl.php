<?
/**
 * Wally default template for rendering Packages Autors List 
 * 
 *
 */
     
    // Barette
    $textarette = "<span class='barette'>".$mainstory->field_textarette[0]["value"]."</span>";
    
    // Foretitle
    $foretitle = $textarette." ".$mainstory->field_textforetitle[0]["value"];

    // Title
    $title = $mainstory->title;

    // Subtitle
    $subtitle = $mainstory->field_textsubtitle[0]["value"];
    
    // Body
    $body = $mainstory->field_textbody[0]["value"];
      
    // Authors
    $authors = "<fieldset id='mainstoryauthors'><h3>Authors</h3>".theme("wallyct_personslist",$mainstory->field_authors_nodes, $mainstory)."</fieldset>";

    // Persons
    //$persons = "<fieldset id='mainstoryauthors'><h3>Persons</h3>".theme("wallyct_personslist",$mainstory->field_persons_nodes, $mainstory)."</fieldset>";
    $persons = "<fieldset id='mainstoryauthors'><h3>Persons</h3>".theme("wallyct_personslist_detail",$mainstory->field_persons_nodes, $mainstory)."</fieldset>";

    // copyright
    $copyright = "<span class='copyright'>".$mainstory->field_copyright[0]["value"]."</span>";

    // Photo Gallery Slide Show
    $photo_gallery = theme("wallyct_photoobject_slider",$node->field_embededobjects_nodes, $node);
    

    $content = "<div style='width:70%;float:left'>".$maincol."</div><div style='width:30%;float:right'>".$leftcol."</div>";
 
?>
<h3><?php print $foretitle; ?></h3>             
<h2><a title="" rel="bookmark" href=""><?php print $title; ?></a></h2>             
<h3><?php print $subtitle; ?></h3>             
             
<div class="date">Published on ### ## #####<span> // <a rel="category tag" title="View all posts in Featured" href="http://bignews.blogohblog.net/category/featured/">Featured</a>, <a rel="category tag" title="View all posts in Technology" href="http://bignews.blogohblog.net/category/technology/">Technology</a></span></div>        

<div class="content">
   
<?php  print $photo_gallery; ?>

<!-- <p><a href="http://bignews.blogohblog.net/wp-content/uploads/2010/11/First_Xbox_buyer.jpg"><img width="300" height="200" alt="" src="http://bignews.blogohblog.net/wp-content/uploads/2010/11/First_Xbox_buyer-300x200.jpg" title="First_Xbox_buyer" class="alignleft size-medium wp-image-75"></a>  -->

<?php print $body; ?>
                        
                           <div class="socialicons">
									
			<a title="Share this post on Delicious" href="http://del.icio.us/post?url=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/&amp;title=After 5 years, Xbox 360 still a big winner"><img alt="Share this post on Delicious" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/delicious.png"></a>
			<a title="StumbleUpon this post" href="http://www.stumbleupon.com/submit?url=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/&amp;title=After 5 years, Xbox 360 still a big winner"><img alt="StumbleUpon this post" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/stumble.png"></a>
			<a title="Share this post on Digg" href="http://digg.com/submit?phase=2?url=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/&amp;title=After 5 years, Xbox 360 still a big winner"><img alt="Share this post on Digg" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/digg.png"></a>
			<a title="Tweet about this post" href="http://twitthis.com/twit?url=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/"><img alt="Tweet about this post" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/twitter.png"></a>
			<a title="Share this post on Mixx" href="http://www.mix.com/submit?page_url=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/&amp;title=After 5 years, Xbox 360 still a big winner"><img alt="Share this post on Mixx" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/mixx.png"></a>
			<a title="Share this post on Technorati" href="http://technorati.com/faves?add=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/"><img alt="Share this post on Technorati" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/technorati.png"></a>
			<a title="Share this post on Facebook" href="http://www.facebook.com/sharer.php?u=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/&amp;t=After 5 years, Xbox 360 still a big winner"><img alt="Share this post on Facebook" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/facebook.png"></a>
			<a title="Share this post on ShareThis" href="http://newsvine.com/_tools/seed&amp;save?u=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/&amp;h=After 5 years, Xbox 360 still a big winner"><img alt="Share this post on NewsVine" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/newsvine.png"></a>
			<a title="Share this post on Reddit" href="http://reddit.com/submit?url=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/&amp;title=After 5 years, Xbox 360 still a big winner"><img alt="Share this post on Reddit" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/reddit.png"></a>
				<a title="Share this post on LinkedIn" href="http://linkedin.com/shareArticle?mini=true&amp;url=http://bignews.blogohblog.net/2010/11/after-5-years-xbox-360-still-a-big-winner/&amp;title=After 5 years, Xbox 360 still a big winner"><img alt="Share this post on LinkedIn" src="http://bignews.blogohblog.net/wp-content/themes/bignews/images/linkedin.png"></a>
			
		
		</div><!--socialicons -->
                                 </div>
                  
                       
                        <div class="tags"><a rel="tag" href="http://bignews.blogohblog.net/tag/gaming/">Gaming</a> <a rel="tag" href="http://bignews.blogohblog.net/tag/tech/">Tech</a></div>         
                        <div class="clear"></div>
                        
						                        <div id="commentsdiv">
                        <!-- You can start editing here. -->
<h1></h1><a href="#" class="box-toggle-show">+ Discussion</a>
<div class="comments-box" style="display: none;"> <a id="comments" name="comments"></a>
      <!-- If comments are open, but there are no comments. -->
        <div id="respond">
    <h3><cufon class="cufon cufon-canvas" alt="Leave " style="width: 50px; height: 24px;"><canvas width="77" height="27" style="width: 77px; height: 27px; top: -5px; left: -3px;"></canvas><cufontext>Leave </cufontext></cufon><cufon class="cufon cufon-canvas" alt="a " style="width: 14px; height: 24px;"><canvas width="41" height="27" style="width: 41px; height: 27px; top: -5px; left: -3px;"></canvas><cufontext>a </cufontext></cufon><cufon class="cufon cufon-canvas" alt="comment" style="width: 75px; height: 24px;"><canvas width="97" height="27" style="width: 97px; height: 27px; top: -5px; left: -3px;"></canvas><cufontext>comment</cufontext></cufon></h3>
    <div class="cancel-comment-reply"> <small>
      <a style="display: none;" href="/2010/11/after-5-years-xbox-360-still-a-big-winner/#respond" id="cancel-comment-reply-link" rel="nofollow">Click here to cancel reply.</a>      </small> </div>
        <p>You must be <a href="http://bignews.blogohblog.net/wp-login.php?redirect_to=http%3A%2F%2Fbignews.blogohblog.net%2F2010%2F11%2Fafter-5-years-xbox-360-still-a-big-winner%2F">Logged in</a> to post comment.</p>
      </div>
  </div>
                        </div>
    
