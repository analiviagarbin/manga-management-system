/*
Manga Management System

Ana Flor de O. Stefani 4770143
Ana Lívia de M. Garbin 14557394
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_TITULO 100
#define TAM_AUTOR 100
#define TAM_GENERO 50
#define TAM_REVISTA 50
#define TAM_EDITORA 50
#define MAX_VOLUMES 100

typedef struct {
    char isbn[14]; // 13 caracteres + '\0'
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    int anoInicio;
    int anoFim;
    char genero[TAM_GENERO];
    char revista[TAM_REVISTA];
    char editora[TAM_EDITORA];
    int anoEdicao;
    int qtdVolumes;
    int qtdVolumesAdquiridos;
    int volumesAdquiridos[MAX_VOLUMES];
    int flag; // FLAG
} Manga;

typedef struct {
    char isbn[14];
    long offset;
} IndicePrimario;

typedef struct {
    char titulo[TAM_TITULO];
    char isbn[14];
} IndiceSecundario;

void criarRegistro(FILE *fpDados, FILE *fpIndPrim, FILE *fpIndSec);
void lerRegistro(FILE *fpDados, FILE *fpIndPrim);
void alterarRegistro(FILE *fpDados, FILE *fpIndPrim, FILE *fpIndSec);
void apagarRegistro(FILE *fpDados, FILE *fpIndPrim, FILE *fpIndSec);
void recriarArquivos();
long buscarISBN(FILE *fpIndPrim, char *isbn);
long buscarTitulo(FILE *fpIndSec, char *titulo);
void inicializarArquivos(FILE **fpDados, FILE **fpIndPrim, FILE **fpIndSec);
void pesquisarPorTitulo(FILE *fpDados, FILE *fpIndSec);

void lerString(char *str, int tamanho);

FILE *fpDados, *fpIndPrim, *fpIndSec;

int main() {
    inicializarArquivos(&fpDados, &fpIndPrim, &fpIndSec);

    int escolha;
    do {
        printf("\n============= Gerenciador de Mangás =============\n\n1. Criar Registro\n2. Ler Registro\n3. Alterar Registro\n4. Apagar Registro\n5. Pesquisar por Titulo\n6. Sair\nEscolha: ");
        scanf("%d", &escolha);
        getchar(); // limpa o '\n'
        switch (escolha) {
            case 1:
                criarRegistro(fpDados, fpIndPrim, fpIndSec);
                break;
            case 2:
                lerRegistro(fpDados, fpIndPrim);
                break;
            case 3:
                alterarRegistro(fpDados, fpIndPrim, fpIndSec);
                break;
            case 4:
                apagarRegistro(fpDados, fpIndPrim, fpIndSec);
                break;
            case 5:
                pesquisarPorTitulo(fpDados, fpIndSec);
                break;
            case 6:
                recriarArquivos();
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != 6);

    fclose(fpDados);
    fclose(fpIndPrim);
    fclose(fpIndSec);
    return 0;
}

void inicializarArquivos(FILE **fpDados, FILE **fpIndPrim, FILE **fpIndSec) {
    *fpDados = fopen("mangas.txt", "a+");
    if (*fpDados == NULL) {
        perror("Erro ao abrir o arquivo de dados");
        exit(1);
    }
    *fpIndPrim = fopen("indice_primario.txt", "a+");
    if (*fpIndPrim == NULL) {
        perror("Erro ao abrir o arquivo de índice primário");
        exit(1);
    }
    *fpIndSec = fopen("indice_secundario.txt", "a+");
    if (*fpIndSec == NULL) {
        perror("Erro ao abrir o arquivo de índice secundário");
        exit(1);
    }
}

void lerString(char *str, int tamanho) {
    fgets(str, tamanho, stdin);
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void criarRegistro(FILE *fpDados, FILE *fpIndPrim, FILE *fpIndSec) {
    Manga novoManga;
    printf("ISBN: ");
    lerString(novoManga.isbn, 14);
    printf("Título: ");
    lerString(novoManga.titulo, TAM_TITULO);
    printf("Autor: ");
    lerString(novoManga.autor, TAM_AUTOR);
    printf("Ano de Início: ");
    scanf("%d", &novoManga.anoInicio);
    printf("Ano de Fim: ");
    scanf("%d", &novoManga.anoFim);
    getchar();
    printf("Gênero: ");
    lerString(novoManga.genero, TAM_GENERO);
    printf("Revista: ");
    lerString(novoManga.revista, TAM_REVISTA);
    printf("Editora: ");
    lerString(novoManga.editora, TAM_EDITORA);
    printf("Ano da Edição: ");
    scanf("%d", &novoManga.anoEdicao);
    printf("Quantidade de Volumes: ");
    scanf("%d", &novoManga.qtdVolumes);
    printf("Quantidade de Volumes Adquiridos: ");
    scanf("%d", &novoManga.qtdVolumesAdquiridos);
    getchar();
    printf("Lista de Volumes Adquiridos: ");
    for (int i = 0; i < novoManga.qtdVolumesAdquiridos; i++) {
        scanf("%d", &novoManga.volumesAdquiridos[i]);
    }
    getchar();
    novoManga.flag = 0; // Registro novo não está desatualizado

    fseek(fpDados, 0, SEEK_END);
    long offset = ftell(fpDados);
    fprintf(fpDados, "%s|%s|%s|%d|%d|%s|%s|%s|%d|%d|%d|%d|", novoManga.isbn, novoManga.titulo, novoManga.autor, novoManga.anoInicio, novoManga.anoFim, novoManga.genero, novoManga.revista, novoManga.editora, novoManga.anoEdicao, novoManga.qtdVolumes, novoManga.qtdVolumesAdquiridos, novoManga.flag);
    for (int i = 0; i < novoManga.qtdVolumesAdquiridos; i++) {
        fprintf(fpDados, "%d", novoManga.volumesAdquiridos[i]);
        if (i < novoManga.qtdVolumesAdquiridos - 1) {
            fprintf(fpDados, ",");
        }
    }
    fprintf(fpDados, "\n");

    fseek(fpIndPrim, 0, SEEK_END);
    fprintf(fpIndPrim, "%s|%ld\n", novoManga.isbn, offset);

    fseek(fpIndSec, 0, SEEK_END);
    fprintf(fpIndSec, "%s|%s\n", novoManga.titulo, novoManga.isbn);
}

void lerRegistro(FILE *fpDados, FILE *fpIndPrim) {
    char isbn[14];
    printf("ISBN: ");
    lerString(isbn, 14);

    long offset = buscarISBN(fpIndPrim, isbn);
    if (offset == -1) {
        printf("Registro não encontrado.\n");
        return;
    }

    fseek(fpDados, offset, SEEK_SET);
    Manga manga;
    fscanf(fpDados, "%13[^|]|%99[^|]|%99[^|]|%d|%d|%49[^|]|%49[^|]|%49[^|]|%d|%d|%d|%d|", manga.isbn, manga.titulo, manga.autor, &manga.anoInicio, &manga.anoFim, manga.genero, manga.revista, manga.editora, &manga.anoEdicao, &manga.qtdVolumes, &manga.qtdVolumesAdquiridos, &manga.flag);
    for (int i = 0; i < manga.qtdVolumesAdquiridos; i++) {
        if (i < manga.qtdVolumesAdquiridos - 1) {
            fscanf(fpDados, "%d,", &manga.volumesAdquiridos[i]);
        } else {
            fscanf(fpDados, "%d\n", &manga.volumesAdquiridos[i]);
        }
    }

    printf("ISBN: %s\n", manga.isbn);
    printf("Título: %s\n", manga.titulo);
    printf("Autor: %s\n", manga.autor);
    printf("Ano de Início: %d\n", manga.anoInicio);
    printf("Ano de Fim: %d\n", manga.anoFim);
    printf("Gênero: %s\n", manga.genero);
    printf("Revista: %s\n", manga.revista);
    printf("Editora: %s\n", manga.editora);
    printf("Ano da Edição: %d\n", manga.anoEdicao);
    printf("Quantidade de Volumes: %d\n", manga.qtdVolumes);
    printf("Quantidade de Volumes Adquiridos: %d\n", manga.qtdVolumesAdquiridos);
    printf("Volumes Adquiridos: ");
    for (int i = 0; i < manga.qtdVolumesAdquiridos; i++) {
        printf("%d ", manga.volumesAdquiridos[i]);
    }
    printf("\n");
}

void alterarRegistro(FILE *fpDados, FILE *fpIndPrim, FILE *fpIndSec) {
    char isbn[14];
    printf("ISBN: ");
    lerString(isbn, 14);

    long offset = buscarISBN(fpIndPrim, isbn);
    if (offset == -1) {
        printf("Registro não encontrado.\n");
        return;
    }

    // marca registro existente como desatualizado
    FILE *fpDadosTemp = fopen("mangas_temp.txt", "w");
    rewind(fpDados);
    char linha[1024];
    while (fgets(linha, sizeof(linha), fpDados)) {
        Manga manga;
        sscanf(linha, "%13[^|]|%99[^|]|%99[^|]|%d|%d|%49[^|]|%49[^|]|%49[^|]|%d|%d|%d|%d|", manga.isbn, manga.titulo, manga.autor, &manga.anoInicio, &manga.anoFim, manga.genero, manga.revista, manga.editora, &manga.anoEdicao, &manga.qtdVolumes, &manga.qtdVolumesAdquiridos, &manga.flag);
        
        //  le a lista de volumes adquiridos
        int index = 0;
        char *ptr = strchr(linha, '|');
        for (int i = 0; i < 11; i++) {
            ptr = strchr(ptr + 1, '|');
        }
        char *token = strtok(ptr + 1, ",\n");
        while (token != NULL) {
            manga.volumesAdquiridos[index++] = atoi(token);
            token = strtok(NULL, ",\n");
        }

        if (strcmp(manga.isbn, isbn) == 0) {
            manga.flag = 1;
        }

        fprintf(fpDadosTemp, "%s|%s|%s|%d|%d|%s|%s|%s|%d|%d|%d|%d|", manga.isbn, manga.titulo, manga.autor, manga.anoInicio, manga.anoFim, manga.genero, manga.revista, manga.editora, manga.anoEdicao, manga.qtdVolumes, manga.qtdVolumesAdquiridos, manga.flag);
        for (int i = 0; i < manga.qtdVolumesAdquiridos; i++) {
            fprintf(fpDadosTemp, "%d", manga.volumesAdquiridos[i]);
            if (i < manga.qtdVolumesAdquiridos - 1) {
                fprintf(fpDadosTemp, ",");
            }
        }
        fprintf(fpDadosTemp, "\n");
    }

    fclose(fpDadosTemp);
    fclose(fpDados);

    remove("mangas.txt");
    rename("mangas_temp.txt", "mangas.txt");

    fpDados = fopen("mangas.txt", "a+");

    // Solicita as novas informações
    Manga mangaAlterado;
    strcpy(mangaAlterado.isbn, isbn); // Manter o ISBN

    printf("Novo Título: ");
    lerString(mangaAlterado.titulo, TAM_TITULO);
    printf("Novo Autor: ");
    lerString(mangaAlterado.autor, TAM_AUTOR);
    printf("Novo Ano de Início: ");
    scanf("%d", &mangaAlterado.anoInicio);
    printf("Novo Ano de Fim: ");
    scanf("%d", &mangaAlterado.anoFim);
    getchar(); 
    printf("Novo Gênero: ");
    lerString(mangaAlterado.genero, TAM_GENERO);
    printf("Nova Revista: ");
    lerString(mangaAlterado.revista, TAM_REVISTA);
    printf("Nova Editora: ");
    lerString(mangaAlterado.editora, TAM_EDITORA);
    printf("Novo Ano da Edição: ");
    scanf("%d", &mangaAlterado.anoEdicao);
    printf("Nova Quantidade de Volumes: ");
    scanf("%d", &mangaAlterado.qtdVolumes);
    printf("Nova Quantidade de Volumes Adquiridos: ");
    scanf("%d", &mangaAlterado.qtdVolumesAdquiridos);
    getchar(); 
    printf("Nova Lista de Volumes Adquiridos: ");
    for (int i = 0; i < mangaAlterado.qtdVolumesAdquiridos; i++) {
        scanf("%d", &mangaAlterado.volumesAdquiridos[i]);
    }
    getchar(); 
    mangaAlterado.flag = 0; // novo registro não está desatualizado

    fseek(fpDados, 0, SEEK_END);
    long novoOffset = ftell(fpDados);
    fprintf(fpDados, "%s|%s|%s|%d|%d|%s|%s|%s|%d|%d|%d|%d|", mangaAlterado.isbn, mangaAlterado.titulo, mangaAlterado.autor, mangaAlterado.anoInicio, mangaAlterado.anoFim, mangaAlterado.genero, mangaAlterado.revista, mangaAlterado.editora, mangaAlterado.anoEdicao, mangaAlterado.qtdVolumes, mangaAlterado.qtdVolumesAdquiridos, mangaAlterado.flag);
    for (int i = 0; i < mangaAlterado.qtdVolumesAdquiridos; i++) {
        fprintf(fpDados, "%d", mangaAlterado.volumesAdquiridos[i]);
        if (i < mangaAlterado.qtdVolumesAdquiridos - 1) {
            fprintf(fpDados, ",");
        }
    }
    fprintf(fpDados, "\n");

    fseek(fpIndPrim, 0, SEEK_END);
    fprintf(fpIndPrim, "%s|%ld\n", mangaAlterado.isbn, novoOffset);

    fseek(fpIndSec, 0, SEEK_END);
    fprintf(fpIndSec, "%s|%s\n", mangaAlterado.titulo, mangaAlterado.isbn);
}

void apagarRegistro(FILE *fpDados, FILE *fpIndPrim, FILE *fpIndSec) {
    char isbn[14];
    printf("ISBN: ");
    lerString(isbn, 14);

    printf("Tem certeza que deseja apagar o registro (S/N)? ");
    char confirmacao;
    scanf(" %c", &confirmacao);
    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Operação cancelada.\n");
        return;
    }

    long offset = buscarISBN(fpIndPrim, isbn);
    if (offset == -1) {
        printf("Registro não encontrado.\n");
        return;
    }

    // marca o registro existente como desatualizado
    FILE *fpDadosTemp = fopen("mangas_temp.txt", "w");
    rewind(fpDados);
    char linha[1024];
    while (fgets(linha, sizeof(linha), fpDados)) {
        Manga manga;
        sscanf(linha, "%13[^|]|%99[^|]|%99[^|]|%d|%d|%49[^|]|%49[^|]|%49[^|]|%d|%d|%d|%d|", manga.isbn, manga.titulo, manga.autor, &manga.anoInicio, &manga.anoFim, manga.genero, manga.revista, manga.editora, &manga.anoEdicao, &manga.qtdVolumes, &manga.qtdVolumesAdquiridos, &manga.flag);
        
        // le a lista de volumes adquiridos
        int index = 0;
        char *ptr = strchr(linha, '|');
        for (int i = 0; i < 11; i++) {
            ptr = strchr(ptr + 1, '|');
        }
        char *token = strtok(ptr + 1, ",\n");
        while (token != NULL) {
            manga.volumesAdquiridos[index++] = atoi(token);
            token = strtok(NULL, ",\n");
        }

        if (strcmp(manga.isbn, isbn) == 0) {
            manga.flag = 1;
        }

        fprintf(fpDadosTemp, "%s|%s|%s|%d|%d|%s|%s|%s|%d|%d|%d|%d|", manga.isbn, manga.titulo, manga.autor, manga.anoInicio, manga.anoFim, manga.genero, manga.revista, manga.editora, manga.anoEdicao, manga.qtdVolumes, manga.qtdVolumesAdquiridos, manga.flag);
        for (int i = 0; i < manga.qtdVolumesAdquiridos; i++) {
            fprintf(fpDadosTemp, "%d", manga.volumesAdquiridos[i]);
            if (i < manga.qtdVolumesAdquiridos - 1) {
                fprintf(fpDadosTemp, ",");
            }
        }
        fprintf(fpDadosTemp, "\n");
    }

    fclose(fpDadosTemp);
    fclose(fpDados);

    remove("mangas.txt");
    rename("mangas_temp.txt", "mangas.txt");

    fpDados = fopen("mangas.txt", "a+");
}

long buscarISBN(FILE *fpIndPrim, char *isbn) {
    rewind(fpIndPrim);
    char linha[256];
    while (fgets(linha, sizeof(linha), fpIndPrim)) {
        char isbnIndice[14];
        long offset;
        sscanf(linha, "%13[^|]|%ld", isbnIndice, &offset);
        if (strcmp(isbnIndice, isbn) == 0) {
            return offset;
        }
    }
    return -1;
}

long buscarTitulo(FILE *fpIndSec, char *titulo) {
    rewind(fpIndSec);
    char linha[256];
    while (fgets(linha, sizeof(linha), fpIndSec)) {
        char tituloIndice[TAM_TITULO];
        char isbn[14];
        sscanf(linha, "%99[^|]|%13s", tituloIndice, isbn);
        if (strcmp(tituloIndice, titulo) == 0) {
            return buscarISBN(fpIndPrim, isbn);
        }
    }
    return -1;
}

void pesquisarPorTitulo(FILE *fpDados, FILE *fpIndSec) {
    char titulo[TAM_TITULO];
    printf("Título: ");
    lerString(titulo, TAM_TITULO);

    long offset = buscarTitulo(fpIndSec, titulo);
    if (offset == -1) {
        printf("Registro não encontrado.\n");
        return;
    }

    fseek(fpDados, offset, SEEK_SET);
    Manga manga;
    fscanf(fpDados, "%13[^|]|%99[^|]|%99[^|]|%d|%d|%49[^|]|%49[^|]|%49[^|]|%d|%d|%d|%d|", manga.isbn, manga.titulo, manga.autor, &manga.anoInicio, &manga.anoFim, manga.genero, manga.revista, manga.editora, &manga.anoEdicao, &manga.qtdVolumes, &manga.qtdVolumesAdquiridos, &manga.flag);
    for (int i = 0; i < manga.qtdVolumesAdquiridos; i++) {
        if (i < manga.qtdVolumesAdquiridos - 1) {
            fscanf(fpDados, "%d,", &manga.volumesAdquiridos[i]);
        } else {
            fscanf(fpDados, "%d\n", &manga.volumesAdquiridos[i]);
        }
    }

    printf("ISBN: %s\n", manga.isbn);
    printf("Título: %s\n", manga.titulo);
    printf("Autor: %s\n", manga.autor);
    printf("Ano de Início: %d\n", manga.anoInicio);
    printf("Ano de Fim: %d\n", manga.anoFim);
    printf("Gênero: %s\n", manga.genero);
    printf("Revista: %s\n", manga.revista);
    printf("Editora: %s\n", manga.editora);
    printf("Ano da Edição: %d\n", manga.anoEdicao);
    printf("Quantidade de Volumes: %d\n", manga.qtdVolumes);
    printf("Quantidade de Volumes Adquiridos: %d\n", manga.qtdVolumesAdquiridos);
    printf("Volumes Adquiridos: ");
    for (int i = 0; i < manga.qtdVolumesAdquiridos; i++) {
        printf("%d ", manga.volumesAdquiridos[i]);
    }
    printf("\n");
}

void recriarArquivos() {
    FILE *fpDadosTemp = fopen("mangas_temp.txt", "w");
    FILE *fpIndPrimTemp = fopen("indice_primario_temp.txt", "w");
    FILE *fpIndSecTemp = fopen("indice_secundario_temp.txt", "w");

    rewind(fpDados);
    char linha[1024];
    long offset = 0;
    while (fgets(linha, sizeof(linha), fpDados)) {
        Manga manga;
        sscanf(linha, "%13[^|]|%99[^|]|%99[^|]|%d|%d|%49[^|]|%49[^|]|%49[^|]|%d|%d|%d|%d|", manga.isbn, manga.titulo, manga.autor, &manga.anoInicio, &manga.anoFim, manga.genero, manga.revista, manga.editora, &manga.anoEdicao, &manga.qtdVolumes, &manga.qtdVolumesAdquiridos, &manga.flag);

        //  le a lista de volumes adquiridos
        int index = 0;
        char *ptr = strchr(linha, '|');
        for (int i = 0; i < 11; i++) {
            ptr = strchr(ptr + 1, '|');
        }
        char *token = strtok(ptr + 1, ",\n");
        while (token != NULL) {
            manga.volumesAdquiridos[index++] = atoi(token);
            token = strtok(NULL, ",\n");
        }

        if (manga.flag == 0) { // registros atualizados
            fprintf(fpDadosTemp, "%s|%s|%s|%d|%d|%s|%s|%s|%d|%d|%d|%d|", manga.isbn, manga.titulo, manga.autor, manga.anoInicio, manga.anoFim, manga.genero, manga.revista, manga.editora, manga.anoEdicao, manga.qtdVolumes, manga.qtdVolumesAdquiridos, manga.flag);
            for (int i = 0; i < manga.qtdVolumesAdquiridos; i++) {
                fprintf(fpDadosTemp, "%d", manga.volumesAdquiridos[i]);
                if (i < manga.qtdVolumesAdquiridos - 1) {
                    fprintf(fpDadosTemp, ",");
                }
            }
            fprintf(fpDadosTemp, "\n");

            fprintf(fpIndPrimTemp, "%s|%ld\n", manga.isbn, offset);
            fprintf(fpIndSecTemp, "%s|%s\n", manga.titulo, manga.isbn);
            offset = ftell(fpDadosTemp);
        }
    }

    fclose(fpDadosTemp);
    fclose(fpIndPrimTemp);
    fclose(fpIndSecTemp);

    fclose(fpDados);
    fclose(fpIndPrim);
    fclose(fpIndSec);

    remove("mangas.txt");
    remove("indice_primario.txt");
    remove("indice_secundario.txt");

    rename("mangas_temp.txt", "mangas.txt");
    rename("indice_primario_temp.txt", "indice_primario.txt");
    rename("indice_secundario_temp.txt", "indice_secundario.txt");

    inicializarArquivos(&fpDados, &fpIndPrim, &fpIndSec);
}
