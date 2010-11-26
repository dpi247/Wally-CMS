function makeSublist(parent,child,isSubselectOptional,childVal)
{
  
  
	$("body").append("<select style='display:none' id='"+parent+child+"'></select>");
	$('#'+parent+child).html($("#"+child+" option"));
	  
//	var parentValue = $('#'+parent).attr("value");

var parentValue = $('#'+parent).find("option:selected").attr("title");
  $('#'+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
	childVal = (typeof childVal == "undefined")? "" : childVal ;
	$("#"+child+' option[@value="'+ childVal +'"]').attr('selected','selected');
	$('#'+parent).change( 
		function()
		{
			var parentValue = $('#'+parent).find("option:selected").attr("title");
			$('#'+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
			if(isSubselectOptional) $('#'+child).prepend("<option value=''> -- Select -- </option>");
			$('#'+child).trigger("change");
                        $('#'+child).focus();
		}
	);
}

$(document).ready(function()
{
		$('.tid').each(
		function(index) {
		taxo = this.id;
		targ = taxo.replace("tid", "target");
		lay = taxo.replace("tid", "layout");
		makeSublist(targ,lay, true, '');	
		makeSublist(taxo,targ, true, '');	
	});
  
});
