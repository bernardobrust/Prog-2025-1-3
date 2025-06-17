#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define BUFFER_SIZE 1024 // Tamanho máximo de uma palavra

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
                matrix[i][j] = (int) j;
            } else if (j == 0) {
                matrix[i][j] = (int) i;
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
[[nodiscard]] int count_ocurences(const char target[], FILE *file) {
    int count = 0;

    char word[BUFFER_SIZE];
    // Loop que lê o arquivo palavra por palavra
    while (fscanf(file, "%s", word) == 1) {
        // Verificando se target e word batem (com tolerancia 1)
        if (strcmp(word, target) == 0) {
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

    // Print nescessário do analizando arquivos
    printf("Analizando arquivos... \n");
    printf("\n");

    int id = -1;
    int ocurences[BUFFER_SIZE];
    char files[BUFFER_SIZE][BUFFER_SIZE];
    while ((directory = readdir(dir)) != NULL) {
        if (directory->d_namlen < 4) continue;

        bool is_pdf = true;

        directory->d_name[strlen(directory->d_name)] = '\0';
        int j = 0;
        for (size_t i = strlen(directory->d_name) - strlen(pdf); i <= strlen(directory->d_name); i++) {
            if (directory->d_name[i] != pdf[j]) {
                is_pdf = false;
            }
            j++;
        }

        if (is_pdf) {
            id++;

            char txt_filename[BUFFER_SIZE];
            j = 0;
            for (size_t i = 0; i <= strlen(directory->d_name) - strlen(txt); i++) {
                txt_filename[i] = directory->d_name[j];
                j++;
            }

            j = 0;
            for (size_t i = strlen(directory->d_name) - strlen(txt); i < strlen(directory->d_name); i++) {
                txt_filename[i] = txt[j];
                j++;
            }

            char file_path[BUFFER_SIZE];
            for (size_t i = 0; i <= strlen(directory->d_name) + strlen(folder) + 1; i++) {
                if (i <= strlen(directory->d_name)) file_path[i] = folder[i];
                else if (i == strlen(directory->d_name) + 1) file_path[i] = '\\';
                else file_path[i] = txt_filename[i - strlen(directory->d_name) - 2];
            }

            txt_filename[strlen(txt_filename)] = '\0';
            FILE *file = fopen(file_path, "r");
            if (file == NULL) {
                printf("Falha ao ler arquivo\n");
            }

            const int ocurence = count_ocurences(word, file);
            fclose(file);

            ocurences[id] = ocurence;
            strcpy(files[id], directory->d_name);
        }
    }

    printf("Busca finalizada!\n\n");
    printf("Ocorrencias em ordem crescente: \n");

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

    for (int i =  0; i <= id; i++) {
        printf("Frequencia %d da palavra %s no arquivo %s\n", ocurences[i], word, files[i]);
    }

    closedir(dir);
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
