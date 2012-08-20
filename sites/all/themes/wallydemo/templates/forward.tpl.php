<?php
/* $Id: forward.tpl.php,v 1.1.2.9 2010/08/30 19:59:34 seanr Exp $ */

/**
 * This template should only contain the contents of the body
 * of the email, what would be inside of the body tags, and not
 * the header.  You should use tables for layout since Microsoft
 * actually regressed Outlook 2007 to not supporting CSS layout.
 * All styles should be inline.
 *
 * For more information, consult this page:
 * http://www.anandgraves.com/html-email-guide#effective_layout
 *
 * If you are upgrading from an old version of Forward, be sure
 * to visit the Forward settings page to enable use of the new
 * template system.
 */
$domain_url = $_SERVER["SERVER_NAME"];
$domain = spdatastructure_getdomain($domain_url);
$site_name = variable_get($domain.'_site_name', NULL);
$site_url = variable_get($domain.'_site_url', NULL);
$associated_brand = variable_get($domain.'_associated_brand', NULL);
$path = drupal_get_path('theme', 'wallydemo');
$my_data = _wallydemo_get_logo_data();
if($my_data["default"] == 1){
  $logo_src = $my_data["html_target"].'/'.$my_data["default_path"];
}else{
  $logo_src = $my_data["html_target"].'/'.$my_data["eve_path"];
}
$logo = "<img src=\"".$logo_src."\" alt=\"".$site_name."\" title=\"".$site_name."\">";
?>
<html>
  <body>
    <table width="<?php print $width; ?>" cellspacing="0" cellpadding="10" border="0">
      <thead>
        <tr>
          <td>
            <h1 style="font-family:Arial,Helvetica,sans-serif; font-size:18px;"><a href="<?php print $site_url; ?>" title="<?php print $associated_brand; ?>"><?php print $logo; ?></a></h1>
          </td>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php print $forward_message; ?>
            <?php if ($message) { ?>
            <p><?php print t('Message from Sender'); ?></p><p><?php print $message; ?></p>
            <?php } ?>
            <?php //if ($title) {<h2 style="font-size: 14px;">test1<?php print $title;</h2>} ?>
            <?php //if ($submitted) { <p><em>print $submitted; </em></p> } ?>
            <?php if ($teaser) { ?><div><?php print $teaser; ?></div><?php } ?><p><?php print $link; ?></p>
          </td>
        </tr>
        <?php if ($dynamic_content) { ?><tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php print $dynamic_content; ?>
          </td>
        </tr><?php } ?>
        <?php if ($forward_ad_footer) { ?><tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php print $forward_ad_footer; ?>
          </td>
        </tr><?php } ?>
        <?php if ($forward_footer) { ?><tr>
          <td style="font-family:Arial,Helvetica,sans-serif; font-size:12px;">
            <?php print $forward_footer; ?>
          </td>
        </tr><?php } ?>
      </tbody>
    </table>
  </body>
</html>