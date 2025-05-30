import os
envp = os.environ
script_name = "/cgi/python.py"

post_a = None
post_b = None

try:
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
	print("Status: 301 its somewhere else")
	print(f"Location: " +
	  script_name +
	  "?result=" +
	  str(
	    post_a + post_b
	  )
	)
	print()
	exit()

if envp["REQUEST_METHOD"] != "GET":
	print("status: 400 huh what")
	print()
	exit()

print("content-type: text/html")
print("")
print(f"""
<!DOCTYPE html>
<html>
<head>
	<title>cool python cgi script</title>
</head>
<body>
	<p>hi i am a cgi script made in python</p>
	<p>i can do everything python can do</p>
	<p>like addition</p>
	<form method="post" action="{script_name}">
		<fieldset>
			<legend>super awesome calculator</legend>
			<p>
				<input name="a" type="number" value="2" />
				+
				<input name="b" type="number" value="2" />
			</p>
			<p>
				<input type="submit" value="calculate" />
			</p>
""")
if get_result is not None:
	print(f"""
			<p>
				result: {get_result}
			</p>
	""")
print("""
		</fieldset>
	</form>
</body>
</html>
""")