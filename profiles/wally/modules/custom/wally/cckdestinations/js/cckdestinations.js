var hidden_dest = [];
var str_to_replace = "tid";

$(document).ready(function() {
	var one_taxo = $(".tid:first").attr("id");
	var last_char = one_taxo.substring(one_taxo.length - 1, one_taxo.length);
	if (last_char.match('^(0|[1-9][0-9]*)$')) {
		str_to_replace = "tid-"+last_char;
	}
	
	updateList();
	//var i = $("#field_destinations_values tbody tr").length;
	$("#field-destinations-items").bind("DOMNodeInserted", function(event) {
		//if ($("#field_destinations_values tbody tr").length > i && event.target.nodeName == "DIV") {
		if (event.target.nodeName == "DIV") {
			updateList();
			refreshHidden();
			//i = $("#field_destinations_values tbody tr").length;
		}
	});
});

function updateList() {
	$(".tid").each(function(index) {
		taxo = this.id;
		
		targ = taxo.replace(str_to_replace, "target");
		lay = taxo.replace(str_to_replace, "layout");
		del = taxo.replace(str_to_replace, "delete");
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
	
	if (str_to_replace == "tid" || (parent.substring(parent.length - 3, parent.length) != "tid" && parent.substring(parent.length - 5, parent.length) != str_to_replace)) {
		var parentValue = $("#"+parent).find("option:selected").attr("title");
	} else {
		var term_value = $("#"+parent).val();
		term_value = term_value.substring(0, term_value.length - 1);
		var expl_term_value = term_value.split('[');
		var parentValue = expl_term_value[expl_term_value.length - 1];
	}
	
	$("#"+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
	childVal = (typeof childVal == "undefined") ? "" : childVal ;
	$("#"+child+' option[value="'+ childVal +'"]').attr("selected","selected");
	
	$("#"+parent).bind("change DOMSubtreeModified", function(event) {
		if (str_to_replace == "tid" || (parent.substring(parent.length - 3, parent.length) != "tid" && parent.substring(parent.length - 5, parent.length) != str_to_replace)) {
			var parentValue = $("#"+parent).find("option:selected").attr("title");
		} else {
			var term_value = $("#"+parent).val();
			term_value = term_value.substring(0, term_value.length - 1);
			var expl_term_value = term_value.split('[');
			var parentValue = expl_term_value[expl_term_value.length - 1];
		}

		$("#"+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
		if(typeof parentValue != "undefined" && parentValue != "0" && isSubselectOptional)
			$("#"+child).prepend("<option value=''> -- Select -- </option>");
		$("#"+child).trigger("change");
		//$("#"+child).focus();
	});
}

function addDeleteAction(del) {
	if ($("#"+del).length > 0) {
		$("#"+del).click(function() {
			var cur_taxo = hideTr(this.id);
			
			var expl_del = del.split("-");
			var del_id = expl_del[3];
			hidden_dest[del_id] = del_id;
			
			var cur_targ = cur_taxo.replace(str_to_replace, "target");
			var cur_lay = cur_taxo.replace(str_to_replace, "layout");
			var cur_selected_targ = $("#"+cur_targ).find("option:selected").attr("value");
			var cur_selected_lay = $("#"+cur_lay).find("option:selected").attr("value");
			makeSublist(cur_taxo, cur_targ, true, cur_selected_targ);
			makeSublist(cur_targ, cur_lay, true, cur_selected_lay);
		});
	}
}

function hideTr(del_id) {
	var cur_taxo = del_id.replace("delete", str_to_replace);
	if (str_to_replace == 'tid') {
		var selected = $("#"+cur_taxo).find("option:selected");
		selected.removeAttr("selected");
		$("#"+cur_taxo+' option[value="0"]').attr("selected", "selected");
	} else {
		$("#"+cur_taxo).val("");
	}

	var not_found = true;
	var parent_tr = $("#"+cur_taxo);
	while (not_found) {
		parent_tr = parent_tr.parent();
		if (parent_tr.hasClass("draggable")) {
			not_found = false;
		}
	}
	parent_tr.hide();
	
	return cur_taxo;
}

function refreshHidden() {
	for(var i in hidden_dest) {
		var del_to_hide = "edit-field-destinations-"+i+"-delete";
		hideTr(del_to_hide);
	}
}
