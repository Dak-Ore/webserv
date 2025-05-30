<?php

if (!isset($_COOKIE["cookies"]))
	$cookies = 0;
else
	$cookies = intval($_COOKIE['cookies']) + 1;
setcookie("cookies", strval($cookies), time() + 60 * 60);

?>

<!DOCTYPE html>
<html>
<head>
	<title><?= $cookies ?> cookies<?= $cookies > 1 ? "s" : "" ?></title>
	<style type="text/css">
		.cookie
		{
			display: inline-block;
			width: 20px;
		}
	</style>
</head>
<body>
	<p>
		each time the page is reload, a cookie will appear :)
	</p>
	<?php
	if ($cookies > 0) {
		?><p><?php
		for ($i=0; $i < $cookies; $i++) { 
			?><img class="cookie" src="cookie.png" alt="cookie" /><?php
		}
		?></p><?php
	}
	?>
	<p>
		the cookies will expire in 1 hour
	</p>
	<p>
		<button onclick="location.reload();">reload page</button></a>
	</p>
</body>