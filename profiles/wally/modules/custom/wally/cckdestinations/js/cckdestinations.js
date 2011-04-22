function updateList() {
	$(".tid").each(function(index) {
		taxo = this.id;
		targ = taxo.replace("tid", "target");
		lay = taxo.replace("tid", "layout");
		var selected_targ = $("#"+targ).find("option:selected").attr("value");
		var selected_lay = $("#"+lay).find("option:selected").attr("value");
		makeSublist(taxo, targ, true, selected_targ);
		makeSublist(targ, lay, true, selected_lay);
	});
}

function makeSublist(parent, child, isSubselectOptional, childVal) {
	if ($("#"+parent+child).length==0) {
		$("body").append("<select style='display:none' id='"+parent+child+"'></select>");
		$("#"+parent+child).html($("#"+child+" option"));
	}
	
	var parentValue = $("#"+parent).find("option:selected").attr("title");
	$("#"+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
	childVal = (typeof childVal == "undefined") ? "" : childVal ;
	$("#"+child+' option[value="'+ childVal +'"]').attr("selected","selected");
	
	$("#"+parent).change(function() {
		var parentValue = $("#"+parent).find("option:selected").attr("title");
		$("#"+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
		if(isSubselectOptional)
			$("#"+child).prepend("<option value=''> -- Select -- </option>");
		$("#"+child).trigger("change");
		$("#"+child).focus();
	});
}

$(document).ready(function() {
	updateList();
	var i = $("#field_destinations_values tbody tr").length;
	$("#field-destinations-items").bind("DOMNodeInserted", function(event) {
		if ($("#field_destinations_values tbody tr").length > i) {
			updateList();
			i = $("#field_destinations_values tbody tr").length;
		}
	});
});
