#include <stdio.h>
#include <string.h>

#include <astf.h>

#define BUFFER_SIZE 100 // tamanho máximo de uma palavra

int test_fn(int x) { return x * x; }

void run() { printf("Running!\n"); }

void test() {
  astf_start_test_suite("Read tests");

  FILE *test1 = fopen("../test-files/test1.txt", "r");

  char word[BUFFER_SIZE];
  while (fscanf(test1, "%s", word) == 1) {
    printf("%s\n", word);
  }

  // verifica se a ultima palavra bate
  astf_AE_string(word, "testes.");

  astf_retrieve_results();
}

int main(int argc, char *argv[]) {
  if (argc > 1 && !strcmp(argv[1], "test")) {
    test();
  } else {
    run();
  }

  return 0;
}
