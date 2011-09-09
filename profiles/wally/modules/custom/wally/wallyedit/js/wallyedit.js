
/**
 * Move a block in the blocks table from one region to another via select list.
 *
 * This behavior is dependent on the tableDrag behavior, since it uses the
 * objects initialized in that behavior to update the row.
 */
Drupal.behaviors.blockDrag = function(context) {
	var table = $('table#blocks');
	var tableDrag = Drupal.tableDrag.blocks; // Get the blocks tableDrag object.

	// Add a handler for when a row is swapped, update empty regions.
	tableDrag.row.prototype.onSwap = function(swappedRow) {
		checkEmptyRegions(table, this);
	};

	// A custom message for the blocks page specifically.
	Drupal.theme.tableDragChangedWarning = function () {
		return '<div class="warning">' + Drupal.theme('tableDragChangedMarker') + ' ' + Drupal.t("The changes to these blocks will not be saved until the <em>Save blocks</em> button is clicked.") + '</div>';
	};

	// Add a handler so when a row is dropped, update fields dropped into new regions.
	tableDrag.onDrop = function() {
		dragObject = this;
		if ($(dragObject.rowObject.element).prev('tr').is('.onglet-message')) {
			var regionRow = $(dragObject.rowObject.element).prev('tr').get(0);
			var regionName = regionRow.className.replace(/([^ ]+[ ]+)*onglet-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
			var regionField = $('select.my-element-onglet-lev1', dragObject.rowObject.element);
			var weightField = $('select.element-weight-lev1', dragObject.rowObject.element);
			var oldRegionName = weightField[0].className.replace(/([^ ]+[ ]+)*element-weight-lev1-([^ ]+)([ ]+[^ ]+)*/, '$2');

			if (!regionField.is('.my-element-onglet-lev1-'+ regionName)) {
				regionField.removeClass('my-element-onglet-lev1-' + oldRegionName).addClass('my-element-onglet-lev1-' + regionName);
				weightField.removeClass('element-weight-lev1-' + oldRegionName).addClass('element-weight-lev1-' + regionName);
				regionField.val(regionName);
			}
		}
	};

	// Add the behavior to each region select list.
	$('select.my-element-onglet-lev1:not(.blockongletselect-processed)', context).each(function() {
		$(this).change(function(event) {
			// Make our new row and select field.
			var row = $(this).parents('tr:first');
			var select = $(this);
			tableDrag.rowObject = new tableDrag.row(row);
			// Find the correct region and insert the row as the first in the region.
			$('tr.onglet-message', table).each(function() {
				if ($(this).is('.onglet-' + select[0].value + '-message')) {
					// Add the new row and remove the old one.
					$(this).after(row);
					// Manually update weights and restripe.
					tableDrag.updateFields(row.get(0));
					tableDrag.rowObject.changed = true;
					if (tableDrag.oldRowElement) {
						$(tableDrag.oldRowElement).removeClass('drag-previous');
					}
					tableDrag.oldRowElement = row.get(0);
					tableDrag.restripeTable();
					tableDrag.rowObject.markChanged();
					tableDrag.oldRowElement = row;
					$(row).addClass('drag-previous');
				}
			});

			// Modify empty regions with added or removed fields.
			checkEmptyRegions(table, row);
			// Remove focus from selectbox.
			select.get(0).blur();
		});
		$(this).addClass('blockongletselect-processed');
	});

	var checkEmptyRegions = function(table, rowObject) {
		$('tr.onglet-message', table).each(function() {
			// If the dragged row is in this region, but above the message row, swap it down one space.
			if ($(this).prev('tr').get(0) == rowObject.element) {
				// Prevent a recursion problem when using the keyboard to move rows up.
				if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
					rowObject.swap('after', this);
				}
			}
			// This region has become empty
			if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').size() == 0) {
				$(this).removeClass('onglet-populated').addClass('onglet-empty');
			}
			// This region has become populated.
			else if ($(this).is('.onglet-empty')) {
				$(this).removeClass('onglet-empty').addClass('onglet-populated');
			}
		});
	};
};
