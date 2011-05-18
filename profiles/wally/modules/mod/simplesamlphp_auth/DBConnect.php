<?
	// == DB Connection ========================================================


	$DBHost = "dbweb1.rossel.be";
	$DBUser = "rosselpdfusr";
	$DBPass = "pdflspwd";
	$DBName = "rosselpdf";

	$DBLink = mysql_connect($DBHost, $DBUser, $DBPass) or die("<strong>Error:</strong> We are actualy experiencing some database problems...\n\r");
	mysql_select_db($DBName, $DBLink) or die("<strong>Error:</strong> We are actualy experiencing some database problems...\n\r");
	
	$DBHost = "commutest.rossel.be";
	$DBUser = "rosselpdftestusr";
	$DBPass = "pdflspwdTest";
	$DBName = "rosselpdftest";

	$DBLink = mysql_connect($DBHost, $DBUser, $DBPass) or die("<strong>Error:</strong> We are actualy experiencing some database problems...\n\r");
	mysql_select_db($DBName, $DBLink) or die("<strong>Error:</strong> We are actualy experiencing some database problems...\n\r");
?>
