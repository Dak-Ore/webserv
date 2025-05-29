<pre><?php
// Démarrer la session
session_start();

// Définir un cookie qui expirera dans 1 heure
setcookie("user", "JohnDoe", time() + 3600, "/");  // expire dans 1 heure

// Définir une variable de session
$_SESSION['username'] = "JohnDoe";

// Afficher les informations
echo "<h1>Cookie et Session Test</h1>";
echo "<p>Nom d'utilisateur: " . $_SESSION['username'] . "</p>";
echo "<p>Nom du cookie: " . (isset($_COOKIE['user']) ? $_COOKIE['user'] : 'Non defini') . "</p>";
?></pre>