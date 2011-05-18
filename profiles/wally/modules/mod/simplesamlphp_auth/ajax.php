<?

require('json.php');
$url = "http://lkroad65.rossel.be:8080/road65-apis/adressNormalizer?street=".urlencode($_GET["rue"])."&zipcode=".urlencode($_GET["cp"])."&city=".urlencode($_GET["ville"])."&number=".urlencode($_GET["numero"])."&lang=F&full_proc=1&authKey=6FA2F702334D06AAB886EA9114";
$rest = file_get_contents($url);
if(count($rest)) {
	$j = new Services_JSON;
	$json = $j->decode($rest); 
	$adresses = array();
	foreach($json as $r) {
		array_push($adresses,$r[0]);
	}
}
echo($rest);

?>