<pre><?php
echo "hi i am a php file\n";
echo "\n";

if (isset($_GET["name"]))
	echo "your name is " . $_GET["name"] . "\n";
else
	echo "define a name with ?name=<your-name>\n";
echo "\n";

if (isset($_POST["favoritemeal"]))
	echo "your favorite meal is " . $_POST["favoritemeal"] . " :)\n";
else
	echo "you can also try to POST the 'favoritemeal' field :)\n";
echo "\n";

echo "this script is " . $_SERVER['SCRIPT_NAME'] . "\n";
echo "\n";

print_r($_SERVER);

?></pre>
