<?php  
$tid  = $variables["view"]->args[0];
$dest = taxonomy_get_term($tid);
$dest_name = str_replace('Accueil ','',$dest->name);
switch($tid){
	case 1:
		$url = "http://www.lameuse.be";
		break;
	case 2:
		$url = "http://www.lanouvellegazette";
		break;
	case 3:
	  $url = "http://www.sudinfo.be";
	  break;
	case 4:
		$url = "http://www.nordeclair.be";
		break;
	case 5:
		$url = "http://www.laprovince.be";
		break;
	case 6:
		$url = "http://www.lacapitale.be";
		break;	
}
?>
<div class="bloc-01 bloc-02">
  <h2><a href="<?php print $url ; ?>">En ce moment sur <?php print $dest_name;  ?></a></h2>
  <div class="inner-bloc">
    <ul><?php foreach ($rows as $id => $row): ?>
    <?php print $row; ?>
    <?php endforeach; ?></ul>
  </div>
</div>
