<?php
$path = drupal_get_path('theme', 'custom_soirmag');

// Préparation des variables.
$sm_footer = $data_array["footer_message"];
?>
    <div id="wrap-footer">
      <div id="footer">
        <div class="container_12 clearfix">
          <div class="return-top">
            <a href="#header" title="Retour en haut de la page"><img src="/<?php print $path; ?>/mediastore/elements/fleche_top.gif" alt="Haut" /></a>
          </div>
          <div class="grid_2">
            <img src="/<?php print $path; ?>/mediastore/elements/logo_footer.gif" alt="Soirmag.be" width="123" height="23" />
            <ul>
              <li><a href="javascript:void(0)">TV News</a></li>
              <li><a href="javascript:void(0)">Stars</a></li>
              <li><a href="javascript:void(0)">Actu</a></li>
              <li><a href="javascript:void(0)">Toute l'actu télé</a></li>
              <li><a href="javascript:void(0)">Photos</a></li>
              <li><a href="javascript:void(0)">Videos</a></li>
            </ul>
            <strong>Blogs :</strong>
            <ul>
              <li><a href="javascript:void(0)">Téléréalité</a></li>
              <li><a href="javascript:void(0)">Séries</a></li>
              <li><a href="javascript:void(0)">Royal</a></li>
            </ul>
          </div>
          <div class="grid_7">
            <? print theme("soirmag_search_box"); ?>
            
            <div class="bloc">
              <strong>Sites du Groupe <em>Rossel</em> :</strong>
              <ul>
                <li><a href="http://www.vlan.be/" title="Aller sur le site Vlan.be (fenêtre externe)" class="ext">Vlan.be</a></li>
                <li><a href="http://www.sillonbelge.be/" title="Aller sur le site Le Sillon Belge (fenêtre externe)" class="ext">Le Sillon Belge</a></li>
                <li><a href="http://www.lenseo.com/fr/" title="Aller sur le site Lenseo (fenêtre externe)" class="ext">Lenseo</a></li>
                <li><a href="http://soirmag.lesoir.be/" title="Aller sur le site Soirmag (fenêtre externe)" class="ext">Soirmag</a></li>
                <li><a href="http://www.netevents.be/fr/" title="Aller sur le site Net Events (fenêtre externe)" class="ext">Net Events</a></li>
                <li><a href="http://boutique.lesoir.be/" title="Aller sur le site La Boutique du Soir (fenêtre externe)" class="ext">La Boutique du Soir</a></li>
              </ul>
              <ul>
                <li><a href="http://www.photobook.be/" title="Aller sur le site PhotoBook (fenêtre externe)" class="ext">Photobook</a></li>
                <li><a href="http://www.gaultmillau.be/" title="Aller sur le site Gaultmillau (fenêtre externe)" class="ext">Gaultmillau</a></li>
                <li><a href="http://www.references.be/" title="Aller sur le site Références (fenêtre externe)" class="ext">Références</a></li>
                <li><a href="http://www.cinenews.be/" title="Aller sur le site Cinenews (fenêtre externe)" class="ext">Cinenews</a></li>
                <li><a href="http://www.sudpresse.be/" title="Aller sur le site Sud Presse (fenêtre externe)" class="ext">Sud Presse</a></li>
                <li><a href="http://www.plaquemineralogique.be/" title="Aller sur le site Plaque Minéralogique (fenêtre externe)" class="ext">Plaque Minéralogique</a></li>
              </ul>
              <ul>
                <li><a href="http://passiondesmontres.lesoir.be" title="Aller sur le site Passion des montres (fenêtre externe)" class="ext">Passion des montres</a></li>
                <li><a href="http://www.rossel.be/" title="Aller sur le site Rossel (fenêtre externe)" class="ext">Rossel</a></li>
                <li><a href="http://www.ticketnet.be/" title="Aller sur le site Ticketnet (fenêtre externe)" class="ext">Ticketnet</a></li>
                <li><a href="http://www.lavoixdunord.fr/" title="Aller sur le site La Voix du Nord (fenêtre externe)" class="ext">La Voix du Nord</a></li>
                <li><a href="http://marchedelart.lesoir.be" title="Aller sur le site Marché de l'art (fenêtre externe)" class="ext">Marché de l'art</a></li>
              </ul>
              <ul>
                <li><a href="http://www.rosseladvertising.be/" title="Aller sur le site Rossel advertising (fenêtre externe)" class="ext">Rossel advertising</a></li>
                <li><a href="http://www.carnews.be/" title="Aller sur le site Carnews (fenêtre externe)" class="ext">Carnews</a></li>
                <li><a href="http://www.lecho.be/" title="Aller sur le site L'Echo (fenêtre externe)" class="ext">L'Echo</a></li>
                <li><a href="http://www.grenzecho.be/" title="Aller sur le site GrentzEcho (fenêtre externe)" class="ext">GrentzEcho</a></li>
                <li><a href="http://saveurs.lesoir.be/" title="Aller sur le site Saveurs.be (fenêtre externe)" class="ext">Saveurs.be</a></li>
              </ul>
            </div>
          </div>
          <div class="grid_3">
            <?php if ($sm_footer): print '<div id="mission">'. $sm_footer .'</div>'; endif; ?>
            <a href="http://www.cim.be" title="Cim.be (nouvelle fenêtre)"><img class="img_middle" src="/<?php print $path; ?>/mediastore/elements/metriweb.gif" width="120" height="34" alt="CIM Metriweb" /></a>
          </div>
        </div>
      </div>
    </div>