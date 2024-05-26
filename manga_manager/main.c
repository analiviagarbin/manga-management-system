/*
Manga Management System

Ana Flor de O. Stefani 4770143
Ana Lívia de M. Garbin 14557394
*/

#include <stdio.h>
#include "MangaFileHandler.h"
#include "MangaDelimiterFileHandler.h"

void menu() {
    int c;
    do {
        printf("\n============== Manga Management System ==============\n");
        printf("1. Add Manga\n");
        printf("2. List Mangas\n");
        printf("3. Update Manga\n");
        printf("4. Delete Manga\n");
        printf("5. Search Manga by Index\n");
        printf("6. Exit\n");
        printf("What are we doing today? ");
        scanf("%d", &c);
        getchar(); // consume newline character left by scanf

        switch (c) {
            case 1: addManga(); break;
            case 2: listMangas(); break;
            case 3: updateManga(); break;
            case 4: deleteManga(); break;
            case 5: {
                int recordNumber;
                printf("Enter record number to search: ");
                scanf("%d", &recordNumber);
                getchar(); // consume newline character left by scanf
                searchByIndex(recordNumber);
                break;
            }
            case 6: printf("Bye bye...\n"); break;
            default: printf("Invalid choice, try again.\n");
        }
    } while (c != 6);
}

int main() {
    menu();
    return 0;
}
