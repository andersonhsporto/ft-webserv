print("Content-Type: text/html")    # Informa ao servidor que o conteúdo será HTML
print()                              # Imprime uma linha em branco para separar o cabeçalho do corpo

# Define o conteúdo HTML que será exibido na página
html = """
<html>
<head>
    <title>Texto animado</title>
    <meta charset="UTF-8">
    <style>
        /* Define o estilo do texto */
        #texto {
            font-size: 30px;
            position: absolute;
            left: 50%;
            top: 50%;
            transform: translate(-50%, -50%);
        }
    </style>
</head>
<body>
    <div id="texto">Olá! este é um teste do cgi em python</div>
    <script>
        var elemento = document.getElementById("texto");
        var posicao = 0;
        var direcao = 1;

        // Define a animação do texto
        function animarTexto() {
            posicao += direcao * 5;
            if (posicao > 50 || posicao < -50) {
                direcao *= -1;
            }
            elemento.style.top = (50 + posicao) + "%";
        }

        // Define a taxa de atualização da animação
        setInterval(animarTexto, 100);
    </script>
</body>
</html>
"""

# Imprime o conteúdo HTML na página
print(html)