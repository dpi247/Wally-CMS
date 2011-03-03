<?php
function theme_dynamicfeedback_page_render($array_of_informations){
$content='<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
   <title>Statistics</title>
   <style type="text/css">
      body {
         margin:0;
         color: #FFFFFF;
         font: bold 11px sans-serif;
      }
      html {
        overflow:hidden;
      }
      a { color: #FFFFFF; text-decoration: none;}
      a:hover { text-decoration: underline; }
   </style>
   <script type="text/javascript">
      <!--
      function popup(mylink, windowname)
      {
         if (! window.focus)return true;
         var href;
         if (typeof(mylink) == \'string\')
            href=mylink;
         else
            href=mylink.href;
         <!-- window.open(href, windowname, \'width=400,height=200,scrollbars=yes\'); -->
         window.open(href, windowname, \'scrollbars=yes\');
         return false;
      }
      //  Color can be altered below - eg. remove - ABCDEF = darker
      function toHex(n){
         var hexChars = \'0123456789ABCDEF\';
         if (n == 0) return n;
         var j, k;
         var temp = \'\';
         while (n != 0){
         j = n % 16;
            n = (n - j)/16;
            temp = hexChars.charAt(j) + temp;
         }
         return temp;
      }

      function colorize()
      {
         if (!document.getElementById) return;
         for (i=0; i<msg.length; i++){
            k = Math.round(Math.random() * 16777215);
            k = toHex(k);
            while (k.length < 6){
               k = k + \'0\';
            }
            document.getElementById(\'colorizer\' + i).style.color = \'#\' + k;
         }

         //  Speed change below - Default = 200 - Lower = Faster

         colorTimer = window.setTimeout(\'colorize()\', 200);
      }
      //-->
   </script>
</head>
<!-- <body onload="return popup(\'http://www.ccieurope.com/\', \'ad\')"> -->
<!-- <body onload="return colorize()";> -->
<body>
';

// background-color: #497CC9;
$content.='<div>, Title: <a href="http://'.$array_of_informations['base_url'].'/node/'.$array_of_informations['nodeId'].'" onClick="return popup(this, \'Wally_web_page\');">'.$array_of_informations['title'].'</a> Comments: '.$array_of_informations['comment_count'].'</div>';

//$content.='<div>, title: '.$array_of_informations['title'].' ';
//$content.='<script>
//var msg = \'Comments: '.$array_of_informations['comment_count'].'\'
//var colorTimer = null;
//for (var i=0; i<msg.length; i++){
//	document.write("<b id =\'colorizer" + i + "\'>" + msg.charAt(i) + "</b>");
//}
//</script>';
//$content.=', <a href="http://10.200.30.167" onClick=return popup(this, \'Wally Site\');">Wally</a></div>';

//  $content.='<div>'.$array_of_informations['title'].'</div>';
//  $content.='<div>'.$array_of_informations['comment_count'].'</div>';
$content.='</body></html>';
  return $content;

}
