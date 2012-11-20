<?php dsm($variables);?>
<div>
  <div id="wallyscheduler_status">
  <h2><?php print t("Status")?></h2>
   <?php
      print theme('table', $variables['variables']['header_status'], $variables['variables']['rows_status'], array('id' => 'wallyscheduler_futur_snapshots'));
   ?>
  </div>

  <div id="wallyscheduler_futur_snapshot">
    <h2><?php print t("Future snapshot")?></h2>
  <?php 
    print theme('table', $variables['variables']['header_future_snapshots'], $variables['variables']['rows_future_snapshots'], array('id' => 'wallyscheduler_futur_snapshots'));
	print theme('pager', NULL, 50, 0);
	?>
  </div>
  
  <div id="wallyscheduler_history">
    <h2><?php print t("History")?></h2>
  <?php
  print theme('table', $variables['variables']['header_history'], $variables['variables']['rows_history'], array('id' => 'admin-dblog'));
  print theme('pager', NULL, 50, 0);
  ?>
  </div>
  
</div>