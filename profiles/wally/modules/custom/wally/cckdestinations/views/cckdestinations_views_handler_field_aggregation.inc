<?php
/**
 * @file
 * Argument handler to filter results by target.
 */

/**
 * Argument handler to filter results by target.
 */
class cckdestinations_views_handler_field_aggregation extends content_handler_field_multiple {

  /*function render($values) {
    //dpm($this);
    $value = '<div> Term : '.taxonomy_get_term($values->{$this->field_alias})->name.'</div>';
    $value .= '<div> Rank : '.$values->node_data_field_destinations_field_destinations_rank.'</div>';
    $value .= '<div> Time rank : '.date('Y-m-d H:i:s', $values->node_data_field_destinations_field_destinations_time_rank).'</div>';
    $value .= '<div> Layout : '.$values->node_data_field_destinations_field_destinations_layout.'</div>';
    $value .= '<div> Target : '.$values->node_data_field_destinations_field_destinations_target.'</div>';
    return $value;
  }*/
}
