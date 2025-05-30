#!/usr/bin/perl

use CGI;

# Créer un objet CGI
my $query = CGI->new;

# Envoyer l'en-tête HTTP Content-Type
print $query->header('text/html');

# Afficher le contenu HTML
print "<html><body>";
print "<h1>Bonjour, le module CGI fonctionne !</h1>";
print "</body></html>";
