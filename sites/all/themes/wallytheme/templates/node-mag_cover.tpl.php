<?php 
foreach($node->files as $file){
	if(isset($file->filepath) && isset($file->filemime))		
	 echo "<object width=\"940\" height=\"500\" data=\"/".$file->filepath."\" type=\"application/x-shockwave-flash\">
    			<param value=\"/".$file->filepath."\" name=\"movie\" />
    			<param value=\"high\" name=\"quality\" />
    			<param value=\"transparent\" name=\"wmode\" />
<h2><a href=\"http://www.soirmag.be\">Soirmag</a></h2>
</object>";
	 
}?>