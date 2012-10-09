var short_str_to_replace;

Drupal.behaviors.destinationForm = function(context) {
	$(document).ready(function() {
		short_str_to_replace = "tid";

		var one_taxo = '';
		$(".tid").each(function(index) {
			var tmp_taxo = this.id;
			if (tmp_taxo != '') {
				one_taxo = tmp_taxo;
				return false;
			}
		});
		if (typeof one_taxo != "undefined" && one_taxo != '') {
			var last_char = one_taxo.substring(one_taxo.length - 1, one_taxo.length);
			if (last_char.match('^(0|[1-9][0-9]*)$')) {
				short_str_to_replace = "tid-"+last_char;
			}
		}
		shortUpdateList();
	});
};

function shortUpdateList() {
	$(".tid").each(function(index) {
		var taxo = this.id;
		if (taxo != '') {
			targ = taxo.replace(short_str_to_replace, "target");
			var selected_targ = $("#"+targ).find("option:selected").attr("value");
			shortMakeSublist(taxo, targ, selected_targ);
		}
	});
}

function shortMakeSublist(parent, child, childVal) {
	if ($("#"+parent+child).length==0) {
		$("body").append("<select style='display:none' id='"+parent+child+"'></select>");
		$("#"+parent+child).html($("#"+child+" option"));
	}
	
	if (short_str_to_replace == "tid" || (parent.substring(parent.length - 3, parent.length) != "tid" && parent.substring(parent.length - 5, parent.length) != short_str_to_replace)) {
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

	$("#edit-conf-destination-tid-1-wrapper").bind("DOMNodeRemoved", function(event) {
		if (event.target.id == "autocomplete") {
			if (short_str_to_replace == "tid" || (parent.substring(parent.length - 3, parent.length) != "tid" && parent.substring(parent.length - 5, parent.length) != short_str_to_replace)) {
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
			$("#"+child).trigger("change");
		}
	});
	
	$("#"+parent).bind("change", function(event) {
		if (short_str_to_replace == "tid" || (parent.substring(parent.length - 3, parent.length) != "tid" && parent.substring(parent.length - 5, parent.length) != short_str_to_replace)) {
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
		$("#"+child).trigger("change");
	});
}

