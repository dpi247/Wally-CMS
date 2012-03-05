<?php 
$path = drupal_get_path('theme', 'wallydemo');
$domain = "http://".$_SERVER["HTTP_HOST"]."/";
$path_image = $domain.$path.'/images/newsletter/';

$my_data = _wallydemo_get_logo_data();
if($my_data["default"] == 1){
	$logo_src = $my_data["default_path"];
}else{
  $logo_src = $my_data["eve_path"];
}

$my_data = spmeteo_getCachedData('meteo_header_bloc');
$imgFolder = $my_data['images_folder'];
$logo = array('Ciel serein','Ciel peu nuageux','Ciel très nuageux','Couvert','Brouillard','Pluie','Bruine','Neige','Pluie et neige','Pluie et bruine','Averses de pluie','Averses orageuses','Giboulées','Averse de neige','Averses de pluie et neige','Orage','Orageux','Verglas');
$my_dataS = spmeteo_getCachedData("meteo_saint");

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Newsletter Sudinfo</title>
</head>

<body style="background-color:#ebebeb">
<table  style="width:728px;" border="0" cellspacing="0" cellpadding="0" align="center">
  <tr>
    <td style="height:5px;" height="5"></td>
  </tr>
  <tr>
    <td><table width="100%" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td><font style="font-family:Arial, Helvetica; font-size:12px; color:#4c4c4c;">Sudinfo par e-mail</font></td>
          <td style="text-align:right;"><font style="font-family:Arial, Helvetica; font-size:12px; color:#4c4c4c;">Si vous ne parvenez pas à lire  ce message, <a href="##VIEW_ONLINE##" target="_blank" style="font-size:12px; color:#003366;">cliquez ici</a></font></td>
        </tr>
      </table></td>
  </tr>
  <tr>
    <td style="height:5px;" height="5"></td>
  </tr>
  <tr>
    <td><a href="http://www1.actito.be/Actito4Media/res.action?c=&ms=Sudpresse_SPQuotidienne_LEADERBOARD&ep=##email##"> <img src="http://www1.actito.be/Actito4Media/res.action?v=&ms=Sudpresse_SPQuotidienne_LEADERBOARD&ep=##email##" border="0"/></a></td>
  </tr>
  <tr>
    <td style="height:10px;" height="10"></td>
  </tr>
  <tr>
    <td style="background-color: #FFFFFF;"><table width="100%" border="0" cellspacing="0" cellpadding="3" style="border:1px solid #CCC;" >
        <tr>
          <td><table width="100%" border="0" cellspacing="15" cellpadding="0">
              <tr>
                <td><table width="100%" border="0" cellspacing="0" cellpadding="0">
                    <tr>
                      <td><table width="100%" border="0" cellspacing="0" cellpadding="0">
                          <tr>
                            <td style="height:19px;" valign="top" height="19"><font style="font-family:Arial, Helvetica; font-size:12px; color:#808080;"><?php echo _wallydemo_date_edition_diplay(time(),'date_jour'); ?></font></td>
                          </tr>
                          <tr>
                            <td style="height:50px;" valign="top" height="50"><a href="<?php print $domain; ?>" target="_blank"> <img src="<?php print $domain; ?><?php print $logo_src; ?>" border="0" /></a></td>
                          </tr>
                          <tr>
                            <td style="height:31px; border-top:1px solid #e0e0d3; border-bottom:1px solid #e0e0d3; " height="31"><img src="<?php print $domain; ?><?php print $imgFolder; ?>/meteo_little/<?php print $my_data["temp_symbole"] ?>.gif" width="20" height="20" align="absmiddle" /> <font style="font-family:Arial, Helvetica; font-size:12px; color:#808080;"> <a href="<?php   echo $domain.drupal_get_path_alias("/meteo")?>" target="_blank" style="font-size:12px; color:#003366; text-decoration:none;"><?php print $logo[($my_data["temp_symbole"])-1] ?></a> <font style="color:#0000ff;"><?php print $my_data["temp_min"] ?></font> / <font style="color:#ff0000;"><?php print $my_data["temp_max"] ?></font> - <?php print $my_dataS['saint']; ?></font></td>
                          </tr>
                        </table></td>
                      <td style="width:15px;">&nbsp;</td>
                      <td style="width:160px;"><a href='http://ads.sudpresse.be/adclick.php?n=ada9251f' target='_blank'><img src='http://ads.sudpresse.be/adview.php?what=zone:435&amp;n=ada9251f' border='0' alt=''/></a></td>
                      <td style="width:15px;">&nbsp;</td>
                      <td style="width:160px;"><a href='http://ads.sudpresse.be/adclick.php?n=a5dca502' target='_blank'><img src='http://ads.sudpresse.be/adview.php?what=zone:436&amp;n=a5dca502' border='0' alt=''/></a></td>
                    </tr>
                  </table></td>
              </tr>
              <?php print $content; ?>
            </table></td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
      </table></td>
  </tr>
</table>
</body>
</html>
