<?php
$content =  str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $content);
$content =  str_replace('href="', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $content);
$content =  str_replace('src="/', 'src="http://'.$_SERVER['SERVER_NAME'].'/', $content);
$styles =  str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $styles);
$styles =  str_replace('src="/', 'src="http://'.$_SERVER['SERVER_NAME'].'/', $styles);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:og="http://ogp.me/ns#" xmlns:fb="http://www.facebook.com/2008/fbml" xml:lang="fr" lang="fr" dir="ltr">
<head>
<?php print $styles ?>
</head>
<body style="background:none;">
<?php print $content; ?>
</body>
</html>