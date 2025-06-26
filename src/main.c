#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024 // Tamanho máximo de uma palavra

// Função para encontrar o mínimo entre três números
[[nodiscard]] int min(const int a, const int b, const int c) {
  if (a < b && a < c) {
    return a;
  }
  if (b < c) {
    return b;
  }
  return c;
}

[[nodiscard]] int is_ponctuation(const char c) {
  const char *ponctuation = "._$,!?;:'\"()[]{}<>-";
  if (strchr(ponctuation, c) != NULL) {
    return 1;
  }
  return 0;
}

void clean_string(char *str) {
  if (str == NULL) {
    return;
}

  int reader = 0, writer = 0;

  while (str[reader] != '\0') {
    if (!is_ponctuation(str[reader])) {
      str[writer] = str[reader];
      writer++;
    }
    reader++;
  }
  str[writer] = '\0';
}

// Algoritmo pra medir a distancia entre duas strings por matrizes (fuzzy
// search)
[[nodiscard]] int char_diff_tolerance(const char *s1, const char *s2) {
  const size_t len1 = strlen(s1);
  const size_t len2 = strlen(s2);

  if (len1 >= BUFFER_SIZE || len2 >= BUFFER_SIZE) {
    printf("Erro: A string é maior que o limite de %d caracteres.\n",
           BUFFER_SIZE);
    return -1;
  }

  // Matriz tem um tamanho fixo, definido em tempo de compilação
  static int matrix[BUFFER_SIZE][BUFFER_SIZE];

  for (size_t i = 0; i <= len1; i++) {
    for (size_t j = 0; j <= len2; j++) {
      if (i == 0) {
        matrix[i][j] = (int)j;
      } else if (j == 0) {
        matrix[i][j] = (int)i;
      } else {
        const int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
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
[[nodiscard]] int count_ocurences(const char target[], FILE *file,
                                  const int tolerance) {
  int count = 0;
  char word[BUFFER_SIZE];

  while (fscanf(file, "%1023s", word) == 1) {
    // Limpa a string de pontuações
    clean_string(word);
    // A mágica acontece aqui: uma única chamada para todos os casos
    if (char_diff_tolerance(word, target) <= tolerance) {
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
  struct dirent *directory;

  // Pedindo ao usuário para inserir a pasta
  printf("Insira o nome da pasta a ser analisada: \n");
  printf("> ");
  scanf("%s", folder);
  printf("\n");

  DIR *dir = opendir(folder);
  if (dir == NULL) {
    printf("Falha ao abrir a pasta");
return 1;
  }

  // Criando uma variável de "extensão" para mostrar ao computador como
  // Percorrer os arquivos do diretório(pasta)
  const char pdf[] = ".pdf";
  const char txt[] = ".txt";
  // Pedindo ao usuário a palavra a ser buscada
  printf("Insira a palavra a ser buscada: \n");
  printf("> ");
  scanf("%s", word);
  printf("\n");

  int tolerance = -1;
  printf("Insira a tolerancia (margem de erro de busca): "
         "\n>");
  scanf("%d", &tolerance);

  // Print necessário do analisando arquivos
  printf("Analizando arquivos... \n");
  printf("\n");

  int id = -1;
  int ocurences[BUFFER_SIZE];
  char files[BUFFER_SIZE][BUFFER_SIZE];
  while ((directory = readdir(dir)) != NULL) {
    // Dados do arquivo
    char filename[BUFFER_SIZE];
    strcpy(filename, directory->d_name);
    size_t filename_len = strlen(filename);

    // Guard clause para tamanhdo do nome
    if (filename_len < 4) {
      continue;
    }

    bool is_pdf = true;
    filename[filename_len] = '\0';

    // Indexador externo
    int j = 0;
    for (size_t i = filename_len - strlen(pdf); i <= filename_len; i++) {
      if (filename[i] != pdf[j]) {
        is_pdf = false;
      }
      j++;
    }

    // Guard clause para tipo de arquivo
    if (!is_pdf) {
      continue;
    }

    id++;

    // Criando o .txt
    char txt_filename[BUFFER_SIZE];
    j = 0;
    for (size_t i = 0; i <= filename_len - strlen(txt); i++) {
      txt_filename[i] = filename[j];
      j++;
    }

    j = 0;
    for (size_t i = filename_len - strlen(txt); i < filename_len; i++) {
      txt_filename[i] = txt[j];
      j++;
    }

    // Criando o "path" do arquivo
    char file_path[BUFFER_SIZE];
    for (size_t i = 0; i <= filename_len + strlen(folder) + 1; i++) {
      if (i <= filename_len) {
        file_path[i] = folder[i];
      } else if (i == filename_len + 1) {
        // Aqui inserimos a '/' no caminho
        // !!! Adendo Importante:
        // No caso do windos, deve ser um '\\'
        // Em linux ou mac, deve ser '/'
        #if defined(_WIN_32) || defined(_WIN_64)
          file_path[i] = '\\';
        #else
          file_path[i] = '/';
        #endif
      } else {
        file_path[i] = txt_filename[i - filename_len - 2];
      }
    }

    // Abrindo o aqruivo pelo path
    txt_filename[strlen(txt_filename)] = '\0';
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
      printf("Falha ao ler arquivo em: %s\n", file_path);
      continue;
    }

    const int ocurence = count_ocurences(word, file, tolerance);
    fclose(file);

    ocurences[id] = ocurence;
    strcpy(files[id], filename);
  }

  printf("Busca finalizada!\n\n");
  printf("Ocorrencias em ordem crescente: \n");

  // Algoritmo de sort
  // Optamos por um simples O(n^2) mesmo pois performance não é essencial
  for (int i = 0; i <= id; i++) {
    for (int j = i; j <= id; j++) {
      if (ocurences[j] < ocurences[i]) {
        int temp_o = ocurences[i];
        char temp_filename[BUFFER_SIZE];
        strcpy(temp_filename, files[i]);

        // int
        ocurences[i] = ocurences[j];
        ocurences[j] = temp_o;

        strcpy(files[i], files[j]);
        strcpy(files[j], temp_filename);
      }
    }
  }

  for (int i = 0; i <= id; i++) {
    printf("Frequencia %d da palavra %s no arquivo %s\n", ocurences[i], word,
           files[i]);
  }

  closedir(dir);
}

int main() { run(); }
