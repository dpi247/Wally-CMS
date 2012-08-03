$(function(){
	
	$("div.emb_package_bpic.hidden").hide();
	$("div.emb_package_thumbs li").click(function(){
		$("div.emb_package_bpic").hide();
		$("div#"+this.id+".emb_package_bpic").show();
	});
});