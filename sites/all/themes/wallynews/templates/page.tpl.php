<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

  <?php print $head ?>
  
  <title><?php print $head_title ?></title>
  <?php print $styles ?>
  <?php print $scripts ?>
  <!--[if lt IE 7]>
    <?php print phptemplate_get_ie_styles(); ?>
  <![endif]-->
</head>
<body>
<div class="outerwrap">
  <div class="wrapper">
    <div class="header">
            <h1><a href="#">Wally News</a></h1>

            <h2>### MISSION ###</h2>
            
            <div class="bannertop">
                <a href="#">
                  <img src="#" alt="#" border="0">
                </a>       
            </div>
                    
            <div class="toplinks">
              <a href="#">Login</a> | <a href="#"> Contact</a>
            </div>
            
            <div class="todaydate">### today date ###</div>

            <div class="topsearch">
                <form action="#" method="get">
                <input value="Search..." name="s" id="ls" class="searchfield" onfocus="if (this.value == 'Search...') {this.value = '';}" onblur="if (this.value == '') {this.value = 'Search...';}" type="text">
                <input src="mediastore/search.png" value="submit" style="vertical-align: middle;" type="image">
                </form>
            </div>
            <!--topsearch -->
    </div>
    <!--header--> 
  </div>
  <!--wrapper --> 

  <div class="wrapper">

    <div class="access">
      <div class="menu">
        <?php if (isset($primary_links)) : ?>
          <? print theme("wallyct_mainmenu", 'primary-links', 'menu-primary-links'); ?>
        <?php endif; ?>
      </div>
    </div>
    <!-- end of access -->

    <div class="clear"></div>

    <div class="access2">
      <div class="menu2">
        <?php if (isset($secondary_links)) : ?>
          <?  //  print theme("wallyct_mainmenu", 'secondary_links'); ?>
        <?php endif; ?>
      </div>
    </div>
    <!-- end of access2 -->

    <div class="clear"></div>        
    
    <div id="ticker-wrapper" class="has-js">
      <ul id="js-news" class="js-hidden">      
        <li class="news-item"><span style="color: rgb(153, 153, 153); font-size: 11px;">10:30 pm » </span><a href="http://bignews.blogohblog.net/2010/11/global-crisis-overshadows-obamas-economic-message/" rel="bookmark" title="Permanent Link to Global crisis hides Obama’s message">Global crisis hides Obama’s message</a></li>                   
        <li class="news-item"><span style="color: rgb(153, 153, 153); font-size: 11px;">10:26 pm » </span><a href="http://bignews.blogohblog.net/2010/11/us-briefs-allies-about-next-wikileaks-release/" rel="bookmark" title="Permanent Link to US briefs allies about next WikiLeaks release">US briefs allies about next WikiLeaks release</a></li>                   
        <li class="news-item"><span style="color: rgb(153, 153, 153); font-size: 11px;">10:24 pm » </span><a href="http://bignews.blogohblog.net/2010/11/brazil-braces-for-more-violence/" rel="bookmark" title="Permanent Link to Brazil Braces for more violence">Brazil Braces for more violence</a></li>                   
        <li class="news-item"><span style="color: rgb(153, 153, 153); font-size: 11px;">10:21 pm » </span><a href="http://bignews.blogohblog.net/2010/11/will-iraq-finally-get-its-weapons-of-mass-destruction/" rel="bookmark" title="Permanent Link to Will Iraq get Weapons of Mass Destruction?">Will Iraq get Weapons of Mass Destruction?</a></li>                   
        <li class="news-item"><span style="color: rgb(153, 153, 153); font-size: 11px;">9:53 pm » </span><a href="http://bignews.blogohblog.net/2010/11/north-korea-accuses-south-of-using-human-shields/" rel="bookmark" title="Permanent Link to North Korea accuses South Korea">North Korea accuses South Korea</a></li>                   
    </ul>
    <div id="ticker"><div id="ticker-title"><span style="display: inline;">Today : </span></div><p style="display: block; left: 69px;" id="ticker-content"><span style="color: rgb(153, 153, 153); font-size: 11px;">10:30 pm » </span><a href="http://bignews.blogohblog.net/2010/11/global-crisis-overshadows-obamas-economic-message/" rel="bookmark" title="Permanent Link to Global crisis hides Obama’s message">Global crisis hides Obama’s message</a></p><div style="left: 69px; margin-left: 275px; display: none;" id="ticker-swipe"><span style="display: block;"><!-- --></span></div></div><ul id="ticker-controls"><li id="play-pause" class="controls"></li><li id="prev" class="controls"></li><li id="next" class="controls"></li></ul></div>

    <div id="container">
      <?php print $content ?>
    </div><!--container-->

    <div class="footer">
      <div class="copyright">Copyright © #### <a href="#">Wally News</a></div>
      <div class="designby">Powered by <a href="http://www.drupal.org/">Drupal</a> | Wally News Theme by <a href="#" title="#">Exxodus</a></div>
      <div class="clear"></div>
    </div>

  </div><!--wrapper-->
</div>
<div title="Scroll Back to Top" style="position: fixed; bottom: 5px; right: 5px; opacity: 0; cursor: pointer;" id="topcontrol"><div class="topper"></div></div>
</body>
</html>
