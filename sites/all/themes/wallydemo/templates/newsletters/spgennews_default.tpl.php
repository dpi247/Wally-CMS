<?php 
    $path = drupal_get_path('theme', 'wallydemo');
    $domain = "http://".$_SERVER["HTTP_HOST"]."/";
    $path_image = $domain.$path.'/images/newsletter/';

    $sql = "SELECT did, conf FROM page_manager_handlers WHERE subtask = 'generate_newsletters' ORDER BY weight ASC";
    $q = db_query($sql);
    while ($record = db_fetch_object($q)) {
      $param = unserialize($record->conf);
      $did = $record->did;
      $title = $param["title"];
      $id = "SPGENNEWS_".$did;
      
      
      $news1 = variable_get($id.'_article1', '');
      $articles = array();
      $articles[0] = variable_get($id.'_article2', '');
      $articles[1] = variable_get($id.'_article3', '');
      $articles[2] = variable_get($id.'_article4', '');
      $articles[3] = variable_get($id.'_article5', '');
      $articles[4] = variable_get($id.'_article6', '');
      $articles[5] = variable_get($id.'_article7', '');
      $portfolio_url = variable_get($id.'_galerie_url', '');
      $portfolio_name = variable_get($id.'_galerie_name', '');
      $portfolio_links = variable_get($id.'_galerie_links', '');  
      $video = variable_get($id.'_video', '');
      $video_url = variable_get($id.'_video_url', '');
      $video_title = variable_get($id.'_video_title', '');            
      $buzz = variable_get($id.'_buzz', '');
      
      break;
    }

?>
<tr><td style="height:10px; background-color:#ebebdd;" height="10"></td></tr>
<tr>
  <td>
  	<table width="100%" border="0" cellspacing="0" cellpadding="0">
		<tr>
			<?php 
		    $node = node_load($news1);
      		wallycontenttypes_packagepopulate($node,$op=NULL);
      		$mainstory = $node->field_mainstory_nodes[0];
 			$main_title = $mainstory->title;
 			$chapo = $mainstory->field_textchapo[0]["value"]; 
			$node_path = $domain.drupal_get_path_alias("node/".$node->nid);
            $embeded_objects = $node->field_embededobjects_nodes;
			$photoObject = wallydemo_get_first_photoEmbededObject_from_package($embeded_objects);
			$photoObject_path = $photoObject->field_photofile[0]['filepath'];
		    $photoObject_summary = $photoObject->field_summary[0]['value'];
		    $photoObject_filename = $photoObject->field_photofile[0]["filename"];
		    $explfilepath = explode('/', $photoObject_path);
		    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
		    if (isset($photoObject_path) && $photoObject_size > 0) { ?>
      		<td style="width:300px;">
      			<a href="<?php echo $node_path; ?>" target="_blank">
      			<?php 
      			  $photoObject_img = theme('imagecache', 'article_300x200', $photoObject_filename, $photoObject_summary, $photoObject_summary);
				  $photoObject_img = str_replace(">"," border=\"0\">",$photoObject_img);
      			  print($photoObject_img); 
      			?>
      			</a>
      		</td>
      		<?php } ?>
      		<td style="width:15px;">&nbsp;</td>
      		<td valign="top">
        		<div style="margin-bottom:10px;">
        			<a href="<?php echo $node_path; ?>" target="_blank" style="font-family:Arial, Helvetica;font-size:24px; color:#003366; text-decoration:none; font-weight:bold;">
        				<?php echo $main_title; ?>
        			</a>
        		</div>
        		<div style="font-family:Arial, Helvetica; font-size:13px;">
  					<?php echo $chapo; ?>
  				</div>
			</td>
    	</tr>
 	</table>
    	<hr size="1" noshade="noshade" color="#e0e0d3" style="margin:10px 0" />
        	<table width="100%" border="0" cellspacing="0" cellpadding="0">
            	<tr>
                	<td>
                		<?php foreach ($articles as $key=>$value) { 
                 		    $node = node_load($value);
                      		wallycontenttypes_packagepopulate($node,$op=NULL);
                      		$mainstory = $node->field_mainstory_nodes[0];
                 			$main_title = $mainstory->title;         
                 			$node_path = $domain.drupal_get_path_alias("node/".$node->nid);    	
                		?>
                  		<table width="100%" border="0" cellspacing="0" cellpadding="0">
  							<tr>
  							    <?php 
  							    $embeded_objects = $node->field_embededobjects_nodes;
                      			$photoObject = wallydemo_get_first_photoEmbededObject_from_package($embeded_objects);
                      			$photoObject_path = $photoObject->field_photofile[0]['filepath'];
                      		    $photoObject_summary = $photoObject->field_summary[0]['value'];
                      		    $photoObject_filename = $photoObject->field_photofile[0]["filename"];
                      		    $explfilepath = explode('/', $photoObject_path);
                      		    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
                      		    if (isset($photoObject_path) && $photoObject_size > 0) { ?>
                                <td style="width:180px;">
                                	<a href="<?php echo $node_path; ?>" target="_blank">

                        			<?php 
                        			  $photoObject_img = theme('imagecache', 'unebis_medium_180x120', $photoObject_filename, $photoObject_summary, $photoObject_summary);
									  $photoObject_img = str_replace(">"," border=\"0\">",$photoObject_img);
									  
                        			  print($photoObject_img);
                        			?>
                                	</a>
                                </td>
                                <?php } ?>
                                <td style="width:15px;">&nbsp;</td>
                                <td valign="top">
                                	<a href="<?php echo $node_path; ?>" target="_blank" style="font-family:Arial, Helvetica;font-size:18px; color:#003366; text-decoration:none; font-weight:bold;">
                                		<?php echo $main_title; ?>
                                	</a>
                                </td>
                              </tr>
                  		</table>
                  		<?php
                  		  if($key < (count($articles)-1)) echo '<hr size="1" noshade="noshade" color="#e0e0d3" style="margin:10px 0" />';
                		} ?>
					</td>
                    <td style="width:15px;">&nbsp;</td>
                    <td valign="top" style="width:300px;"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                      <tr>
                        <td style="width:20px;" valign="top"><a href="<?php print $portfolio_links; ?>" target="_blank">
                        <img src="<?php print $path_image; ?>galerie.gif" width="20" height="52" border="0" style="display:block" /></a></td>
                        <td style="width:78px;" valign="top"><a href="<?php print $portfolio_links; ?>" target="_blank"><?php 
						$portfolio_img = theme('imagecache', 'une_small_78x52', $portfolio_url, 'portfolio', 'portfolio');
						$portfolio_img = str_replace(">"," border=\"0\" style=\"display:block\">",$portfolio_img);
						print $portfolio_img; ?></a></td>
                        <td style="width:10px;">&nbsp;</td>
                        <td valign="top"><a href="<?php print $portfolio_links; ?>" target="_blank" style="font-family:Arial, Helvetica; font-size:13px; font-weight:bold; color:#003366; text-decoration:none;"><?php print $portfolio_name; ?></a></td>
                      </tr>    
                    </table>
                  	
                  	<hr size="1" noshade="noshade" color="#e0e0d3" style="margin:10px 0" />
                    <table width="100%" border="0" cellspacing="0" cellpadding="0">
                      <tr>
                        <td style="width:20px;" valign="top"><a href="<?php print $video; ?>" target="_blank"><img src="<?php print $path_image; ?>video.gif" width="20" height="52" border="0" style="display:block" /></a></td>
                        <td align="center" valign="top" style="width:78px; background-color:#000; padding-top:3px;">
                        	<a href="<?php print $video; ?>" target="_blank">
                        		<?php 
								$video_img = theme('imagecache', 'divers_68x45', $video_url, 'video', 'video');
								$video_img = str_replace(">"," border=\"0\" style=\"display:block\">",$video_img);
								print $video_img; ?>
                        	</a>
                        </td>
                        <td style="width:10px;">&nbsp;</td>
                        <td valign="top">
                        	<a href="<?php print $video; ?>" target="_blank" style="font-family:Arial, Helvetica; font-size:13px; font-weight:bold; color:#003366; text-decoration:none;">
                        	<?php print $video_title; ?>
                        	</a>
                        </td>
  
                      </tr>
                    </table>

                    <hr size="1" noshade="noshade" color="#e0e0d3" style="margin:10px 0" />
                    <table width="100%" border="0" cellspacing="0" cellpadding="0">
                      <tr>
                        <?php 
                          $node = node_load($buzz);
                    	  wallycontenttypes_packagepopulate($node,$op=NULL);
                    	  $mainstory = $node->field_mainstory_nodes[0];
               			  $main_title = $mainstory->title;         
               			  $node_path = $domain.drupal_get_path_alias("node/".$node->nid);  
                          
                          $embeded_objects = $node->field_embededobjects_nodes;
                          $photoObject = wallydemo_get_first_photoEmbededObject_from_package($embeded_objects);
                          $photoObject_path = $photoObject->field_photofile[0]['filepath'];
                          $photoObject_summary = $photoObject->field_summary[0]['value'];
                          $photoObject_filename = $photoObject->field_photofile[0]["filename"];
                          $explfilepath = explode('/', $photoObject_path);
                          $photoObject_size = $photoObject->field_photofile[0]['filesize'];
                        ?>
                        <td style="width:20px;" valign="top"><a href="<?php echo $node_path; ?>" target="_blank"><img src="<?php print $path_image; ?>buzz.gif" width="20" height="52" border="0" style="display:block" /></a></td>
                        <?php if (isset($photoObject_path) && $photoObject_size > 0) { ?> 
                        <td style="width:78px;" valign="top">
                        	<a href="<?php echo $node_path; ?>" target="_blank">
                        	<?php 
                        	  $photoObject_img = theme('imagecache', 'une_small_78x52', $photoObject_filename, $photoObject_summary, $photoObject_summary);
							  $photoObject_img = str_replace(">"," border=\"0\" style=\"display:block\">",$photoObject_img);
                        	  print($photoObject_img);
                        	?>
                        	</a>
                        </td>
                        <?php } ?>
                        
                        <td style="width:10px;">&nbsp;</td>
                        <td valign="top"><a href="<?php echo $node_path; ?>" target="_blank" style="font-family:Arial, Helvetica; font-size:13px; font-weight:bold; color:#003366; text-decoration:none;"><?php echo $main_title; ?></a></td>
                      </tr>
                    </table>
                  	<div style="margin:15px 0; height:10px; background-color:#ebebdd;"></div>
                  	<a href='http://ads.sudpresse.be/adclick.php?n=aae01357' target='_blank'><img src='http://ads.sudpresse.be/adview.php?what=zone:437&amp;n=aae01357' border='0' alt=''></a>
                  	<!-- Image Tag (Tag for Images in newsletters only) //TAG for network 927: Rossel // Website: STATIC_FR // Page: nwl_Sudpresse  // Placement: STATIC_FR_nwl_Sudpresse (3457154)  // created at: Oct 4, 2011 3:39:43 PM  -->
                    <!-- <a href="https://secserv.adtech.de/adlink/3.0/927/3457154/0/170/ADTECH;grp=[group];cookie=no;uid=no;" target="_blank"><img src="https://secserv.adtech.de/adserv/3.0/927/3457154/0/170/ADTECH;grp=[group];cookie=no;uid=no" border="0" height="250" width="300" alt="[Alt-Text]"></a> -->
                    <!-- End of Image Tag -->
                  	<div style="margin:15px 0; height:10px; background-color:#ebebdd;"></div></td>
          		</tr>
      		</table>
  		</td>
      </tr>
      <tr>
      	<td align="right"><a href="##UNSUBSCRIBE##" target="_blank" style="font-family:Arial, Helvetica; font-size:13px; color:#003366;">Se d&eacute;sinscrire</a></td>
	  </tr>
    </table>
	</td>
</tr>
</table>
</td>