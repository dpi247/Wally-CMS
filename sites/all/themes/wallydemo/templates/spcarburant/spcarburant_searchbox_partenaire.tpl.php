      <div class="column recherche carburant service">
      <h2><span>Recherche</span></h2>
        <div class="formulaire">
          <form id="searchForm" action="http://www.carbu.be/results.php" method="post">
          <div class="row">
          <select name="carburant_type" >
                     <option value="1" selected>Diesel</option>

                     <option value="2" >Eurosuper 95</option>
                     <option value="3" >Eurosuper 98</option>
                     <option value="4" >LPG</option>
          </select>
          </div>
          <div class="row">
          <label>Entrez la commune ou le code postal</label>

          <input type="text" name="searchCp" size="20" value="Code postal ou localit&eacute;" onfocus="javascript:this.value=''" >
          </div>
          <div class="row">
          <input type="hidden" name="source" value="sudpresse">
          <input type ="submit" value="Rechercher">
          </div>
          </form>
        </div>
      </div>
