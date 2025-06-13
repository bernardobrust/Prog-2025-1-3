#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024 // Tamanho máximo de uma palavra
#define LIST "../lista.txt"

// Função para encontrar o mínimo entre três números
int min(int a, int b, int c) {
  if (a < b && a < c) {
    return a;
  } else if (b < c) {
    return b;
  } else {
    return c;
  }
}

// Algoritmo pra medir a distancia entre duas strings por matrizes (fuzzy
// search)
int char_diff_tolerance(const char *s1, const char *s2) {
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);

  if (len1 >= BUFFER_SIZE || len2 >= BUFFER_SIZE) {
    printf("Erro: A string é maior que o limite de %d caracteres.\n",
           BUFFER_SIZE);
    return -1;
  }

  // Matriz tem um tamanho fixo, definido em tempo de compilação
  int matrix[BUFFER_SIZE][BUFFER_SIZE];

  for (size_t i = 0; i <= len1; i++) {
    for (size_t j = 0; j <= len2; j++) {
      if (i == 0) {
        matrix[i][j] = (int)j;
      } else if (j == 0) {
        matrix[i][j] = (int)i;
      } else {
        int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
        matrix[i][j] = min(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1,
                           matrix[i - 1][j - 1] + cost);
      }
    }
  }

  return matrix[len1][len2];
}

/*
 * Função responsável por contar quantas ocorrências da
 * palavra tem num arquivo
 */
[[nodiscard]] int count_ocurences(char target[], FILE *file) {
  int count = 0;

  char word[BUFFER_SIZE];
  // Loop que lê o arquivo palavra por palavra
  while (fscanf(file, "%s", word) == 1) {
    // Verificando se target e word batem (com tolerancia 1)
    if ((char_diff_tolerance(word, target) == 0) || (char_diff_tolerance(word, target) == 1)) {
      count++;
    }
  }

  return count;
}

/*
 * Função responsável pela execução do programa
 * Fazer leitura, processamento e input aqui
 */
void run() {
  char folder[BUFFER_SIZE];
  char word[BUFFER_SIZE];
  char cmd[BUFFER_SIZE * 2];

  // Pedindo ao usuário para inserir a pasta
  printf("Insira o nome da pasta a ser analisada: \n");
  printf("> ");
  scanf("%s", folder);
  printf("\n");

  // --- Lendo o diretório

  // Criando uma variável de "extensão" para mostrar ao computador como
  // Percorrer os arquivos do diretório(pasta)
  const char pdf[] = "/*.pdf";

  int j = 0;
  for (size_t i = strlen(folder); i <= strlen(folder) + strlen(pdf); i++) {
    folder[i] = pdf[j];
    j++;
  }
  folder[strlen(folder)] = '\0';

  // Pedindo ao usuário a palavra a ser buscada
  printf("Insira a palavra a ser buscada: \n");
  printf("> ");
  scanf("%s", word);
  printf("\n");

  // Print nescessário do analizando arquivos
  printf("Analizando arquivos... \n");
  printf("\n");

  // Rodando o comando
  sprintf(cmd, "cd .. && dir \%s > lista.txt", folder);
  system(cmd);

  // --- Coletando os arquivos
  // Lendo o arquivo "lista.txt"
  FILE *list = fopen(LIST, "r");

  if (list == NULL) {
    printf("Erro na abertura da lista\n");
  }

  // Loop que lê o arquivo palavra por palavra
  char pdf_file[BUFFER_SIZE];
  while (fscanf(list, "%s", pdf_file) == 1) {
    // Extraindo os nomes dos arquivos e lendo seus .txt's equivalentes
    char txt[] = "txt";
    char txt_file[BUFFER_SIZE];

    // Atribuindo temporáriamente o nome do pdf ao arquivo que leremos
    strcpy(txt_file, pdf_file);

    // Formatando corretamente o nome do arquivo a ser lido
    int j = 0;
    for (size_t i = strlen(txt_file) - strlen(txt); i <= strlen(txt_file);
         i++) {
      txt_file[i] = txt[j];
      j++;
    }
    txt_file[strlen(txt_file)] = '\0';

    // Abrindo o arquivo
    FILE *file = fopen(txt_file, "r");

    if (file == NULL) {
      printf("Erro na abertura de %s\n", txt_file);
    }

    // Lendo do arquivo
    int count = count_ocurences(word, file);

    // Printando o resultado
    printf("Encontramos %d referencias a palavra %s em %s\n", count, word,
           pdf_file);
  }
}

/*
 * Função responssável por testes
 * Testar o código aqui
 */
int test() {
  // Intencionalmente em branco
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc > 1 && !strcmp(argv[1], "test")) {
    test();
  } else {
    run();
  }
}
