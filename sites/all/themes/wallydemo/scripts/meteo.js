function makeSublist(parent,child,isSubselectOptional,childVal)
{
	
	$("body").append("<select style='display:none' id='"+parent+child+"'></select>");
	$('#'+parent+child).html($("#"+child+" option"));
	
	var parentValue = $('#'+parent).attr('value');
	$('#'+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
	
	var childVal = (typeof childVal == "undefined")? "" : childVal ;
	$("#"+child+' option[value="'+ childVal +'"]').attr('selected','selected');
	
	$('#'+parent).change( 
		function()
		{
			var parentValue = $('#'+parent).attr('value');
			$('#'+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
			if(isSubselectOptional) $('#'+child).prepend("<option value='none'> -- Select -- </option>");
			$('#'+child).trigger("change");
                        $('#'+child).focus();
		}
	);
}


$(document).ready(function(){
	
	// Bloc recherche météo internationale
	var str = "";
	makeSublist('pays','regions', true, '');	
	makeSublist('continents','pays', false, '1');

	$("select#regions").change(function () {
		$("input#edit-international").val($("select#regions").find("option:selected").attr("value"))
	}).change();

	// lien sur bloc
	$(".bloc_meteo, .moyen_bloc_meteo").not("#date_meteo").bind("mouseover", function(){
		$(this).css("cursor", "pointer");
	});
	$(".bloc_meteo, .moyen_bloc_meteo").not("#date_meteo").click(function(){
    	window.location=$(this).find("a").attr("href");
		return false;
	});

	// effet fade sur les cartes sattellite
	/*$('.anim_carte > ul').innerfade({
		animationtype: 'fade',
		speed: 750,
		timeout: 2000,
		type: 'sequence'
	});*/
	
});