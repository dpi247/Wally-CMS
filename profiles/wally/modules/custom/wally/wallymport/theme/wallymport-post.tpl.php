<!DOCTYPE html>
<html lang="en">
<head>
<link rel="stylesheet" type="text/css" href="/<?php print drupal_get_path('module','wallytoolbox').'/twitter_bootstrap/bootstrap/css/bootstrap.css'?>" />
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
<script src="/<?php print drupal_get_path('module','wallytoolbox').'/twitter_bootstrap/bootstrap/js/bootstrap.min.js'?>"></script>
<style type="text/css">

body {margin:1em;}
#preview-tabs{margin-bottom:0}
.tab-content{
min-height:500px;
padding:10px;
border-bottom: 1px solid #DDD;
border-left: 1px solid #DDD;
border-right: 1px solid #DDD;
}
</style>
<script>
$(document).ready(function() {
	$('#wallymport-preview-form').addClass('form-inline');
	$('select').css('width','auto');
	});
  $('#preview-tabs a').click(function (e) {
    e.preventDefault();
    $(this).tab('show');
  })
</script>
<script>

</script>
</head>
<body>
<?php 
if(isset($return['error']) and $return['error']){
  $error=true;
  
}
if($error){
  $icon='<i class="icon-remove"></i>';
  
}
else{
  $icon='<i class="icon-ok"></i>';
  
}

//icon is useless
$icon="";
?>

<!-- 
<script>

$("div.wallymport_result ul").hide();
$("div#wallyedit_preview h1").hide();
$("div.wallymport_result .logs").click(function(){
$("div.wallymport_result ul").toggle();
});
</script>
 -->
<div id="wallimport-post">
  <div id="tab_container">
    <ul id="preview-tabs" class="nav nav-tabs">
      <li class="active"><a href="#preview" data-toggle="tab">Preview</a></li>
      <li><a href="#profile" data-toggle="tab">Logs</a></li>
            <li class="disabled pull-right"><a><?php print date("d-m-Y h:i").$icon;?></a></li>
    </ul>
    <div class="tab-content">
      <div class="tab-pane active" id="preview">  
      <?php if($error):?>
          <div class="alert alert-error">
           <p><strong><?php print t('An error occured during the preview process...')?></strong></p>
           <dl class="dl-horizontal">
             <dt><?php print t('Error message')?></dt>
             <dd><?php print $return['error_msg'] ?></dd>
           </dl>
           
           <p>
             <?php print t('Please ensure the followings');?>:
             <ul>
               <li><?php print t('The content type of your package is correctly set (e.g: Article, Poll, Gallery).');?></li>
               <li><?php print t('Your package contains at least one existing destination.');?></li>
               <li><?php print t('Your package has a title.');?></li>
               <li><?php print t('Your package has a text.');?></li>
               <li><?php print t('If your package is a poll then the start&stop time and the mutiplicity of the poll are defined.');?></li>
               </ul>
           </p>
           <p>
           
           <?php print t('You can also check the logs for more informations.')?>
           </p>
          </div>
        <?php else:?>
          <?php print $return['content']?>
        <?php endif;?>
      </div>
      <div class="tab-pane" id="profile">

        <div class="accordion" id="accordion2">
		  <div class="accordion-group">
		    <div class="accordion-heading">
			  <a class="accordion-toggle" data-toggle="collapse" data-parent="#accordion2" href="#collapseOne">  Drupal messages </a>
			</div>
			<div id="collapseOne" class="accordion-body collapse ">
			  <div class="accordion-inner">
                <?php   print theme('status_messages')?>
			  </div>
		    </div>
		  </div>
		  <div class="accordion-group">
		    <div class="accordion-heading">
			  <a class="accordion-toggle" data-toggle="collapse" data-parent="#accordion2" href="#collapseTwo">  Import Process </a>
			</div>
		  <div id="collapseTwo" class="accordion-body collapse ">
		    <div class="accordion-inner">
			  <?php print theme('wallymport_logs', $return);?>
			</div>
		  </div>
        </div>
        <!-- 
		<div class="accordion-group">
		  <div class="accordion-heading">
		    <a class="accordion-toggle" data-toggle="collapse" data-parent="#accordion2" href="#collapseThree"> Time </a>
		  </div>
		  <div id="collapseThree" class="accordion-body collapse ">
		    <div class="accordion-inner">
			  KAiro kairo
			</div>
		  </div>
	    </div>
	     -->
	  </div>
    </div>
  </div>
</div>
  
</div>
</body>
</html>
