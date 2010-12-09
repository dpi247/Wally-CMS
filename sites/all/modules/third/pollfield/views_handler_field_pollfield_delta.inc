<?php
// $Id: views_handler_field_pollfield_delta.inc,v 1.1.2.1 2010/02/06 01:12:40 marioprkos Exp $

/**
 * Field handler to translate a node type into its readable form.
 */

class views_handler_field_pollfield_delta extends views_handler_field {
  function render($values) {
   
    $number=$values->pollfield_votes_delta;
		$nid=$values->pollfield_votes_nid;
    $field_name_delta=$values->pollfield_votes_field_name_delta;
    $field_name=$values->pollfield_votes_field_name;

		//get table information
		$fields = content_fields();
    $db_info = content_database_info($fields[$field_name]);
    $table_content = $db_info['table'];
    $column_choice = $field_name ."_choice";
    $column_question = $field_name ."_question";
    $column = 'delta';
    $is_delta_exist = db_column_exists($table_content, $column);
		//get question and choices
    if (!$is_delta_exist) {
       
			 $poll_choices=db_result(db_query("SELECT %s FROM {%s} WHERE nid=%d",
															$column_choice,$table_content,$nid));
    }
		else{
			 $poll_choices=db_result(db_query("SELECT %s FROM {%s} WHERE nid=%d AND delta=%d",
															$column_choice,$table_content,$nid,$field_name_delta));
			
		}
		$choices=unserialize($poll_choices);
		
    
    return $choices[$number]['choice'];
  }
}

