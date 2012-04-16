<div class="bloc-01 bloc-02">
	<h2><a href="/blogs/">Nos blogs thématiques</a></h2>
	<div class="inner-bloc">
		<ul>
		<?php
		$theme_path = $_SERVER['DOCUMENT_ROOT'].'/'.drupal_get_path('theme','custom_sp').'/images';
		$c = count($feed);
		$i = 0; 
		foreach ($feed as $f) {
		  $href = $f["ExternalURI"]["value"];
          $title = $f["MainStory"]["Title"]["value"];
		      $src = $f["EmbeddedContent"]["EmbeddedObjects"]["Object"][0]["LocaleImage"]["filepath"];
		      if($src == ""){ $src = $theme_path."/default_pic_blogs.gif"; }
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
                <p class="titre_bloc-02"><a target="_blank" href="'.$href.'">'.$title.'</a></p>	
    	  	</li>';
          $i++;
          unset($f);
		}
		unset($feed);
		?>
		</ul>
	</div>
</div>
