<?php 
  require_once 'includes/constants.inc';

	$title=t('Connection');
	if(isset($_GET["err"])){
		$title=$_GET["msg"];
	}
?>
<div class="p-main-content">
	<div class="p-wrap">
		<div class="p-the-content" style="padding-top:20px;">
			<div id="divtochange">
				<div id="loading">&nbsp;</div> 
				<form name="mydata" id="mydata" action="" method="post" onsubmit='return coPr(document.getElementById("memail").value, document.getElementById("mmdp").value);'>
				<input type="hidden" name="form" value="done" /> 
				<div class="row">
					<label for="memail"><span>*</span> <?php echo t('E-mail') ?> :</label>
					<input type="text" class="form-text" name="memail" id="memail" />
				</div>
				<div class="row">
					<label for="mmdp"><span>*</span> <?php echo t('Password') ?> :</label>
					<input type="password" class="form-text" name="mmdp" id="mmdp" />
				</div>
				<input type="submit" class="btn-submit" value="<?php echo $title;?>" />
				</form><div class="row" id="fgpwd"><a href="<?php echo SSO_FORGET; ?>?aff=FULL&amp;backurlPDF='+encodeURIComponent(window.location.href)+'"><?php t('Password forgotten?'); ?></a></div>
			</div>
		</div>
		<div class="p-bottom">&nbsp;</div>
	</div>
</div>
<iframe id="ssol" style="display: none;">
</iframe>