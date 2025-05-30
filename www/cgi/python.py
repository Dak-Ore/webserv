import os

envp = os.environ
script_name = "/cgi/python.py"

post_a = None
post_b = None

# Récupérer les données POST si disponibles
try:
    if envp["REQUEST_METHOD"] == "POST":
        # Traiter les données de la requête POST
        while True:
            l = input()
            l = l.split("&")
            for v in l:
                if v.startswith("a="):
                    post_a = int(v[2:])
                elif v.startswith("b="):
                    post_b = int(v[2:])
except EOFError:
    pass

get_result = None
if envp["QUERY_STRING"].startswith("result="):
    get_result = envp["QUERY_STRING"][7:]

if envp["REQUEST_METHOD"] == "POST":
    # Effectuer la redirection après la soumission du formulaire POST
    print("Status: 301 Moved Permanently")
    print(f"Location: {script_name}?result={str(post_a + post_b)}")
    print()
    exit()

if envp["REQUEST_METHOD"] != "GET":
    # Réponse pour les autres types de requêtes non supportées
    print("Status: 400 Bad Request")
    print()
    exit()

# Si la méthode est GET, afficher le formulaire et le résultat
print("Content-Type: text/html")
print("")
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
                <input name="a" type="number" value="2" />
                +
                <input name="b" type="number" value="2" />
            </p>
            <p>
                <input type="submit" value="Calculate" />
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
