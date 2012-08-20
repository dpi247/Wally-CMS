<?php 
$sp_footer = variable_get('site_footer',NULL);
$path = drupal_get_path('theme', 'wallydemo');
?>
<div class="clearfix" id="liste_logos"><img src="/<?php print $path; ?>/images/logos_tous.gif" alt="La Meuse, La Nouvelle Gazette, La Province, Nord Eclair,  La Capitale, Sudpresse" width="900" height="96" border="0" usemap="#Map" />
      <map name="Map" id="Map">
        <area shape="rect" coords="21,19,156,49" href="http://www.lameuse.be" title="Édition La Meuse de Sudpresse" />
        <area shape="rect" coords="173,19,325,49" href="http://www.lanouvellegazette.be" title="Édition La Nouvelle Gazette de Sudpresse" />
        <area shape="rect" coords="343,19,512,49" href="http://www.laprovince.be" title="Édition La Province de Sudpresse" />
        <area shape="rect" coords="529,19,691,49" href="http://www.nordeclair.be" title="Édition Nord Eclair de Sudpresse" />
        <area shape="rect" coords="708,19,868,49" href="http://www.lacapitale.be" title="Édition La Capitale de Sudpresse" />

        <area shape="rect" coords="781,63,898,86" href="http://www.sudpresse.be" title="Sudpresse" />
      </map>
</div>
<p class="footer">
<?php 
print($sp_footer);
?>
</p>
<p class="rossel">
  <a href="http://www.rosseladvertising.be/" target="_blank" title="Rossel Advertising"><img src="/<?php print $path; ?>/images/logos/rad.gif" alt="Rossel Advertising" title="Rossel Advertising" width="167" height="55" border="0" />
  </a>
</p>
