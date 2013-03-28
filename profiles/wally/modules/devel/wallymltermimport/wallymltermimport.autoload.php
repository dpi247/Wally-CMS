<?php 

use Symfony\Component\Yaml\Yaml;

//libraries to parse yaml files
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Yaml.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Parser.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Inline.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Unescaper.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Escaper.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Dumper.php');

//libraries for Exception
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Exception/ExceptionInterface.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Exception/RuntimeException.php');
require_once ($_SERVER['DOCUMENT_ROOT'].'/profiles/wally/modules/devel/wallymltermimport/libraries/Yaml/Exception/ParseException.php');

