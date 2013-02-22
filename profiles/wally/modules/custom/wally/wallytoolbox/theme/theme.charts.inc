<?php
/**
 * Returns a google line chart.
 *
 * @param $data
 *   A 2d table of data.
 *
 * @param $title
 * 	 The title of graph.
 * 
 * @param $width
 *   The width of graph.
 *   
 * @param $height
 *   The height of graph.
 *   
 * @param $pointSize
 *   The size of the point in the graph.
 *   
 * @return $output
 * 	 A div where the graph was displayed.
 */
function theme_wallytoolbox_line_chart($data,$title,$width,$height,$pointSize,$AxeXTitle)
{
	$google_api_js = drupal_get_path('module', 'wallytoolbox').'/scripts/googleChartApi.js';
	//include google api js
	drupal_add_js($google_api_js);
	//google api takes datas in json table
	$data = json_encode($data);
	
	$jsCode = ' 
      google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawChart);
      function drawChart() {
        var data = google.visualization.arrayToDataTable('.$data.');

        var options = {
          title: "'.$title.'",
          pointSize: '.$pointSize.',
          width: '.$width.', height: '.$height.',
          hAxis: {title: "'.$AxeXTitle.'"}
        };

        var chart = new google.visualization.LineChart(document.getElementById("chart_div"));
        chart.draw(data, options);
      }';
	
	//include the js code in the page
	drupal_add_js($jsCode, 'inline');
	$output = '<div id="chart_div" style="width: '.$width.'px; height: '.$height.'px;"></div>';
	return $output;
}