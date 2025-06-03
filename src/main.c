#include <stdio.h>
#include <string.h>

#include <astf.h>

int test_fn(int x) { return x * x; }

void run() { printf("Running!\n"); }

void test() {
  astf_start_test_suite("Some test");

  astf_AE_int(test_fn(3), 9);
  astf_AE_int(test_fn(4), 16);
  astf_AE_int(test_fn(5), 25);

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
