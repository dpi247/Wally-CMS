<?php 
	$title="Je me connecte";
	$butt="se connecter";
	if(isset($_GET["err"])){
		$title=$_GET["msg"];
		$butt="je r�essaie";
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
					<label for="memail"><span>*</span> E-mail :</label>
					<input type="text" class="form-text" name="memail" id="memail" />
				</div>
				<div class="row">
					<label for="mmdp"><span>*</span> Mot de passe :</label>
					<input type="password" class="form-text" name="mmdp" id="mmdp" />
				</div>
				<input type="submit" class="btn-submit" value="<?php echo $title;?>" />
				</form><div class="row" id="fgpwd"><a href="http://pdf.lesoir.be/liseuseSSO/authentification/forget.php?aff=FULL&amp;backurlPDF='+encodeURIComponent(window.location.href)+'">J'ai oubli&eacute; mon mot de passe</a></div>
			</div>
		</div>
		<div class="p-bottom">&nbsp;</div>
	</div>
</div>
<iframe id="ssol" style="display: none;">
</iframe>