#include <stdio.h>
#include <string.h>

// Micro framework de teste
#include "astf.h"

#define BUFFER_SIZE 100 // Tamanho máximo de uma palavra
#define TAMANHO_MAX_STRING 1024 // tamanho maximo da matriz de strings

#define FILE1 FILE *test1 = fopen("../test-files/test1.txt", "r")
#define FILE2 FILE *test2 = fopen("../test-files/test2.txt", "r")
#define FILE3 FILE *test3 = fopen("../test-files/test3.txt", "r")
#define FILE4 FILE *test4 = fopen("../test-files/test4.txt", "r")
#define FILE5 FILE *test5 = fopen("../test-files/test5.txt", "r")

// funcao pra retornar o tamanho de uma string
int our_strlen(const char *str){
  int i;
  for(i = 0; str[i] != '\0' && i < BUFFER_SIZE; i++);
  return i;
}

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

// algoritmo pra medir a distancia entre duas strings por matrizes (fuzzy search)
int levenshtein_distance(const char *s1, const char *s2) {
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);

  if (len1 >= TAMANHO_MAX_STRING || len2 >= TAMANHO_MAX_STRING) {
    printf("Erro: A string é maior que o limite de %d caracteres.\n", TAMANHO_MAX_STRING);
    return -1;
  }

  // matriz tem um tamanho fixo, definido em tempo de compilação
  int matrix[TAMANHO_MAX_STRING][TAMANHO_MAX_STRING];

  for (size_t i = 0; i <= len1; i++) {
      for (size_t j = 0; j <= len2; j++) {
          if (i == 0) {
              matrix[i][j] = (int)j;
          } else if (j == 0) {
              matrix[i][j] = (int)i;
          } else {
              int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
              matrix[i][j] = min(matrix[i - 1][j] + 1,
                                 matrix[i][j - 1] + 1,
                                 matrix[i - 1][j - 1] + cost);
          }
      }
  }

  return matrix[len1][len2];
}

int count_ocurences(char target[], FILE *file);



/*
 * Função responsável por contar quantas ocorrências da
 * palavra tem num arquivo
*/
int count_ocurences(char target[], FILE *file) {
  int count = 0;

  char word[BUFFER_SIZE];
  // Loop que lê o arquivo palavra por palavra
  while (fscanf(file, "%s", word) == 1) {
    printf("%s\n", word);

    // Verificando se target e word batem
    if (strcmp(target, word) == 0) {
      printf("Bateu!");
    }
  }

  return count;
}

/*
  * Função responsável pela execução do programa
  * Fazer leitura, processamento e input aqui
*/
void run() {
  printf("Running!\n");


  // teste pra levenshtein (ta funcionando)
  const char *arr[] = {"casa", "kasa", "kaza", "casas", "kazas", "cansas", "kansas"};
  int num_palavras = sizeof(arr) / sizeof(arr[0]);
  printf("Número de palavras: %d\n", num_palavras);

  for(int i = 0; i < num_palavras; i++){
    printf("Distância de Levenshtein entre '%s' e 'casa': %d\n", arr[i], levenshtein_distance(arr[i], "casa"));
  }
}

/*
  * Função responssável por testes
  * Testar o código aqui
*/
int test() {
  // Inicializanddo os teste
  astf_start_test_suite("Read tests");

  // Para abrir cada arquivo, basta digitar essa linha com o devido número
  FILE1;

  // Conferindo que o arquivo existe
  astf_AN_null(test1);

  // Verificando se "qualquer coisa que não tem num arquivo" não ocorre em "test1.txt"
  astf_AE_int(0, count_ocurences("qualquer coisa que não tem num arquivo", test1));

  // Verificando o resultado dos testes
  astf_retrieve_results();

  // Fechando o arquivo
  fclose(test1);

  return 1;
}


int main(int argc, char *argv[]) {
  if (argc > 1 && !strcmp(argv[1], "test")) {
    test();
  } else {
    run();
  }
}
