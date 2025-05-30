#!/usr/bin/python3

import os

# Environnement et paramètres
envp = os.environ
script_name = "/cgi/python.py"

# Valeurs fixes pour a et b
post_a = 2
post_b = 2

# Calculer le résultat de l'addition
result = post_a + post_b

# Gérer la variable de résultat dans la requête GET
get_result = None
if "result" in envp["QUERY_STRING"]:
    get_result = envp["QUERY_STRING"].split("=")[1]

# Rediriger après un POST pour éviter une boucle infinie
if envp["REQUEST_METHOD"] == "POST":
    print("Status: 301 Moved Permanently")
    print(f"Location: {script_name}?result={result}")
    print()
    exit()  # Terminer le script après la redirection

# Gérer les autres types de requêtes non supportées
if envp["REQUEST_METHOD"] != "GET":
    print("Status: 400 Bad Request")
    print("Content-Type: text/html")
    print()
    print("<html><body><p>400 Bad Request: Method not supported</p></body></html>")
    exit()

# Si la méthode est GET, afficher le formulaire et le résultat
print("Content-Type: text/html")
print()
print(f"""
<!DOCTYPE html>
<html>
<head>
    <title>Cool Python CGI Script</title>
</head>
<body>
    <p>Hi, I am a CGI script made in Python.</p>
    <p>I can do everything Python can do, like addition.</p>
    <form method="post" action="{script_name}">
        <fieldset>
            <legend>Super Awesome Calculator</legend>
            <p>
                2 + 2 = {result}
            </p>
        </fieldset>
    </form>
""")
if get_result is not None:
    print(f"""
        <p>
            Result: {get_result}
        </p>
    """)
print("""
</body>
</html>
""")
