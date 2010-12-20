<!DOCTYPE HTML PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <?php print $head ?>
  
  <title><?php print $head_title ?></title>
  <?php print $styles ?>
  <?php print $scripts ?>
  <!--[if lt IE 7]>
    <?php print wallynews_get_ie_styles(); ?>
  <![endif]-->
</head>
<body>
<div class="outerwrap">
  <div class="wrapper">
    <div class="header">
            <h1><a href="\">Wally News</a></h1>

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
                <? print theme("wallyct_coucou"); ?>
      
        <?php if (isset($primary_links)) : ?>
          <?php print $sf_primarymenu; ?> 
          <? // print theme("wallyct_mainmenu", 'primary-links', 'menu-primary-links'); ?>
        <?php endif; ?>
      </div>
    </div>
    <!-- end of access -->

    <div class="clear"></div>

    <div class="access2">
      <div class="menu2">
        <?php if (isset($secondary_links)) : ?>
          <?php print $sf_secondarymenu;  ?> 
          <? // print theme("wallyct_mainmenu", 'secondary_links', 'menu-secondary-links'); ?>
        <?php endif; ?>
      </div>
    </div>
    <!-- end of access2 -->

    <div class="clear"></div>        
    

    <?php if ($messages): print $messages; endif; ?>
    <?php if ($help): print $help; endif; ?>


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
