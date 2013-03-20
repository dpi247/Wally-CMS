<?php 

use Symfony\Component\Yaml\Yaml;

//libraries to parse yaml files
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Yaml.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Parser.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Inline.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Unescaper.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Escaper.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Dumper.php');

//libraries for Exception
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Exception/ExceptionInterface.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Exception/RuntimeException.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallyimport_destination/libraries/Yaml/Exception/ParseException.php');

