#include <stdio.h>
#include <string.h>

// Micro framework de teste
#include "astf.h"

#define BUFFER_SIZE 100 // Tamanho máximo de uma palavra

#define FILE1 FILE *test1 = fopen("../test-files/test1.txt", "r")
#define FILE2 FILE *test2 = fopen("../test-files/test2.txt", "r")
#define FILE3 FILE *test3 = fopen("../test-files/test3.txt", "r")
#define FILE4 FILE *test4 = fopen("../test-files/test4.txt", "r")
#define FILE5 FILE *test5 = fopen("../test-files/test5.txt", "r")

void run();
int test();
int count_ocurences(char target[], FILE *file);

int main(int argc, char *argv[]) {
  if (argc > 1 && !strcmp(argv[1], "test")) {
    test();
  } else {
    run();
  }
}

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
void run() { printf("Running!\n"); }

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
