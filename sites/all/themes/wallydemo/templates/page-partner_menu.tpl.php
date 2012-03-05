<?php 
echo 'julien coucou';  
$content = $content;   	
  $content =  str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $content);
  $content =  str_replace('src="/', 'src="http://'.$_SERVER['SERVER_NAME'].'/', $content);
  print $content; 
?>