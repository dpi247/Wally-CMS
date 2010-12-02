<?php

  global $user;

  $path = base_path() . path_to_theme();
  $add_html = ""; 
  $html_class = "";
  
  switch ($zone) {
    case 'ad_01.gif':
    case 'ad_02.gif':
    case 'ad_03.gif':
      $width='214';
      $height='60';
      $img= $path."/mediastore/temp/".$zone;
      break;
    case 'leaderboard.jpg';
      $width='728';
      $height='90';
      $img= $path."/mediastore/temp/".$zone;
      break;
    
  }
  $html_class = str_replace(".","",$zone);  
  $add_html = "<a class='".$html_class."_heading' href='javascript:void(0)'><img src='".$img."' width='".$width."' height='".$height."' alt='publicite' /></a>";

// some info if needed to populate adds with specials infos. 
// $node & $user are available.
//
$display = array(); 

if (isset($user)) $display[] = "name:".$user->name;
if (isset($user)) $display[] = "email:".$user->mail;
if (isset($node)) $display[] = "Node title:".$node->title;

if (isset($node->field_embededobjects_nodes)) $display[] = "Fil ariane:".theme("soirmag_fil_ariane", $node->field_embededobjects_nodes, $node);
if (isset($node->field_free_tags)) $display[] = "free tags:".theme("wallyct_taxotermlist",$node->field_free_tags, $node);
if (isset($node->field_tags)) $display[] = "categorized tags: :".theme("wallyct_taxotermlist",$node->field_tags, $node);

?>

<script type="text/javascript">
    $(document).ready(function() {
      jQuery('.<?php print $html_class; ?>_content').hide();
      jQuery('.<?php print $html_class; ?>_heading').click(function()
      {
        jQuery(this).next('.<?php print $html_class; ?>_content').slideToggle(500);
      });
    });
</script>
<div class="<?php print $html_class; ?>_layer" style="width:<?php print $width; ?>px;">
<?php print $add_html; ?>
<div class="<?php print $html_class; ?>_content" style="background:white;">
<?php
foreach($display as $line) {
  print $line."<br/>"; 
}
?>
</div>
</div>

