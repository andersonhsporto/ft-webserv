import cgi
import cgitb
import os
import sys

cgitb.enable()

method = cgi.parse_header(os.environ["REQUEST_METHOD"])[0]
content = sys.stdin.read()

if method == "GET":
    print("<html>")
    print("<meta charset='utf-8'>")
    print("<head>")
    print("<title>Exemplo de CGI em Python</title>")
    print("</head>")
    print("<body>")
    print("<h1>Exemplo de CGI em Python</h1>")
    print(f"<p>Método HTTP: {method}</p>")
    print("</form>")
    print("</body>")
    print("</html>")

elif method != "GET":
    print(f"Método HTTP: {method}")
    print(f"Corpo da requisição:\n\n {content}")

#################################################################################################
#################################################################################################


# # LOOP INFINITO
# while True:
#     print("Looping...")

