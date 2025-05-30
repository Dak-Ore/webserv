<p>hi i am a php file</p>

<?php
if (isset($_GET["name"])) {
	?>
	<p>your name is <b><?= $_GET["name"] ?></b>.<p>
	<?php
} else {
	?>
	<form method="get" action=".">
		<fieldset>
			<legend>send info with method GET</legend>
			<p><label for="get_name">name:</label><input id="get_name" name="name" type="text" />
			<p><input type="submit" value="send" /></p>
		</fieldset>
	</form>
	<?php
}

if (isset($_POST["favoritemeal"])) {
	?>
	<p>your favorite meal is <b><?= $_POST["favoritemeal"] ?></b> :)
	<?php
} else {
	?>
	<form method="post" action=".">
		<fieldset>
			<legend>send info with method POST</legend>
			<p><label for="get_fav">favorite meal:</label><input id="get_fav" name="favoritemeal" type="text" />
			<p><input type="submit" value="send" /></p>
		</fieldset>
	</form>
	<?php
}
?>

<p>this script is <code><?= $_SERVER['SCRIPT_NAME'] ?></code></p>

<pre><?php print_r($_SERVER); ?></pre>

<p>(<a href="cookie/">test cookies</a>)</p>
<p>(<a href="cookies/">test cookies 2</a>)</p>
<p>(<a href="python.py">python cgi</a>)</p>
