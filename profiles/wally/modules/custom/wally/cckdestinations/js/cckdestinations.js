var str_to_replace = "tid";

$(document).ready(function() {
	var one_taxo = $(".tid:first").attr("id");
	var last_char = one_taxo.substring(one_taxo.length - 1, one_taxo.length);
	if (last_char.match('^(0|[1-9][0-9]*)$')) {
		str_to_replace = "tid-"+last_char;
	}

	updateList();
	$("#field-destinations-items").bind("DOMNodeInserted", function(event) {
		if (event.target.nodeName == "DIV") {
			updateList();
		}
	});
});

function updateList() {
	$(".tid").each(function(index) {
		taxo = this.id;
		
		targ = taxo.replace(str_to_replace, "target");
		lay = taxo.replace(str_to_replace, "layout");
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
	
	if (str_to_replace == "tid" || (parent.substring(parent.length - 3, parent.length) != "tid" && parent.substring(parent.length - 5, parent.length) != str_to_replace)) {
		var parentValue = $("#"+parent).find("option:selected").attr("title");
	} else {
		var term_value = $("#"+parent).val();
		term_value = term_value.substring(0, term_value.length - 1);
		var expl_term_value = term_value.split('[');
		var parentValue = expl_term_value[expl_term_value.length - 1];
	}
	
	$("#"+child).html($("#"+parent+child+" .sub_"+parentValue).clone());
	if(typeof parentValue != "undefined" && parentValue != "0" && isSubselectOptional) {
		$("#"+child).prepend("<option value=''> -- Select -- </option>");
	}
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
		if(typeof parentValue != "undefined" && parentValue != "0" && isSubselectOptional) {
			$("#"+child).prepend("<option value=''> -- Select -- </option>");
		}
		childVal = (typeof childVal == "undefined") ? "" : childVal ;
		$("#"+child+' option[value="'+ childVal +'"]').attr("selected","selected");
		$("#"+child).trigger("change");
		//$("#"+child).focus();
	});
}

