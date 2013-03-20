<?php 
//use the namespace of symfony/yaml library
use Symfony\Component\Yaml\Yaml;

/**
 * 
 * Import yaml file form
 * 
 * @param $form_state
 * @return $form
 */
function wallyimport_destination_form($form_state){
	
	//get taxonomy vocabulary
	$options = wallyimport_destination_get_vocabulary();
	
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	
	$form['wallyimport_destination_yaml_file'] = array(
			'#type' => 'file',
			'#title' => t('your yaml file destination'),
			'#size' => 40,
			);

	$form['wallyimport_destination_vid'] = array(
			'#type' => 'select',
			'#title' => t('choose the vocabulary destination'),
			'#options' => $options, 
			'#default_value' => 2,	
			);
	
	$form['wallyimport_destination_submit'] = array(
			'#type' => 'submit',
			'#value' => 'upload',
			);

	return $form;
}

/**
 * 
 * this function get the taxonomy vocabulary
 * 
 * @return array
 * return vocabulary
 */
function wallyimport_destination_get_vocabulary(){
	
	//select just vid and name
	$results = db_query(db_rewrite_sql("SELECT vid, name FROM {vocabulary}"));
	
	while($result = db_fetch_object($results))
	{
		$voc[$result->vid] = $result->name;
	}
	return $voc;
}

/**
 * 
 * Import yaml file submit form
 * 
 * @param $form
 * @param $form_state
 */
function wallyimport_destination_form_submit($form,$form_state){
	
	//destination by default : /tmp/
	$dest = '';
	$validators = array(
			'file_validate_extensions' => array("yaml txt"),
	);
	
	$file = file_save_upload('wallyimport_destination_yaml_file',$validators,$dest,FILE_EXISTS_REPLACE);
	
	$vid = $form_state['values']['wallyimport_destination_vid'];
	
	if(file_exists($file->filepath))
	{
		//read the yaml file
		$data = wallyimport_destination_read_yaml_file($file);
		//parse the txt file from the txt file
		$data = wallyimport_destination_parse_txtToYaml($data,$vid);
		if(empty($data))
		{
			drupal_set_message(t("you have specified a hierarchy for the authors vocabulary!
					 The authors vocabulary doesn't takes hierarchy's"),'error');
		}
		else
		{
			//parse the yaml data from the file with the symfony/yaml library
			$data = wallyimport_destination_parse_yaml($data);
			
			wallyimport_destination($data,$vid);
		}
		
	}
	else 
	{
		drupal_set_message(t('you must provided a yaml file !'),'error');
	}
	
}

/**
 * 
 * this function read the yaml file and return the content
 * 
 * @param $file
 * the uploaded file
 * @return string
 * return the content of the file
 */
function wallyimport_destination_read_yaml_file($file){
	
	$FileName = $file->filepath;
	$FileHandle = fopen($FileName, 'r');
	
	$data = fread($FileHandle, filesize($FileName));
	fclose($FileHandle);
	
	//delete the file after reading
	unlink($FileName);

	return $data;
}

/**
 * 
 * this function parse txt format in yaml format
 * 
 * @param $txt
 * the txt data from the file
 * @return string
 * return the yaml
 */
function wallyimport_destination_parse_txtToYaml($txt,$vid){
	
	//cut the txt in array for each line
	$txt = explode("\n", $txt);
	
	$is_hierarchique = false;
	//delete empty index
	$txt = wallyimport_destination_del_empty_index($txt);
	
	$txt[0] = str_replace("\t", " ", $txt[0]);
	//max number of space
	$tempMax = wallyimport_destination_position_firstLetter($txt[0]);
	
	if(!empty($txt[0]))
	{
		$txt[0] = $txt[0].':';
	}
	
	for($i=1;$i<count($txt);$i++)
	{
		/*replace tabulation by space because the tabulations 
		   is not supported by the Yaml parseur*/ 
		$txt[$i] = str_replace("\t", " ", $txt[$i]);
		
		if(!empty($txt[$i]))
		{
			$txt[$i] = $txt[$i].':';
		}
		
		$tempMaxCurrent = wallyimport_destination_position_firstLetter($txt[$i]);
		
		if($tempMax >= $tempMaxCurrent)
		{
			if(!empty($txt[$i-1]))
			{
				$txt[$i-1] = $txt[$i-1].' 0';
			}
		}
		//new tempMax
		$tempMax = $tempMaxCurrent;
		
		if($tempMax > 0)
		{
			$is_hierarchique = true;
		}
	}
	//convert the array to string, each line separate by a carrige return
	$txt = implode("\n", $txt);
	
	//if it's the authors vocabulary
	if($is_hierarchique && $vid == 1)
	{
		return '';
	}
	else
	{
		return $txt;
	}
}

/**
 * 
 * this function delete empty index in an array and reorder it
 * 
 * @param $txt
 * the array
 * @return array
 */
function wallyimport_destination_del_empty_index($txt){

	for($i=1;$i<count($txt)-1;$i++)
	{
	
		$temp = trim($txt[$i]);
		$txt[$i] = rtrim($txt[$i]);
		
		if(empty($temp))
		{
			//unset the empty index
			unset($txt[$i]);
			//reorder the array
			$txt = array_merge($txt);
		}
	}
	return $txt;
}

/**
 * 
 * this function count the numbers of spaces before the first letter
 * 
 * @param $str
 * the string
 * @return number
 * return the numbers of spaces before the first letter
 */
function wallyimport_destination_position_firstLetter($str){
	$long = strlen($str);
	//browse the string character by character
	for($i=0;$i<$long;$i++)
	{
		//if the character is a not a space
		if($str{$i} != ' ')
		{
			return $i;
		}
	}
	
}

/**
 * 
 * this function parse the yaml and return an array
 * 
 * @param $yaml
 * the yaml string from the file
 * @return $data
 * return an array
 */
function wallyimport_destination_parse_yaml($yaml){
	
	//autoload.php load all yaml library file
	require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/wallyimport_destination.autoload.php');
	
	try 
	{
		//try to parse the yaml data
		$data = Yaml::parse($yaml);
	}
	catch (Exception $e)
	{
		//if an error is happen during the parse
		$e = explode('.', $e);
		drupal_set_message(t('An error is happen %s' , array('%s' => $e[0])),'error');
	}
	
	return $data;
}

/**
 * 
 * this function import destination from an array
 * 
 * @param $tabs
 * the array generate by wallyextra_parse_yaml() function
 * @param $vid
 * id of vocabulary
 */
function wallyimport_destination($tabs,$vid){
	
	//load taxonomy module core to insert destination in database
	module_load_include('module', 'taxonomy','taxonomy');

	foreach ($tabs as $key=>$value)
	{
		//if the term is a root term
		if($value == 0)
		{
			//test si vid = 0 et si name est vide
			$tree = taxonomy_get_tree($vid);
			
			wallyimport_destination_save_root_term($key,$tree,$vid);
		}
		else
		{
			//add child term to his parents
			$tabParents[0]='root';
			wallyimport_destination_add_term($value,$key,'root',$vid,$tabParents);
		}
		
	}

}

/**
 * 
 * this function verify if the root term exist and add it
 * 
 * @param $Nameterm
 * the name of the root term from the yaml
 * @param $tree
 * the term tree from db
 * @param $vid
 * id of vocabulary 
 */
function wallyimport_destination_save_root_term($Nameterm,$tree,$vid){
	
	$term  = wallyimport_destination_create_termArray($Nameterm, $vid,array());

	$isPresent = false;
	foreach($tree as $branch)
	{
		//see in all term which haven't parents
		if($branch->parents[0] == 0)
		{
			//and if the term exist
			if($branch->name == $term['name'])
			{
				$isPresent = true;
			}
		}
	}

	//if the term is not present in the tree, add it
	if(!$isPresent)
	{
		//add the term in database
		$status = taxonomy_save_term($term);
		drupal_set_message(t("the term : %s has been added",array('%s'=>$term['name'])));
	}
	else
	{
		drupal_set_message(t("the term : %s exist and hasn't been added",
									array('%s'=>$term['name'])),'warning');
	}
	
	$isPresent = false;
}

/**
 * 
 * this recursive function browse the term tree
 * 
 * @param $child
 * the child of parent
 * @param $parent
 * the parent
 * @param $parentOfParent
 * the parent of parent
 * @param $vid
 * id of vocabulary
 * @param $tabParents
 * all ancestors name of the term
 */
function wallyimport_destination_add_term($child,$parent,$parentOfParent,$vid,$tabParents){
	
	if($parentOfParent == 'root')
	{
		$tree = taxonomy_get_tree($vid);
		wallyimport_destination_save_root_term($parent,$tree,$vid);
	}
	//if $child is an array
	if(is_array($child))
	{
		foreach ($child as $key=>$value)
		{
			//if $value is an array
			if(is_array($value))
			{
				$i = count($tabParents);
				//construct an array with all parents tree of the term added
				if($tabParents[$i-1] != $parent)
				{
					$tabParents[$i++] = $parent;
				}
				
				//retrieve the good parent id
				$parentTid = wallyimport_destination_find_tid_parent($parentOfParent,$vid,$tabParents);
				//create the term array format
				$term = wallyimport_destination_create_termArray($key,$vid,$parentTid);
				//save the child term
				wallyimport_destination_save_child_term($term,$vid);
				//recursive function call itself
				wallyimport_destination_add_term($value,$key,$parent,$vid,$tabParents);
			}
			else
			{
				$i = count($tabParents);
				//construct an array with all parents tree of the term added
				if($tabParents[$i-1] != $parent)
				{
					$tabParents[$i++] = $parent;
				}
				//retrieve the good parent id
				$parentTid = wallyimport_destination_find_tid_parent($parentOfParent,$vid,$tabParents);
				//create the term array format
				$term = wallyimport_destination_create_termArray($key,$vid,$parentTid);
				//save the last child term in the tree
				wallyimport_destination_save_child_term($term,$vid);
			}
		}
	}
	
}

/**
 * 
 * this function verify if the child term exist and add it to her parent
 * 
 * @param $term
 * the child term to save in database
 */
function wallyimport_destination_save_child_term($term,$vid){
	
	$isPresent = false;
	
	 //for each parent of the term
	for($i=0;$i<count($term['parent']);$i++)
	{
		//get all children from the current parent
		$children = taxonomy_get_children($term['parent'][$i],$vid);
		
		foreach ($children as $child)
		{	
			//if the term exist in his parent
			if($child->name == $term['name'])
			{
				$isPresent = true;
			}
		}
		
		//if the term is not present in his parent, add it
		if(!$isPresent)
		{
			//add the term in db
			$status = taxonomy_save_term($term);
			
			$parent = taxonomy_get_term($term['parent'][$i]);
			
			drupal_set_message(t("the term : %s has been added to his parent : %sp",
					array('%s'=>$term['name'],'%sp'=>$parent->name)));
		}
		else
		{
			//else the term is already present in his parent
			$parent = taxonomy_get_term($term['parent'][$i]);
			
			drupal_set_message(t("the term : %s exist in his parent : %sp and hasn't been added",
					array('%s'=>$term['name'],'%sp'=>$parent->name)),'warning');
		}
		
		$isPresent = false;
	}
}

/**
 * 
 * this function find the good parent tid of the term
 * 
 * @param $parentName
 * the parent name of the term
 * @param $parentOfParent
 * the parent of parent name of the term
 * @param $tabParents
 * all ancestors name of the term
 */
function wallyimport_destination_find_tid_parent($parentOfParent,$vid,$tabParents){
	
	$countParent = count($tabParents)-1;

	//retrieve parents by name
	$tabTerm = taxonomy_get_term_by_name($tabParents[$countParent]);
	
	//reverse order of tabParents
	$tabParents = array_reverse($tabParents);
	
	for($i=0;$i<count($tabTerm);$i++)
	{
		//if the parent of parent root
		if($parentOfParent == 'root')
		{
			//if it is the same vid vocabulary
			if($tabTerm[$i]->vid == $vid)
			{
				return $tabTerm[$i]->tid;
			}
		}
		else
		{
			//if it is the same vid vocabulary
			if($tabTerm[$i]->vid == $vid)
			{
				$isParent = wallyimport_destination_is_parent($tabTerm[$i]->tid,$tabParents);

				if($isParent)
				{
					return $tabTerm[$i]->tid;	
				}
			}
			
		}
	}
}

/**
 * 
 * this function verify if all ancestor correspond
 * 
 * @param $tabTermTid
 * tid of the parent
 * @param $tabParents
 * all ancestor of the term
 * @return boolean
 */
function wallyimport_destination_is_parent($tabTermTid,$tabParents){
	
	//get all parents of term parent
	$AllParent = taxonomy_get_parents_all($tabTermTid);
	
	for($j=0;$j<count($AllParent);$j++)
	{
		if($tabParents[$j] == $AllParent[$j]->name)
		{
			$isParent = true;
		}
		else
		{
			$isParent = false;
			//break the for because isn't the good parent
			break;
		}
	}
	
	return $isParent;
}

/**
 * 
 * this function create a term array for the function taxonomy_save_term($term)
 * 
 * @param $termName
 * the name of the term
 * @param $vid
 * id of the vocabulary
 * @param $parentTid
 * if the term has a parent
 * @return $term
 * return the term array
 */
function wallyimport_destination_create_termArray($termName,$vid,$parentTid){
	
	//if it is a root term
	if(empty($parentTid))
	{
		$term = array(
				'name' => $termName,
				'vid' => $vid,
				'weight' => 0,
				'parent' => array(),
		);
	}
	else
	{
		$parent[0] = $parentTid;
		$term = array(
				'name' => $termName,
				'vid' => $vid,
				'weight' => 0,
				'parent' => $parent,
		);
	}
	return $term;
}