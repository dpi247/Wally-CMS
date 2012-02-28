<?php 
  $SPmenutop = str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $SPmenutop);
  print $SPmenutop; 
?>