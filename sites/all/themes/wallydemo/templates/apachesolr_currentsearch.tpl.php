<!-- 
Bloc qui affiche le mot recherché + les filtres sélectionnés
class="pane-apachesolr-search-currentsearch" -->
<div class="votre_recherche"><h2>Filtres :&nbsp;</h2>
  <ul>
    <?php 
foreach($links as $link) {
  if(!is_array($link)) echo $link;
}
?>
  </ul>
</div>
