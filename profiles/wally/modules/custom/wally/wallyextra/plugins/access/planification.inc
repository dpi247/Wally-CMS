<?php
// $Id: planification.inc,v 1.7.2.2 2009/11/13 00:44:16 merlinofchaos Exp $

/**
 * @file
 * Plugin to provide access control based upon node type.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Planification'),
  'description' => t('Planification of the visibility of this content.'),
  'callback' => 'wallyextra_planificatioin_ctools_access_check',
  'default' => array(
    'all_day' => 1,
    'repeats_type' => 'select',
    'monthly_type' => 'month',
  ),
  'settings form' => 'wallyextra_planification_ctools_access_settings',
  'settings form validate' => 'wallyextra_planification_ctools_access_settings_validate',
  'settings form submit' => 'wallyextra_planification_ctools_access_settings_submit',
  'summary' => 'wallyextra_planification_ctools_access_summary',
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function wallyextra_planification_ctools_access_settings(&$form, &$form_state, $conf) {
  $settings_form = array();

  ctools_include('dependent');
  ctools_add_js('dependent');
  ctools_add_css('planification', 'wallyextra');

  $settings_form['from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('From'),
    '#description' => t('Beginning day'),
    '#default_value' => $conf['from_date'],
    '#required' => TRUE,
  );

  $settings_form['to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('To'),
    '#description' => t('Ending day'),
    '#default_value' => $conf['to_date'],
    '#required' => TRUE,
  );

  $settings_form['all_day'] = array(
    '#type' => 'checkbox',
    '#title' => t('All day'),
    '#default_value' => $conf['all_day'],
  );

  $settings_form['use_recurrency'] = array(
    '#type' => 'checkbox',
    '#title' => t('Repeat...'),
    '#default_value' => $conf['use_recurrency'],
  );

  $settings_form['repeat'] = array(
    '#type' => 'fieldset',
    '#title' => t('Repeat'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#input' => TRUE, //Force process of the fieldset to enable ctools dependent
    '#dependency' => array('edit-use-recurrency' => array(1)),
    '#process' => array('ctools_dependent_process'),
    '#prefix' => '<div id="edit-repeat-wrapper">',
    '#suffix' => '</div>',
  );

  $type_options = array(
    'select' => t('-- Select --'),
    'daily' => t('Daily'),
    'weekday' => t('Every weekday (mon-fry)'),
    'weekend' => t('On week-end days'),
    'weekly' => t('Weekly'),
    'monthly' => t('Monthly'),
  );
  $settings_form['repeat']['repeats_type'] = array(
    '#type' => 'select',
    '#title' => t('Repeats'),
    '#options' => $type_options,
    '#default_value' => $conf['repeats_type'],
    '#dependency' => array('edit-use-recurrency' => array(1)),
    '#process' => array('ctools_dependent_process'),
  );

  $repeat_days_options = array();
  for ($i = 1; $i < 31; $i++) {
    $repeat_days_options[$i] = $i.($i == 1 ? ' day' : ' days');
  }
  $settings_form['repeat']['repeat_days'] = array(
    '#type' => 'select',
    '#title' => t('Repeat every'),
    '#options' => $repeat_days_options,
    '#default_value' => $conf['repeat_days'],
    '#dependency' => array('edit-repeats-type' => array('daily')),
    '#process' => array('ctools_dependent_process'),
  );

  $repeat_weeks_options = array();
  for ($i = 1; $i < 52; $i++) {
    $repeat_weeks_options[$i] = $i.($i == 1 ? ' week' : ' weeks');
  }
  $settings_form['repeat']['repeat_weeks'] = array(
    '#type' => 'select',
    '#title' => t('Repeat every'),
    '#options' => $repeat_weeks_options,
    '#default_value' => $conf['repeat_weeks'],
    '#dependency' => array('edit-repeats-type' => array('weekly')),
    '#process' => array('ctools_dependent_process'),
  );

  $weekly_days_options = array(
    'mon' => t('Mon'),
    'tue' => t('Tue'),
    'wed' => t('Wed'),
    'thu' => t('Thu'),
    'fri' => t('Fri'),
    'sat' => t('Sat'),
    'sun' => t('Sun'),
  );
  $settings_form['repeat']['weekly_days'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Repeat on'),
    '#options' => $weekly_days_options,
    '#default_value' => $conf['weekly_days'],
    '#dependency' => array('edit-repeats-type' => array('weekly')),
    '#process' => array('ctools_dependent_process', 'expand_checkboxes'),
    '#prefix' => '<div id="edit-weekly-days-wrapper">',
    '#suffix' => '</div>',
  );

  $repeat_months_options = array();
  for ($i = 1; $i < 12; $i++) {
    $repeat_months_options[$i] = $i.($i == 1 ? ' month' : ' months');
  }
  $settings_form['repeat']['repeat_months'] = array(
    '#type' => 'select',
    '#title' => t('Repeat every'),
    '#options' => $repeat_months_options,
    '#default_value' => $conf['repeat_months'],
    '#dependency' => array('edit-repeats-type' => array('monthly')),
    '#process' => array('ctools_dependent_process'),
  );
  
  $monthly_type_options = array(
    'month' => t('Day of the month'),
    'week' => t('Day of the week (first of the month)'),
  );
  $settings_form['repeat']['monthly_type'] = array(
    '#type' => 'radios',
    '#title' => t('Repeat by'),
    '#options' => $monthly_type_options,
    '#default_value' => $conf['monthly_type'],
    '#dependency' => array('edit-repeats-type' => array('monthly')),
    '#process' => array('ctools_dependent_process', 'expand_radios'),
    '#prefix' => '<div id="edit-monthly-type-wrapper">',
    '#suffix' => '</div>',
  );

  $settings_form['repeat']['never_ends'] = array(
    '#type' => 'checkbox',
    '#title' => t('Never stop the repetitions'),
    '#default_value' => $conf['never_ends'],
  );

  //@todo : résumé du choix ("summary" Google Calendar)

  $form['settings'] = $settings_form;
  return $form;
}

function wallyextra_planification_ctools_access_settings_after_build($form, $form_state) {
  /*
  ctools_include('dependent');
  ctools_add_js('dependent');

  $form['settings']['from_date']['time']['#dependency'] = array('edit-all-day' => array(0));
  array_unshift($form['settings']['from_date']['time']['#process'], 'ctools_dependent_process');
  $form['settings']['from_date']['time'] = ctools_dependent_process($form['settings']['from_date']['time'], array(), $form_state, $form);
  */
  return $form;
}

function wallyextra_planification_ctools_access_settings_validate($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['from_date'] > $values['to_date']) {
    form_set_error('to_date', t('The "To" date must be greater than the "From" date'));
  }
  
  if ($values['use_recurrency']) {
    switch ($values['repeats_type']) {
      case 'select':
        form_set_error('repeats_type', t('You have to choose a repetition type'));
        break;

      case 'weekly':
        $no_days = TRUE;
        foreach($values['weekly_days'] as $key => $value) {
          if($key === $value) {
            $no_days = FALSE;
            break;
          }
        }
        if($no_days) {
          form_set_error('', t('You have to choose at least one day'));
        }
        break;
    }
  }
}

function wallyextra_planification_ctools_access_settings_submit($form, &$form_state) {
  $values = &$form_state['values'];
  
  $values['settings'] = array(
    'from_date' => $values['from_date'],
    'to_date' => $values['to_date'],
    'all_day' => $values['all_day'],
    'use_recurrency' => $values['use_recurrency'],
    'monthly_type' => $values['monthly_type'],
  );
  
  if ($values['use_recurrency']) {
    $values['settings']['repeats_type'] = $values['repeats_type'];
    $values['settings']['never_ends'] = $values['never_ends'];

    switch ($values['repeats_type']) {
      case 'daily':
        $values['settings']['repeat_days'] = $values['repeat_days'];
        break;

      case 'weekly':
        $values['settings']['repeat_weeks'] = $values['repeat_weeks'];
        $values['settings']['weekly_days'] = array();
        foreach($values['weekly_days'] as $key => $value) {
          if($key === $value) {
            $values['settings']['weekly_days'][] = $value;
          }
        }
        break;

      case 'monthly':
        $values['settings']['repeat_months'] = $values['repeat_months'];
        $values['settings']['monthly_type'] = $values['monthly_type'];
        break;
    }
  }
}

/**
 * Check for access.
 */
function wallyextra_planificatioin_ctools_access_check($conf, $context) {
  $access = FALSE;
  
  $expl_from_date = explode(' ', $conf['from_date']);
  $expl_to_date = explode(' ', $conf['to_date']);
  $from_time = strtotime($conf['from_date']);
  $from_hour = strtotime($expl_from_date[1]);
  $to_time = strtotime($conf['to_date']);
  $to_hour = strtotime($expl_to_date[1]);
  $current_hour = strtotime(date('H:i:s'));
  $delta_years = date('Y') - date('Y', $from_time);

  if ($conf['use_recurrency']) {
    // Following condition means :
    // current time is greater than minimum time AND
    // (current time is less than maximum time OR repetitions never stop) AND
    // ("All day" checked OR current hour is between minimum and maximum hours)
    if (($current_hour > $from_time) && ($current_hour < $to_time || $conf['never_ends']) && ($conf['all_day'] || ($current_hour > $from_hour && $current_hour < $to_hour))) {
      switch ($conf['repeats_type']) {
        case 'daily':
          $delta_days = date('z') - date('z', $from_time) + 365 * $delta_years;
          $access = (($delta_days % $conf['repeat_days']) === 0);
          break;

        case 'weekday':
          $access = (date('N') < 6);
          break;

        case 'weekend':
          $access = (date('N') > 5);
          break;

        case 'weekly':
          $delta_weeks = date('W') - date('W', $from_time) + 52 * $delta_years;
          $access = (in_array(strtolower(date('D')), $conf['weekly_days']) && ($delta_weeks % $conf['repeat_weeks']) === 0);
          break;

        case 'monthly':
          $delta_months = date('n') - date('n', $from_date) + 12 * $delta_years;
          if (($delta_months % $conf['repeat_months']) === 0) {
            switch ($conf['monthly_type']) {
              case 'month':
                $access = (date('j') == date('j', $from_time));
                break;
              case 'week':
                $access = ((date('j') < 7) && (date('j') == date('j', $from_time)));
                break;
            }
          }
          break;
      }
    }
  } elseif($current_hour > $from_time && $current_hour < $to_time) {
    $access = TRUE;
  }

  return $access;
}

/**
 * Provide a summary description based upon the checked subdomains.
 */
function wallyextra_planification_ctools_access_summary($conf, $context) {
  $summary = '';
  $substitutions = array();
  
  $from_time = strtotime($conf['from_date']);
  $to_time = strtotime($conf['to_date']);
  
  $substitutions['!from_date'] = date('d M Y', $from_time);
  $substitutions['!from_hour'] = date('H:i', $from_time);
  $substitutions['!to_date'] = date('d M Y', $to_time);
  $substitutions['!to_hour'] = date('H:i', $to_time);
  
  $summary = 'From !from_date';
  $summary .= $conf['never_ends'] ? ':' : ' to !to_date: ';

  if ($conf['use_recurrency']) {
    switch ($conf['repeats_type']) {
      case 'daily':
        $summary .= $conf['repeat_days'] == 1 ? 'Daily' : 'Every !num_days days';
        $substitutions['!num_days'] = $conf['repeat_days'];
        break;

      case 'weekday':
        $summary .= 'Every weekdays';
        break;

      case 'weekend':
        $summary .= 'Every week-end days';
        break;

      case 'weekly':
        $summary .= $conf['repeat_weeks'] == 1 ? 'Weekly' : 'Every !num_weeks weeks';
        $summary .= ' on !days';

        $weekly_days_options = array(
          'mon' => t('Monday'),
          'tue' => t('Tuesday'),
          'wed' => t('Wednesday'),
          'thu' => t('Thursday'),
          'fri' => t('Friday'),
          'sat' => t('Saterday'),
          'sun' => t('Sunday'),
        );
        $weekly_days = array();
        foreach ($conf['weekly_days'] as $day) {
          $weekly_days[] = $weekly_days_options[$day];
        }

        $substitutions['!num_weeks'] = $conf['repeat_weeks'];
        $substitutions['!days'] = implode(', ', $weekly_days);
        break;

      case 'monthly':
        $summary .= $conf['repeat_months'] == 1 ? ' Monthly' : 'Every !num_months months';
        $summary .= $conf['monthly_type'] == 'month' ? ' on day !day_num' : ' on the first !week_day';

        $substitutions['!num_months'] = $conf['repeat_months'];
        $substitutions['!day_num'] = date('d', $from_time);
        $substitutions['!week_day'] = date('l', $from_time);
        break;
    }

    $summary .= $conf['all_day'] ? '' : ', from !from_hour to !to_hour';
  } else {
    $summary .= $conf['all_day'] ? '' : ' at !from_hour';
    $summary .= ' to !to_date';
    $summary .= $conf['all_day'] ? '' : ' at !to_date';
  }
  
  $summary .= '.';

  return t($summary, $substitutions);
}
