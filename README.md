# Scanner de arquivos para buscar palavras (com tolerância)

> De autoria do templo Shaolin

***
#### Funções:
- [x] João cuida da contagem de palavras (função count_ocurences)
- [x] Jonathan cuida da saída de dados (mostra a contagem, apresenta count e os arquivos)
- [x] Cauã cuida da tolerância
- [x] Miguel cuida da entrada (recebe o nome do arquivo, palavra de busca)
- [x] Matheus escreve e faz o slide
***
#### Como rodar:
Dependências:
- Cmake
- Make
- Um compilador de C (como clang, gcc, mingw, etc.)

1. Crie um diretório de build
2. Entre no diretório de build
3. `Cmake ..`
4. `make`
5. `./FileScanner`

#### Possíveis problemas
1. Caso o programa não consiga abrir a pasta que você quer, garanta que ela está no diretório de build
> A pasta `test-files` pode ser colocada no diretório de build para testar o funcionamento
2. Caso o programa tenha falhas em abrir os arquivos, garanta que cada arquivo `.pdf` possui um `.txt` com o mesmo nome

