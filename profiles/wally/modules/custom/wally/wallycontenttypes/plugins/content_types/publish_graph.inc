<?php 
/**
 * Callback function to supply a graph of published node,article,gallery,poll.
 */
function wallycontenttypes_publish_graph_ctools_content_types() {

	return array(
			'single' => TRUE,
			'title' => t('publish graph'),
			'category' => t('Dashboard'),
			'description' => t('Display a graph of publication statistics.'),
			'defaults' => array(),
	);
}
/**
 * Implementation of content_type_form()
 */
function wallycontenttypes_publish_graph_content_type_edit_form(&$form, &$form_state) {
	
	$options_lines = array(
			'all node'=>'all node',
			'article package'=>'article package',
			'gallery package'=>'gallery package',
			'poll package'=>'poll package',
	);
	$options_time = array(
			'hours' => 'hours of the days',
			'days' => 'days of the month',
	);
	
	$options_size = array(
			'middle' => 'middle',
			'big' => 'big',
	);
	
	$defaultLines = $options_lines;
	
	if($form_state['conf']['times']!=NULL)
	{
		$defaultTime = $form_state['conf']['times'];
	}
	
	if($form_state['conf']['lines']!=NULL)
	{
		$defaultLines = $form_state['conf']['lines'];
	}
	
	if($form_state['conf']['size']!=NULL)
	{
		$defaultSize = $form_state['conf']['size'];
	}
	
	$form['publish_graph_lines'] = array(
			'#type' => 'checkboxes',
			'#title' => 'choose the different data of graph',
			'#options' => $options_lines,
			'#default_value' => $defaultLines,
			);
	
	$form['publish_graph_time'] = array(
				'#type' => 'select',
				'#title' => t('Choose the time range'),
				'#options' => $options_time,
				'#default_value' => $defaultTime,
			);
	
	$form['publish_graph_size'] = array(
			'#type' => 'select',
			'#title' => t('Choose the size of graph'),
			'#options' => $options_size,
			'#default_value' => $defaultSize,
			);
}
/**
 * Implementation of content_type_form_submit()
 */
function wallycontenttypes_publish_graph_content_type_edit_form_submit(&$form, &$form_state) {

	$form_state['conf']['lines'] = $form_state['values']['publish_graph_lines'];
	$form_state['conf']['times'] = $form_state['values']['publish_graph_time'];
	$form_state['conf']['size'] = $form_state['values']['publish_graph_size'];
}

/**
 * Implementation of content_type_render()
 */
function wallycontenttypes_publish_graph_content_type_render($subtype, $conf, $panel_args, $context) {

	$timesRange = $conf['times'];
	$lines = $conf['lines'];
	$size = $conf['size'];
	$j = 1;
	$titleChart = t('node published per '.$timesRange);
	
	$where = "WHERE n.type = '%s' AND n.created BETWEEN %d AND %d GROUP BY created_time";
	
	//size of the graph
	if($size == 'middle')
	{
		$width = 700;
		$height = 400;
	}
	else if($size == 'big')
	{
		$width = 1000;
		$height = 600;
	}
		
	//node published per hours
	if($timesRange == 'hours') 
	{
		$time = time();
		$hours = (date('G',$time));
		$limit_tstamp_inf = $time - ($hours*60*60);
		$limit_tstamp_sup = $time + (1*60*60);
		$nbr_range = 24;
		$AxeXTitle = "Hours of the day";
		
		$sql = "SELECT COUNT(nid) nbr, FROM_UNIXTIME(n.created,'%k') created_time FROM {node} n ";
		$data[0][0] = 'Hours';
		for($i = 0;$i<=$nbr_range;$i++)
		{
			$data[$i][0] = $i;
		}
		
	}
	//node published per days
	else if($timesRange == 'days')
	{
		$month = date('F');
		$time = time();
		$titleChart = t('node published per '.$timesRange.' in '.$month);
		
		//get the numbers of days in the current month
		$nbr_range = date('t',mktime(0, 0, 0, date('m'), 1, date('y')));
		$jour = date('j');
		$limit_tstamp_inf = $time - ($jour*24*60*60);
		$limit_tstamp_sup = $limit_tstamp_inf + ($nbr_range*24*60*60);
		$AxeXTitle = "Days of ".$month;
		
		$sql = "SELECT COUNT(nid) nbr, FROM_UNIXTIME(n.created,'%e') created_time FROM {node} n ";
		
		$data[0][0] = 'Days';
		for($i = 0;$i<=$nbr_range;$i++)
		{
			$data[$i][0] = $i;
		}
		
	}
	
	foreach($lines as $line)
	{
		if($line != '')
		{
			if($line == 'all node')
			{
				//set the legend of the line
				$data[0][$j] = 'all node';

				for($i = 1;$i<=$nbr_range;$i++)
				{
					$data[$i][$j] = 0;
				}
		
				//special where to get all nodes	
				$where_node = "WHERE n.created BETWEEN %d AND %d GROUP BY created_time";
				$result_node = db_query($sql.$where_node,$limit_tstamp_inf,$limit_tstamp_sup);
				
				while($all_node = db_fetch_object($result_node))
				{
					$data[$all_node->created_time][$j] = intval($all_node->nbr);
				}
				
				$j++;
			}
			else if($line == 'article package')
			{
				//set the legend of the line
				$data[0][$j] = 'article package';
				
				for($i = 1;$i<=$nbr_range;$i++)
				{
					$data[$i][$j] = 0;
				}
							
				$type = 'wally_articlepackage';
				$result_article = db_query($sql.$where,$type,$limit_tstamp_inf,$limit_tstamp_sup);
				
				while($article = db_fetch_object($result_article))
				{
					$data[$article->created_time][$j] = intval($article->nbr);
				}
				$j++;
			}
			else if($line == 'gallery package')
			{
				//set the legend of the line
				$data[0][$j] = 'gallery package';
				
				for($i = 1;$i<=$nbr_range;$i++)
				{
					$data[$i][$j] = 0;
				}
						
				$type = 'wally_gallerypackage';
				$result_gallery = db_query($sql.$where,$type,$limit_tstamp_inf,$limit_tstamp_sup);
			
				while($gallery = db_fetch_object($result_gallery))
				{
					$data[$gallery->created_time][$j] = intval($gallery->nbr);
				}
				$j++;
			}
			else if($line == 'poll package')
			{
				//set the legend of the line
				$data[0][$j] = 'poll package';
				
				for($i = 1;$i<=$nbr_range;$i++)
				{
					$data[$i][$j] = 0;
				}
							
				$type = 'wally_pollpackage';
				$result_poll = db_query($sql.$where,$type,$limit_tstamp_inf,$limit_tstamp_sup);
							
				while($poll = db_fetch_object($result_poll))
				{
					$data[$poll->created_time][$j] = intval($poll->nbr);
				}
				$j++;
			
			}
							
		}
	
	}

	$block = new stdClass();
	$block->title = 'publish graph';
	
	//call theme function wallytoolbox_line_chart
	$block->content = theme('wallytoolbox_line_chart',$data,$titleChart,$width,$height,3,$AxeXTitle);
	
	return $block;
}