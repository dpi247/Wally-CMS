<?php
/**
 *
 * 
 * 
 */
?> 
<h1>GENERIC NODE TEMPLATE FOR "wally_articlepackage" LAYOUT "article"</h1>
<hr/>


<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

<?php print theme("wallyct_authorslist",$node); ?>


<div id="main-content">
<div id=authors></div>
</div>



