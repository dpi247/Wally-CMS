<div id="cpsprint" class="cpsprint"> 
  <?php if ($content['133x185']) { ?>
    <div id="print133x185">
      <?php print $content['133x185'];?>
    </div>
  <?php }?>
  <?php if ($content['133x90A'] | $content['133x90B']) {?>
    <div id="print133x90AB">
    <?php if ($content['133x90A']) { ?>
      <div id="print133x90A">
        <?php print $content['133x90A'];?>
      </div>
    <?php }?>
    <?php if ($content['133x90B']) { ?>
      <div id="print133x90B">
        <?php print $content['133x90B'];?>
      </div>
    <?php }?>
    </div>
 <?php }?> 
 <?php if ($content['133x185B']) { ?>
    <div id="print133x185B">
      <?php print $content['133x185B'];?>
    </div>
  <?php }?>
 <?php if ($content['133x185C']) { ?>
    <div id="print133x185C">
      <?php print $content['133x185C'];?>
    </div>
  <?php }?>
  <div class="clear"></div>
 </div>