// $Id: supercron.admin.rule.js,v 1.1 2010/03/18 23:25:59 63reasons Exp $

function supercron_rule_wizard() {
  // Minutes
  try {
    var minutes = $('#edit-options-wizard-wrapper [name="minutes"]:checked').val();
    if (minutes.length == 0) {
      try {
        minutes = $('#edit-options-wizard-wrapper #edit-minutes-custom').val();
        minutes = minutes.join(',');
      }
      catch(e) {
        minutes = '*';
      }
    }
  }
  catch(e) {
    minutes = '*';
  }

  // Hours
  try {
    var hours = $('#edit-options-wizard-wrapper [name="hours"]:checked').val();
    if (hours.length == 0) {
      try {
        hours = $('#edit-options-wizard-wrapper #edit-hours-custom').val();
        hours = hours.join(',');
      }
      catch(e) {
        hours = '*';
      }
    }
  }
  catch(e) {
    hours = '*';
  }

  // Days of month
  try {
    var days = $('#edit-options-wizard-wrapper [name="days"]:checked').val();
    if (days.length == 0) {
      try {
        days = $('#edit-options-wizard-wrapper #edit-days-custom').val();
        days = days.join(',');
      }
      catch(e) {
        days = '*';
      }
    }
  }
  catch(e) {
    hours = '*';
  }

  // Months
  try {
    var months = $('#edit-options-wizard-wrapper [name="months"]:checked').val();
    if (months.length == 0) {
      try {
        months = $('#edit-options-wizard-wrapper #edit-months-custom').val();
        months = months.join(',');
      }
      catch(e) {
        months = '*';
      }
    }
  }
  catch(e) {
    months = '*';
  }

  // Week days
  try {
    var week_days = $('#edit-options-wizard-wrapper [name="week_days"]:checked').val();
    if (week_days.length == 0) {
      try {
        week_days = $('#edit-options-wizard-wrapper #edit-week-days-custom').val();
        week_days = week_days.join(',');
      }
      catch(e) {
        week_days = '*';
      }
    }
  }
  catch(e) {
    week_days = '*';
  }

  return [minutes, hours, days, months, week_days].join(' ');
}

function _supercron_update_wizard_form(section, values) {
  if ($('[id="edit-'+ section +'-' + values + '"]').size()) {
    $('[id="edit-'+ section +'-' + values + '"]').trigger('click');
  }
  else {
    $('[id="edit-'+ section +'-"]').trigger('click');

    $('#edit-'+ section +'-custom option')
      .removeClass('supercron-processed')
      .removeAttr('selected');
    values = values.split(',');
    for (var i in values) {
      $('#edit-'+ section +'-custom option:not(.supercron-processed)').each(function(){
        if ($(this).val() == values[i]) {
          $(this).attr('selected', 'selected').addClass('supercron-processed');
        }
        else {
          $(this).removeAttr('selected');
        }
      });
    }
  }
}

function supercron_update_wizard_form() {
  var query = '';
  
  // Reset the wizard form first
  query = '#supercron-settings-rule-toolbox input[type="reset"]';
  $(query).trigger('click');

  // Get active rule value
  query = '#supercron-settings .supercron-rule-setting:eq('+ supercron_active_rule +')';
  var rules = $(query).parent().find('input').val();

  if (rules.length == 0) {
    // Reset rule
    query = '#edit-minutes-, #edit-hours-, #edit-days-, #edit-months-, #edit-week-days-, #edit-options-all';
    $(query).trigger('click');
    return;
  }

  try {
    // Click the option
    $('#edit-options-wizard').trigger('click');

    rules = rules.split(' ');
    for (var i in rules) {
      var section = '';
      switch (parseInt(i)) {
        // Minutes
        case 0:
          section = 'minutes';
          break;

        // Hours
        case 1:
          section = 'hours';
          break;

        // Day of month
        case 2:
          section = 'days';
          break;
          
        // Months
        case 3:
          section = 'months';
          break;
          
        // Days of week
        case 4:
          section = 'week-days';
          break;
      }

      if (section) {
        _supercron_update_wizard_form(section, rules[i]);
      }
    }

    // @TODO: Open the wizard section
    $('#edit-options-wizard-wrapper').trigger('click');
  }
  catch(e) {
    return;
  }
}

var supercron_active_rule = 0;

Drupal.behaviors.supercron_rules = function(context) {
  var query = '';
  
  /**
   * Add clock icons to rule textfields
   */
  $('form#supercron-settings .form-text.supercron-rule', context).each(function(){
    $(this).after('<a href="#TB_inline?height=500&width=1000&inlineId=supercron-settings-rule-toolbox" class="thickbox supercron-rule-setting"></a>');
  });

  /**
   * Handle these icon click-event
   */
  $('form#supercron-settings a.supercron-rule-setting', context).each(function(i){
    $(this).click(function(){
      supercron_active_rule = i;

      // Update wizard form
      supercron_update_wizard_form();

      return false;
    });
  });

  /**
   * Move...
   */
  $('fieldset.supercron-wizard').appendTo('#edit-options-wizard-wrapper');
  $('fieldset.supercron_advanced_fieldset').appendTo('#edit-options-advanced-wrapper');

  query = '#supercron_rule_toolbox > div.form-radios > div.form-item';
  var $options = $(query);

  // Hide sub sections.
  $options.parent().find('div.form-item > fieldset').hide().end()
  
  $options.each(function(){
    var $option = $(this);

    $('input[name="options"]', $(this)).click(function(){
      // Hide others
      $options.parent().find('div.form-item > fieldset').hide();

      $option.find('fieldset:eq(0)').fadeIn();
    });
  });

  // Handle rule generating button
  $('#supercron_rule_toolbox #edit-generate-rule').click(function(){
    var option = $options.find('input[name="options"]:checked').val();
    var rule   = '';

    switch (option) {
      case 'all':
        // Clean rule textfield
        break;

      case 'advanced':
        // Copy advanced-rule-textfield to rule text field.
        rule = $('#edit-advanced').val();
        break;

      case 'wizard':
        rule = supercron_rule_wizard();
        break;
    };

    // Update rule field
    $('form#supercron-settings a.supercron-rule-setting:eq('+ supercron_active_rule +')', context)
      .parent().find('input').val(rule);

    // Close thickbox
    $('#TB_closeWindowButton').trigger('click');

    // Bread form submission
    return false;
  });
};
