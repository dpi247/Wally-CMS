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
       font: 11px sans-serif;
       background-color: transparent;
    }
    div {
       color: #FF0000;
       font: 11px sans-serif;
       background-color: transparent;
    }
    html {
      overflow:hidden;
      background-color: transparent;
    }
    a { color: #FFFFFF; text-decoration: none;}
    a:hover { color: #FFFFFF; text-decoration: underline; cursor: hand }
    </style>
    </head>
    <body>
    ';

    // background-color: #497CC9;
  
    $content.='<div>'.$array_of_informations['title'].'</div>';
    $content.='<div>'.$array_of_informations['comment_count'].'</div>';
    $content.='</body></html>';
    return $content;

}