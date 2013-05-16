<?php

/**
 * Returns a google line chart.
 * see https://google-developers.appspot.com/chart/interactive/docs/gallery/linechart
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
function theme_wallytoolbox_line_chart($data,$title,$pointSize,$AxeXTitle) {
  static $i = 0;

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
          hAxis: {title: "'.$AxeXTitle.'"}
        };

        var chart = new google.visualization.LineChart(document.getElementById("chart_div_'.$i.'"));
        chart.draw(data, options);
      }';

	//include the js code in the page
	drupal_add_js($jsCode, 'inline');
	$output = '<div id="chart_div_'.$i.'" style="width: 100%;"></div>';

	$i++;

	return $output;
}

/**
 * Returns a google pie chart.
 * see https://google-developers.appspot.com/chart/interactive/docs/gallery/piechart
 *
 * Example:
 *  $data=array(
 *    array("DDU", 20000),
 *    array("ODM", 22000),
 *    array("ASL", 18000),
 *    array("JPL", 8000),
 *  );
 *  $vars['pie']=theme("wallytoolbox_pie_chart",$data,"Number of commit","Dev name","commits","400","400");
 *
 * @param $data
 *   An array each element is a  2d (string, integer) array.
 *
 * @param $title
 * 	 The title of graph.
 *
 * @param $header_name
 * 	 The name of the value.
 *
 * @param $header_unit
 * 	 The unit of the value
 *
 * @param $width
 *   The width of graph.
 *
 * @param $height
 *   The height of graph.
 *
 *
 * @return $output
 * 	 A div where the graph was displayed.
 */
function theme_wallytoolbox_pie_chart($data,$title,$header_name,$header_unit) {
  static $i = 0;

  $google_api_js = drupal_get_path('module', 'wallytoolbox').'/scripts/googleChartApi.js';
  drupal_add_js($google_api_js);

  $data = json_encode($data);
  $jsCode = '
    // Load the Visualization API and the piechart package.
      google.load("visualization", "1.0", {"packages":["corechart"]});

      // Set a callback to run when the Google Visualization API is loaded.
      google.setOnLoadCallback(drawChart);
      // Callback that creates and populates a data table,
      // instantiates the pie chart, passes in the data and
      // draws it.
      function drawChart() {
        // var data = google.visualization.arrayToDataTable('.$data.');
        // Set chart options
        var data = new google.visualization.DataTable();
        data.addColumn("string", "'.$header_name.'");
        data.addColumn("number", "'.$header_unit.'");
        data.addRows('.$data.');
        var options = {"title":"'.$title.'",
        };
        var chart = new google.visualization.PieChart(document.getElementById("piechart_div_'.$i.'"));
        chart.draw(data, options);
      }';

  //include the js code in the page
  drupal_add_js($jsCode, 'inline');
  $output = '<div id="piechart_div_'.$i.'" style="width: '.$width.'px; height: '.$height.'px;"></div>';

  $i++;

  return $output;
}

/**
 * Returns a google bar chart.
 * see https://google-developers.appspot.com/chart/interactive/docs/gallery/barchart#Example
 *
 * Example:
 *  $data=array(
 *    array("Month", "DDU","ODM","ASL","JPL"),
 *    array("Sept", 4786, 4356, 2345, 000),
 *    array("Oct", 3456, 4321, 3456, 000),
 *    array("Nov", 2134, 6543, 4563, 2000),
 *  );
 *  $vars['bar']=theme("wallytoolbox_bar_chart",$data,"Number of commit","commits","Dev name","400","400");
 *
 * @param $data
 *   An matrix.
 *   The first line are the labels
 *   The first row represent how data are group together.
 *   Each other row represent a category of data
 *
 * @param $title
 * 	 The title of graph.
 *
 * @param $vAxis
 * 	 Vertical Axis Label
 *
 * @param $hAxis
 * 	 Horizontal Axis Label
 *
 * @param $width
 *   The width of graph.
 *
 * @param $height
 *   The height of graph.
 *
 *
 * @return $output
 * 	 A div where the graph was displayed.
 */
function theme_wallytoolbox_bar_chart($data,$title,$hAxis,$vAxis){
  static $i = 0;

  $google_api_js = drupal_get_path('module', 'wallytoolbox').'/scripts/googleChartApi.js';
  drupal_add_js($google_api_js);

  $data = json_encode($data);
  $jsCode = '
    // Load the Visualization API and the piechart package.
      google.load("visualization", "1.0", {"packages":["corechart"]});

      // Set a callback to run when the Google Visualization API is loaded.
      google.setOnLoadCallback(drawChart);
      // Callback that creates and populates a data table,
      // instantiates the pie chart, passes in the data and
      // draws it.
      function drawChart() {
        // var data = google.visualization.arrayToDataTable('.$data.');
        // Set chart options
        var data = google.visualization.arrayToDataTable('.$data.');
        var options = {"title":"'.$title.'",
                        vAxis: {title: "'.$vAxis.'"},
                        hAxis: {title: "'.$hAxis.'"},
        };
        var chart = new google.visualization.BarChart(document.getElementById("barchart_div_'.$i.'"));
        chart.draw(data, options);
      }';

  //include the js code in the page
  drupal_add_js($jsCode, 'inline');
  $output = '<div id="barchart_div_'.$i.'" ></div>';

  $i++;

  return $output;
}

/**
 * Returns a gauge bar chart.
 * see https://google-developers.appspot.com/chart/interactive/docs/gallery/barchart#Example
 *
 * Example:
 *  $data=array(
 *    array("ammount of commit", "DDU","ODM","ASL","JPL"),
 *    array("In percent", 30, 30, 30, 10),
 *  );
 *  $vars['bar']=theme("wallytoolbox_bar_chart",$data,"Number of commit","commits","Dev name","400","400");
 *
 * @param $data
 *   An matrix.
 *   The first line are the labels
 *   The second line is the percentage:  *TOTAL SHOULD BE 100 ! On only one line
 *   No third line
 *
 * @param $title
 * 	 The title of graph.
 *
 * @param $vAxis
 * 	 Vertical Axis Label
 *
 * @param $hAxis
 * 	 Horizontal Axis Label
 *
 * @param $width
 *   The width of graph.
 *
 * @param $height
 *   The height of graph.
 *
 *
 * @return $output
 * 	 A div where the graph was displayed.
 */
function theme_wallytoolbox_gauge_chart($data,$title,$hAxis,$vAxis){
  static $i = 0;

  $google_api_js = drupal_get_path('module', 'wallytoolbox').'/scripts/googleChartApi.js';
  drupal_add_js($google_api_js);

  $data = json_encode($data);

  $jsCode = '
    // Load the Visualization API and the piechart package.
      google.load("visualization", "1.0", {"packages":["corechart"]});

      // Set a callback to run when the Google Visualization API is loaded.
      google.setOnLoadCallback(drawChart);
      // Callback that creates and populates a data table,
      // instantiates the pie chart, passes in the data and
      // draws it.
      function drawChart() {
    
    var data = google.visualization.arrayToDataTable('.$data.');
    var options = {
        title:"'.$title.'",
        vAxis: {title: "'.$vAxis.'"},
        hAxis: {title: "'.$hAxis.'",maxValue:"100"},
        isStacked: "True"
  
    }
     var chart = new google.visualization.BarChart(document.getElementById("gaugechart_div_'.$i.'"));
        chart.draw(data, options);
      }';

  //include the js code in the page
  drupal_add_js($jsCode, 'inline');
  $output = '<div id="gaugechart_div_'.$i.'" ></div>';

  $i++;

  return $output;
}
