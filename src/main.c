#include <stdio.h>
#include <string.h>

#include "astf.h"

#define BUFFER_SIZE 100 // tamanho máximo de uma palavra

#define FILE1 FILE *test1 = fopen("../test-files/test1.txt", "r")
#define FILE2 FILE *test2 = fopen("../test-files/test2.txt", "r")
#define FILE3 FILE *test3 = fopen("../test-files/test3.txt", "r")
#define FILE4 FILE *test4 = fopen("../test-files/test4.txt", "r")
#define FILE5 FILE *test5 = fopen("../test-files/test5.txt", "r")

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
  astf_start_test_suite("Read tests");

  // Para abrir cada arquivo, basta digitar essa linha com o devido número
  FILE1;

  if (test1 == NULL) {
    printf("Erro na leitura do arquivo!\n");
    return 1;
  }

  char word[BUFFER_SIZE];
  // Loop que lê o arquivo palavra por palavra
  while (fscanf(test1, "%s", word) == 1) {
    printf("%s\n", word);
  }

  // verifica se a ultima palavra bate
  astf_AE_string(word, "testes.");

  astf_retrieve_results();

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
