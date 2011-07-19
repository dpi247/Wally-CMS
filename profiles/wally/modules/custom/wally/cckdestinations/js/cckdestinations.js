function updateList() {
	$(".tid").each(function(index) {
		taxo = this.id;
		targ = taxo.replace("tid", "target");
		lay = taxo.replace("tid", "layout");
		del = taxo.replace("tid", "delete");
		var selected_targ = $("#"+targ).find("option:selected").attr("value");
		var selected_lay = $("#"+lay).find("option:selected").attr("value");
		makeSublist(taxo, targ, true, selected_targ);
		makeSublist(targ, lay, true, selected_lay);
		
		addDeleteAction(del);
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
		if(typeof parentValue != "undefined" && parentValue != "0" && isSubselectOptional)
			$("#"+child).prepend("<option value=''> -- Select -- </option>");
		$("#"+child).trigger("change");
		$("#"+child).focus();
	});
}

function addDeleteAction(del) {
	if ($("#"+del).length > 0) {
		$("#"+del).click(function() {
			var cur_taxo = this.id.replace("delete", "tid");
			var selected = $("#"+cur_taxo).find("option:selected");
			selected.removeAttr("selected");
			$("#"+cur_taxo+' option[value="0"]').attr("selected", "selected");

			var not_found = true;
			var parent_tr = $("#"+cur_taxo);
			while (not_found) {
				parent_tr = parent_tr.parent();
				if (parent_tr.hasClass("draggable")) {
					not_found = false;
				}
			}
			parent_tr.hide();
			
			var cur_targ = cur_taxo.replace("tid", "target");
			var cur_lay = cur_taxo.replace("tid", "layout");
			var cur_selected_targ = $("#"+cur_targ).find("option:selected").attr("value");
			var cur_selected_lay = $("#"+cur_lay).find("option:selected").attr("value");
			makeSublist(cur_taxo, cur_targ, true, cur_selected_targ);
			makeSublist(cur_targ, cur_lay, true, cur_selected_lay);
		});
	}
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
