<?php 
  $content = $content;   	
  $content =  str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $content);
  $content =  str_replace('src="/', 'src="http://'.$_SERVER['SERVER_NAME'].'/', $content);
  	
  $SPfooter =  str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $SPfooter);
  $SPfooter =  str_replace('src="/', 'src="http://'.$_SERVER['SERVER_NAME'].'/', $SPfooter);

  $contentF = theme("sp_footer", $data_array);   	
  $contentF =  str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $contentF);
  $contentF =  str_replace('src="/', 'src="http://'.$_SERVER['SERVER_NAME'].'/', $contentF);
?>
  <?php print $content; ?> <!-- Affiche le bloc "Nos rubriques" -->
  <div id="sp_footer"> <?php print $SPfooter; ?><? print $contentF; ?></div>
