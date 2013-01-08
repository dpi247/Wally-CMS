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
			var targ = taxo.replace(short_str_to_replace, "target");
			shortMakeSublist(taxo, targ);
			$(this).bind("change", function(event) {
				shortMakeSublist(taxo, targ);
			});
		}
	});
}

function shortMakeSublist(taxo, targ) {
	if (short_str_to_replace == "tid" || (taxo.substring(taxo.length - 3, taxo.length) != "tid" && taxo.substring(taxo.length - 5, taxo.length) != short_str_to_replace)) {
		var selected_tid = $("#"+taxo).find("option:selected").val();
	} else {
		var term_value = $("#"+taxo).val();
		term_value = term_value.substring(0, term_value.length - 1);
		var expl_term_value = term_value.split('[');
		var selected_tid = expl_term_value[expl_term_value.length - 1];
	}

	var selected_targ = $("#"+targ).find("option:selected").val();

	$("#"+targ).html('');
	if (selected_tid > 0) {
		var select_options = Drupal.settings.shortcckdestchoices;

		var targ_options = select_options[selected_tid].redacblocks;
		$.each(targ_options, function() {
			$("#"+targ).append('<option value="'+this.name+'">'+this.title+'</option>');
		});
		$("#"+targ+' option[value="'+selected_targ+'"]').attr("selected", "selected");
	}
}
