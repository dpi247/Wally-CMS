var str_to_replace = "tid";

$(document).ready(function() {
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
			str_to_replace = "tid-"+last_char;
		}
	}

	updateList();
	$("#field-destinations-items").bind("DOMNodeInserted", function(event) {
		if (event.target.nodeName == "TABLE") {
			updateList();
		}
	});
});

function updateList() {
	$(".tid").each(function(index) {
		var taxo = this.id;
		if (taxo != '') {
			var targ = taxo.replace(str_to_replace, "target");
			var lay = taxo.replace(str_to_replace, "layout");
			
			makeSublist(taxo, targ, lay);
			$(this).bind("change", function(event) {
				makeSublist(taxo, targ, lay);
			});
		}
	});
	
	$("#field-destinations-items").bind("DOMNodeRemoved", function(event) {
		if (event.target.id == "autocomplete") {
			var taxo = event.target.owner.input.id;
			var targ = taxo.replace(str_to_replace, "target");
			var lay = taxo.replace(str_to_replace, "layout");
			makeSublist(taxo, targ, lay);
		}
	});
}

function makeSublist(taxo, targ, lay) {
	if (str_to_replace == "tid" || (taxo.substring(taxo.length - 3, taxo.length) != "tid" && taxo.substring(taxo.length - 5, taxo.length) != str_to_replace)) {
		var selected_tid = $("#"+taxo).find("option:selected").val();
	} else {
		var term_value = $("#"+taxo).val();
		term_value = term_value.substring(0, term_value.length - 1);
		var expl_term_value = term_value.split('[');
		var selected_tid = expl_term_value[expl_term_value.length - 1];
	}
	
	var selected_targ = $("#"+targ).find("option:selected").val();
	var selected_lay = $("#"+lay).find("option:selected").val();
	$("#"+targ).html('');
	$("#"+lay).html('');
	
	if (selected_tid > 0) {
		var select_options = Drupal.settings.cckdestchoices;
		
		updateSublist(select_options[selected_tid].redacblocks, targ, selected_targ);
		updateSublist(select_options[selected_tid].redacblocks[$("#"+targ).find("option:selected").val()].layouts, lay, selected_lay);
		$("#"+targ).bind("change", function(event) {
			selected_lay = $("#"+lay).find("option:selected").val();
			$("#"+lay).html('');
			updateSublist(select_options[selected_tid].redacblocks[$("#"+targ).find("option:selected").val()].layouts, lay, selected_lay);
		});
	}
}

function updateSublist(options, id, default_option) {
	$.each(options, function() {
		$("#"+id).append('<option value="'+this.name+'">'+this.title+'</option>');
	});
	$("#"+id+' option[value="'+default_option+'"]').attr("selected", "selected");
}
