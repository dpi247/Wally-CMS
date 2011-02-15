<?php
// $Id: supercron.admin.rule.inc,v 1.1 2010/03/18 23:02:38 63reasons Exp $

/**
 * @file
 * Contains administrative configuration details for complex cron scheduling
 */

include_once(drupal_get_path('module', 'supercron') . '/supercron-main.inc');

function supercron_settings_rule_toolbox() {
  $module_path = drupal_get_path('module', 'supercron');
  drupal_add_js("{$module_path}/js/supercron.admin.rule.js");
  drupal_add_js("{$module_path}/js/jquery.thickbox/thickbox-compressed.js");
  drupal_add_css("{$module_path}/js/jquery.thickbox/thickbox.css");

  $form['rule_toolbox'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rule toolbox'),
    '#attributes' => array(
      'id' => 'supercron_rule_toolbox',
    ),
  );

  $form['rule_toolbox']['options'] = array(
    '#type' => 'radios',
    '#options' => array(
      'all'      => t('Execute with every cron call'),
      'wizard'   => t('Execute according to these rules'),
      'advanced' => t('Advanced definition')
    ),
    '#default_value' => 'wizard'
  );

  $form['rule_toolbox']['wizard'] = array(
    '#type' => 'fieldset',
    # '#title' => t('Wizard'),
    '#attributes' => array('class' => 'supercron-wizard'),
  );

  $form['rule_toolbox']['wizard']['minute'] = array(
    '#type' => 'fieldset',
    '#title' => t('Minutes'),
    'minutes_custom' => array(
      '#type' => 'select',
      '#title' => t('Custom'),
      '#options' => range(0, 59),
      '#multiple' => TRUE,
      '#size' => 10,
    ),
    'minutes' => array(
      '#title' => t('Templates'),
      '#type' => 'radios',
      '#multiple' => TRUE,
      '#attributes' => array('class' => 'supercron-wizard-minutes'),
      '#options' => array(
        '' => t('Custom'),
        '*'  => t('Every Minute'),
        '0,5,10,15,20,25,30,35,40,45,50,55'  => t('Every 5 Minutes'),
        '0,10,20,30,40,50' => t('Every 10 Minutes'),
        '0,15,30,45' => t('Every 1/4 Hour'),
        '0,30' => t('Every 1/2 Hour'),
        '0'  => t('Once Per Hour'),
      ),
    )
  );

  $form['rule_toolbox']['wizard']['hour'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hours'),
    'hours_custom' => array(
      '#type'     => 'select',
      '#title'    => t('Custom'),
      '#options'  => range(0, 23),
      '#multiple' => TRUE,
      '#size' => 10,
    ),
    'hours' => array(
      '#title' => t('Templates'),
      '#type' => 'radios',
      '#multiple' => TRUE,
      '#attributes' => array('class' => 'supercron-wizard-minutes'),
      '#options' => array(
        '' => t('Custom'),
        '*'  => t('Every Hour'),
        '0,2,4,6,8,10,12,14,16,18,20,22' => t('Even Hours'),
        '1,3,5,7,9,11,13,15,17,19,21,23' => t('Odd Hours'),
        '0,6,12,18'  => t('Every 1/4 Day'),
        '0,12' => t('Every 1/2 Day'),
        '0' => t('Once Per Day'),
      ),
    )
  );

  $day_options = array();
  for ($i = 1; $i <= 31; ++$i) {
    $day_options[$i] = $i;
  }

  $form['rule_toolbox']['wizard']['day'] = array(
    '#type' => 'fieldset',
    '#title' => t('Days of month'),
    'days_custom' => array(
      '#type' => 'select',
      '#title' => t('Custom'),
      '#options' => $day_options,
      '#multiple' => TRUE,
      '#size' => 10,
    ),
    'days' => array(
      '#title' => t('Templates'),
      '#type' => 'radios',
      '#multiple' => TRUE,
      '#attributes' => array('class' => 'supercron-wizard-minutes'),
      '#options' => array(
        ''   => t('Custom'),
        '*'  => t('Every Day'),
        '2,4,6,8,10,12,14,16,18,20,22,24,26,28,30' => t('Even Days'),
        '1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31' => t('Odd Days'),
        '1,15'  => t('Twice Per Month'),
        '1' => t('Once Per Month'),
      ),
    )
  );

  $form['rule_toolbox']['wizard']['month'] = array(
    '#type' => 'fieldset',
    '#title' => t('Months'),
    '#attributes' => array('style' => 'clear: left'),
    'months_custom' => array(
      '#type' => 'select',
      '#title' => t('Custom'),
      '#options' => range(0, 59),
      '#multiple' => TRUE,
      '#size' => 10,
    ),
    'months' => array(
      '#title' => t('Templates'),
      '#type' => 'radios',
      '#multiple' => TRUE,
      '#attributes' => array('class' => 'supercron-wizard-minutes'),
      '#options' => array(
        '' => t('Custom'),
        '*'  => t('Every Month'),
        '2,4,6,8,10,12' => t('Even Months'),
        '1,3,5,7,9,11' => t('Odd Months'),
        '1,4,7,10'  => t('Every 1/4 Year'),
        '1,7' => t('Every 1/2 Year'),
        '1' => t('Once Per Year'),
      ),
    )
  );

  $form['rule_toolbox']['wizard']['week_day'] = array(
    '#type' => 'fieldset',
    '#title' => t('Days of week'),
    'week_days_custom' => array(
      '#type' => 'select',
      '#title' => t('Custom'),
      '#options' => range(0, 59),
      '#multiple' => TRUE,
      '#size' => 10,
    ),
    'week_days' => array(
      '#title' => t('Templates'),
      '#type' => 'radios',
      '#multiple' => TRUE,
      '#attributes' => array('class' => 'supercron-wizard-minutes'),
      '#options' => array(
        '' => t('Custom'),
        '*'  => t('Every Day of the Week'),
        '1-5' => t('Non-Weekend Days (Mon-Fri)'),
        '0,6' => t('Weekend Days'),
        '0' => t('Once Per Week (Sunday)'),
      ),
    )
  );

  $form['rule_toolbox']['radios']['advanced-fieldset'] = array(
    '#type'  => 'fieldset',
    '#attributes' => array(
      'class' => 'supercron_advanced_fieldset',
    ),
    'advanced' => array(
      '#type'  => 'textfield',
      '#title' => t('Advanced definition'),
    )
  );

  $form['rule_toolbox']['generate_rule'] = array(
    '#type' => 'button',
    '#value' => t('Update rule')
  );

  $form['rule_toolbox']['reset'] = array(
    '#value' => '<input class="form-button" type="reset" value="Reset" />',
  );

  return $form;
}
