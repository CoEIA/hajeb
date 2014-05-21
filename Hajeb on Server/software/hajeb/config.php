<?php

	$user = "root";
	$password = "" ;
	$database = "coeia_hajeb";
	
	mysql_connect("localhost", $user, $password);
	@mysql_select_db($database) or die( "Unable to select database");

?>