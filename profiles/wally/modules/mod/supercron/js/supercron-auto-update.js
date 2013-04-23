// $Id: supercron-auto-update.js,v 1.1 2010/03/18 23:25:59 63reasons Exp $

Drupal.SuperCronStatus = function() {
    var superCronStatusDetails = function(data) {
        var result= Drupal.parseJson(data);
        if (result.modules) {
            for ( var i=0, len=result.modules.length; i<len; ++i ){
                $('#' + result.modules[i].timestamp_formatted_json_id).html(result.modules[i].timestamp_formatted);
                $('#' + result.modules[i].execution_time_formatted_json_id).html(result.modules[i].execution_time_formatted);
                $('#' + result.modules[i].average_execution_time_formatted_json_id).html(result.modules[i].average_execution_time_formatted);
                $('#' + result.modules[i].output_link_json_id).html(result.modules[i].output_link);
                $('#' + result.modules[i].exception_link_json_id).html(result.modules[i].exception_link);
            }
        }
        $('#supercron-lower-warning').html(result.lower_warning);
    }
  
    $.get(path_to_supercron_status_callback, null, superCronStatusDetails);

    return false;
}

$(document).ready(function(){
    if (Drupal.jsEnabled) {
        $(document).ready(setInterval('Drupal.SuperCronStatus()',1000));
    }
});