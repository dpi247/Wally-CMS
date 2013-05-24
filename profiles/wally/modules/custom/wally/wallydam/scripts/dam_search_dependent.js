
var newsroomSelectID = 'edit-thesaurus-options-newsroom';
var productSelectID = 'edit-thesaurus-options-product';
var zoneSelectID = 'edit-thesaurus-options-zone';
var productsByNewsroom, zonesByProduct;

Drupal.behaviors.refreshPreview = function(context) {
	productsByNewsroom = Drupal.settings.damSearchSelectOptions.products;
	zonesByProduct = Drupal.settings.damSearchSelectOptions.zones;

	restrictProductsToNewsroom();
	restrictZonesToProduct();
	$('#'+newsroomSelectID).change(function(e) {
		// restrict products to only those in the given newsroom
		restrictProductsToNewsroom();
	});
	$('#'+productSelectID).change(function(e) {
		restrictZonesToProduct();
	});
};

function restrictProductsToNewsroom() {
	return restrict($('#'+newsroomSelectID), $('#'+productSelectID), productsByNewsroom);
}

function restrictZonesToProduct() {
	return restrict($('#'+productSelectID), $('#'+zoneSelectID), zonesByProduct, function() {
		var list = [];
		var ns = $('#'+newsroomSelectID+' option[value!=""]:selected');
		if (ns.length == 0) {
			ns = $('#'+newsroomSelectID+' option[value!=""]');
		}
		ns.each(function() {
			var list2 = productsByNewsroom[$(this).val()];
			for (var k = 0; k < list2.length; k++) {
				list.push(list2[k]['code']);
			}
		});
		return list;
	});
}

function restrict(limitingSelect, limitedSelect, table, toutFct) {
	// if 'All' is selected, unselect all others.
	if (limitingSelect.find('option[value = "all"]:selected').attr('selected')) {
		limitingSelect.find('option[value != "all"]:selected').attr('selected', false);
	}
	if (limitingSelect.find('option:selected').val() == 'all') {
		limitingSelect.find('option[value = "all"]').attr('selected', true);
	}

	// save list of products already selected
	var limitedSelected = [];
	limitedSelect.find('option:selected').each(function() {
		limitedSelected.push($(this).val());
	});

	// list of selected elements:
	var limitingSelected = $.map( 
			limitingSelect.find('option[value != "all"]:selected'), function(option) {
				return option.value;
			}
	);
	if (limitingSelected.length == 0) { // if nothing selected, take everything...
		if (toutFct) {
			limitingSelected = toutFct();
		} else {
			for (var limiting in table) {
				limitingSelected.push(limiting);
			}
		}
	}

	// delete all products
	limitedSelect.find('option').remove();

	// add new ones
	$('<option></option>').val('all').html(Drupal.t('All')).appendTo(limitedSelect);

	// collect all the products...
	var newOptions = [];
	for (var i = 0; i < limitingSelected.length; i++) {
		var objs = table[limitingSelected[i]];
		if (objs) {
			Array.prototype.push.apply(newOptions, objs);
		}
	}

	// sort them
	newOptions.sort(function(a, b) {
		return a.dname > b.dname ? +1 : (a.dname < b.dname ? -1 : 0);
	});

	// remove duplicates
	newOptions = newOptions.filter(function(elem, pos) {
		for (var j = 0; j < newOptions.length; j++) {
			if (j < pos && newOptions[j]['code'] == newOptions[pos]['code']) {
				return false;
			}
		}
		return true;
	});

	// make the HTML <option> elements
	for (var j = 0; j < newOptions.length; j++) {
		var newOption = newOptions[j];
		$('<option></option>').val(newOption['code']).html(newOption['dname']).appendTo(limitedSelect);
	}

	// select again the products which are still there
	if (limitedSelected.length > 0) {
		for (var i = 0; i < limitedSelected.length; i++) {
			limitedSelect.find("option[value='" + limitedSelected[i] + "']").attr('selected', 'selected');
		}
	} else {
		limitedSelect.find("option[value='']").attr('selected', true);
	}
}
