// $Id: supercron.admin.js,v 1.1 2010/03/18 23:02:38 63reasons Exp $

/**
 * Show/Hide custom script path textfield.
 */
Drupal.behaviors.supercron_script_path = function(context) {
  $('#supercron-settings input[name="supercron_script_path"]', context).click(function(){
    if ($(this).val() == 3) {
      $('#edit-supercron-script-path-text').show();
    }
    else {
      $('#edit-supercron-script-path-text').hide();
    }
  });

  try {
    if ($('#supercron-settings input[name="supercron_script_path"]:checked', context).val() != 3) {
      $('#edit-supercron-script-path-text').hide();
    }
  }
  catch(e) {
    $('#edit-supercron-script-path-text').hide();
  }
};
