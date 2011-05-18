<?
	// == DB Connection ========================================================

	$DBHost = "dbweb1.rossel.be";
	$DBUser = "lsforum2";
	$DBPass = "for9695um2";
	$DBName = "lsforum2";

	$DBLink = mysql_connect($DBHost, $DBUser, $DBPass) or die("<strong>Error:</strong> We are actualy experiencing some database problems...\n\r");
	mysql_select_db($DBName, $DBLink) or die("<strong>Error:</strong> We are actualy experiencing some database problems...\n\r");
?>
