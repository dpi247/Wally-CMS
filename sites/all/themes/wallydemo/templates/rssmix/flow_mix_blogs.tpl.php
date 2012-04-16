<div class="bloc-01 bloc-02">
	<h2><a href="/blogs/">Nos blogs thématiques</a></h2>
	<div class="inner-bloc">
		<ul>
		<?php
		$c = count($feed);
		$i = 0; 
		foreach ($feed as $f) {
		  $href = $f["ExternalURI"]["value"];
          $title = $f["MainStory"]["Title"]["value"];
		      $src = $f["EmbeddedContent"]["EmbeddedObjects"]["Object"][0]["LocaleImage"]["filepath"];
		      if($src == ""){ $src = "/sites/all/themes/custom_sp/images/default_pic_last-vids.gif"; }          
          $src = theme('imagecache', 'une_small_78x52', $src, $title, $title);
          
          $jourmois =date('d/m', $f["MainStory"]["PostDate"]["value"]);
          $heuremin= date('G', $aaaa).'h'.date('i', $f["MainStory"]["PostDate"]["value"]);
          $hrefB = $f["MainStory"]["SectionUrl"]["value"];
          $nameB = $f["MainStory"]["Section"]["value"];
          if($i == ($c-1)) $class = ' class="last"';
          else $class = '';
          
          print '
    	  	<li'.$class.'>
                <a target="_blank" href="'.$href.'">'.$src.'</a>
                <p class="time right"><span title="">'.$jourmois.'</span> - '.$heuremin.'</p>
                <h3><a href="'.$hrefB.'">'.$nameB.'</a></h3>
                <a target="_blank" href="'.$href.'">'.$title.'</a>	
    	  	</li>';
          $i++;
		}
		unset($feed);
		?>
		</ul>
	</div>
</div>
