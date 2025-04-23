<pre><?php
echo "hi i am a php file\n";

if (isset($_GET["name"])) {
	echo "your name is " . $_GET["name"] . "\n";
} else {
	echo "define a name with ?name=<your-name>\n";
}
echo "this script is " . $_SERVER['SCRIPT_NAME'] . "\n";
print_r($_SERVER);

?></pre>
