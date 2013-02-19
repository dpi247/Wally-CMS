CKEDITOR.plugins.add('cciinlineobjects', {
	init: function (editor) {
		var iconPath = this.path + 'images/inline.png';
		var customImageIconPath = this.path + 'images/customImage.png';
		var removeIconPath = this.path + 'images/remove.png';

		editor.addCommand('addInlineObject', new CKEDITOR.dialogCommand('inlineObjectDialog'));
		editor.addCommand('removeInlineObject', {
			exec: function(editor) {
				removeInlineObject(editor);
			}
		});

		editor.ui.addButton('cciinlineobjects', {
			label: 'Add Inline Object',
			command: 'addInlineObject',
			icon: iconPath
		});

		CKEDITOR.dialog.add('inlineObjectDialog', function(editor) {
			return getInlineObjectDialog(editor);
		});
		
		if (editor.contextMenu) {
			editor.addMenuGroup('editInlineGroup');
			editor.addMenuItem('customImage', {
				label: 'Image Properties',
				icon: customImageIconPath,
				command: 'image',
				group: 'editInlineGroup'
			});
			editor.removeMenuItem('image');
			editor.addMenuItem('editInlineItem', {
				label: 'Edit inline object',
				icon: iconPath,
				command: 'addInlineObject',
				group: 'editInlineGroup'
			});
			editor.addMenuItem('removeInlineItem', {
				label: 'Remove inline object',
				icon: removeIconPath,
				command: 'removeInlineObject',
				group: 'editInlineGroup'
			});
			editor.contextMenu.addListener(function(element) {
				var tmp_element = false;
				if (element) {
					tmp_element = element.getAscendant('processedinline', true);
				}
				if (tmp_element && !tmp_element.isReadOnly() && !tmp_element.data('cke-realelement')) {
					return {editInlineItem: CKEDITOR.TRISTATE_OFF, removeInlineItem: CKEDITOR.TRISTATE_OFF};
				}
				if (element) {
					element = element.getAscendant('img', true);
				}
				if (element && !element.isReadOnly() && !element.data('cke-realelement')) {
					return {customImage: CKEDITOR.TRISTATE_OFF};
				}
				return null;
			});
		}
		
		editor.on('beforeGetData', function(e) {
			// Remove "processedinline" tags
			var regex = new RegExp("<processedinline>.*?</processedinline>", "g");;
			if (e.editor.document) {
				e.editor.document.$.body.innerHTML = e.editor.document.$.body.innerHTML.replace(regex, '');
			} else {
				e.editor.element.$.defaultValue = e.editor.element.$.defaultValue.replace(regex, '');
			}
		});
		
		editor.on('instanceReady', function(e) {
			// Process inline tags
			processTextToInlineObjects(e.editor);
		});
	}
});

function processTextToInlineObjects(editor) {
	var defaultTextValue = '';
	if (editor.document) {
		defaultTextValue = editor.document.getBody().$.innerHTML;
	} else {
		defaultTextValue = editor.element.$.defaultValue;
	}

	$.ajax({
		type: "POST",
		url: "/ckeditor/processinlineobjects",
		cache: false,
		data: {
			'inline_objects_text': defaultTextValue,
			'prenode_cache_name': Drupal.settings.cache_name
		},
		complete: function(data) {
			editor.document.$.body.innerHTML = data.responseText;
		}
	});
}

function getInlineObjectDialog(editor) {
	var dialog_elements = [];
	if (typeof Drupal.settings.embed_objects_options != "undefined") {
		var objects_options = [];
		for (var key in Drupal.settings.embed_objects_options) {
			objects_options.push([key, Drupal.settings.embed_objects_options[key]]);
		}
		var wallycrops_options = [];
		for (var key in Drupal.settings.wallycrops) {
			wallycrops_options.push([key, Drupal.settings.wallycrops[key]]);
		}
		if (objects_options == [] || wallycrops_options == []) {
			dialog_elements = [{
				type : 'html',
				html : 'No embed objects to insert.'		
			}];
		} else {
			var align_options = [['Left','left'],['Right','right'],['Center','center']];
			
			dialog_elements = [{
				type: 'select',
				id: 'embeds',
				label: 'Embed object',
				items: objects_options,
				validate : CKEDITOR.dialog.validate.notEmpty("Embed object cannot be empty"),
				setup: function(element) {
					this.setValue(element.getAttribute('extref'));
				},
				commit: function(element) {
					element.setAttribute('extref', this.getValue());
				}
			},{
				type: 'select',
				id: 'wallycrop',
				label: 'Croping preset',
				items: wallycrops_options,
				validate : CKEDITOR.dialog.validate.notEmpty("Croping preset cannot be empty"),
				setup: function(element) {
					this.setValue(element.getAttribute('preset'));
				},
				commit: function(element) {
					element.setAttribute('preset', this.getValue());
				}
			},{
				type: 'select',
				id: 'align',
				label: 'Alignment',
				items: align_options,
				validate : CKEDITOR.dialog.validate.notEmpty("Alignment value cannot be empty"),
				setup: function(element) {
					this.setValue(element.getAttribute('align'));
				},
				commit: function(element) {
					element.setAttribute('align', this.getValue());
				}
			},{
				type: 'text',
				id: 'runaround',
				label: 'Margin (px)',
				validate: CKEDITOR.dialog.validate.number("The margin has to be a numerical value"),
				setup: function(element) {
					this.setValue(element.getAttribute('runaround'));
				},
				commit: function(element) {
					element.setAttribute('runaround', this.getValue());
				}
			}];
		}
	} else {
		dialog_elements = [{
			type : 'html',
			html : 'No embed objects to insert.'		
		}];
	}

	return {
		title: 'Add Inline Object',
		minWidth: 400,
		minHeight: 300,
		contents: [{
			id: 'addinlineobject',
			label: 'Add Inline Object',
			elements: dialog_elements
		}],
		onShow: function() {
			var sel = editor.getSelection();
			element = sel.getStartElement();
			if (element) {
				element = element.getAscendant('processedinline', true);
			}
			if (!element || element.getName() != 'processedinline' || element.data('cke-realelement')) {
				element = editor.document.createElement('ccix:inline');
				this.insertMode = true;
			} else {
				element = element.$.previousElementSibling;
				this.insertMode = false;
			}
			this.element = element;
			this.setupContent(this.element);
		},
		onOk: function() {
			addInlineObjectSubmit(editor, this);
		}
	};
}

function addInlineObjectSubmit(editor, dialog) {
	if (typeof Drupal.settings.embed_objects_options != "undefined") {
		var inline = dialog.element;
		if (dialog.insertMode) {
			editor.insertElement(inline);
		}
		dialog.commitContent(inline);
		
		processTextToInlineObjects(editor);
	}
}

function removeInlineObject(editor) {
	var sel = editor.getSelection();
	var img_element = sel.getStartElement();
	if (img_element) {
		var process_element = img_element.getAscendant('processedinline', true);
		editor.document.$.body.innerHTML = editor.document.$.body.innerHTML.replace(process_element.$.outerHTML, '');
		editor.document.$.body.innerHTML = editor.document.$.body.innerHTML.replace(process_element.$.previousElementSibling.outerHTML, '');
	}
}
