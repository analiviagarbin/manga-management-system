#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MangaFileHandler.h"
#include "MangaDelimiterFileHandler.h"

Manga mangas[MAX_MANGAS];
int mangaCount = 0;

// Function readString reads string inputs
void readString(char *prompt, char *str, int length) {
    printf("%s", prompt);
    fgets(str, length, stdin);
    str[strcspn(str, "\n")] = '\0'; // Remove newline character
}


// Function readInt reads int inputs
void readInt(char *prompt, int *num) {
    printf("%s", prompt);
    scanf("%d", num);
    getchar(); // Consume newline character left by scanf
}


// Function addManga adds a new manga to the list
void addManga() {
    if (mangaCount >= MAX_MANGAS) {
        printf("Your library is full!\n");
        return;
    }

    Manga m; // new manga
    printf("Enter the data: \n");
    readString("ISBN: ", m.ISBN, 20);
    readString("Title: ", m.title, MAX_TITLE_LEN);
    readString("Author: ", m.author, MAX_AUTHOR_LEN);
    readInt("Start Year: ", &m.startYear);
    readInt("End Year: ", &m.endYear);
    readString("Genre: ", m.genre, MAX_GENRE_LEN);
    readString("Magazine: ", m.magazine, MAX_MAGAZINE_LEN);
    readString("Publisher: ", m.publisher, MAX_PUBLISHER_LEN);
    readInt("Edition Year: ", &m.editionYear);
    readInt("Total Volumes: ", &m.totalVolumes);
    readInt("Acquired Volumes: ", &m.acquiredVolumes);

    for (int i = 0; i < m.acquiredVolumes; i++) {
        printf("Enter Volume %d: ", i + 1);
        scanf("%d", &m.acquiredVolumeList[i]); // receives the informed quantity of volumes sold
    }
    getchar(); // enter

    mangas[mangaCount++] = m; // increments the manga counter
    printf("Manga successfully added to your library!\n");
}


// Function listMangas lists the mangas uploaded according to the counter mangaCount
void listMangas() {
    for (int i = 0; i < mangaCount; i++) {
        printf("Manga %d: %s (%s)\n", i + 1, mangas[i].title, mangas[i].ISBN);
    }
}


// Function updateManga updates the manga according to the ISBN
void updateManga() {
    char ISBN[20];
    readString("Enter ISBN of the manga to update: ", ISBN, 20);

    for (int i = 0; i < mangaCount; i++) {
        if (strcmp(mangas[i].ISBN, ISBN) == 0) {
            printf("Enter the new data: \n");
            readString("Title: ", mangas[i].title, MAX_TITLE_LEN);
            readString("Author: ", mangas[i].author, MAX_AUTHOR_LEN);
            readInt("Start Year: ", &mangas[i].startYear);
            readInt("End Year: ", &mangas[i].endYear);
            readString("Genre: ", mangas[i].genre, MAX_GENRE_LEN);
            readString("Magazine: ", mangas[i].magazine, MAX_MAGAZINE_LEN);
            readString("Publisher: ", mangas[i].publisher, MAX_PUBLISHER_LEN);
            readInt("Edition Year: ", &mangas[i].editionYear);
            readInt("Total Volumes: ", &mangas[i].totalVolumes);
            readInt("Acquired Volumes: ", &mangas[i].acquiredVolumes);

            for (int j = 0; j < mangas[i].acquiredVolumes; j++) {
                printf("Volume %d: ", j + 1);
                scanf("%d", &mangas[i].acquiredVolumeList[j]);
            }
            getchar();

            printf("Manga updated successfully.\n");
            return;
        }
    }

    printf("You don't have this manga in your library yet!\n");
}


// Function deleteManga delete the manga according to the ISBN
void deleteManga() {
    char ISBN[20];
    readString("Enter ISBN of the manga to delete: ", ISBN, 20);

    for (int i = 0; i < mangaCount; i++) {
        if (strcmp(mangas[i].ISBN, ISBN) == 0) {
            printf("Are you sure you want to delete %s? (y|Y/n): ", mangas[i].title);
            char confirm;
            scanf(" %c", &confirm);
            getchar();
            if (confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < mangaCount - 1; j++) {
                    mangas[j] = mangas[j + 1];
                }
                mangaCount--;
                printf("Manga deleted successfully.\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            return;
        }
    }

    printf("You don't have this manga in your library yet!\n");
}


// Function savePrimaryIndex saves in the file according to ISBN and index
void savePrimaryIndex() {
    FILE *file = fopen("primary_index.txt", "w");
    if (file) {
        for (int i = 0; i < mangaCount; i++) {
            fprintf(file, "%s %d\n", mangas[i].ISBN, i);
        }
        fclose(file);
    } else {
        printf("Error opening primary_index.txt for writing.\n");
    }
}


// Function saveSecondaryIndex saves in the file according to Title and index
void saveSecondaryIndex() {
    FILE *file = fopen("secondary_index.txt", "w");
    if (file) {
        for (int i = 0; i < mangaCount; i++) {
            fprintf(file, "%s %d\n", mangas[i].title, i);
        }
        fclose(file);
    } else {
        printf("Error opening secondary_index.txt for writing.\n");
    }
}


// Function saveData saves the index files and updates the data file
void saveData() {
    savePrimaryIndex();
    saveSecondaryIndex();

    FILE *file = fopen("mangas.txt", "w");
    if (file) {
        for (int i = 0; i < mangaCount; i++) {
            Manga m = mangas[i];
            fprintf(file, "%s|%s|%s|%d|%d|%s|%s|%s|%d|%d|%d|",
                    m.ISBN, m.title, m.author, m.startYear, m.endYear,
                    m.genre, m.magazine, m.publisher, m.editionYear,
                    m.totalVolumes, m.acquiredVolumes);
            for (int j = 0; j < m.acquiredVolumes; j++) {
                fprintf(file, "%d", m.acquiredVolumeList[j]);
                if (j < m.acquiredVolumes - 1) {
                    fprintf(file, " ");
                }
            }
            fprintf(file, "|");
        }
        fclose(file);
    } else {
        printf("Error opening mangas.txt for writing.\n");
    }
}


// Function loadData loads the data file into the local data structure
void loadData() {
    FILE *file = fopen("mangas.txt", "r");
    if (file) {
        mangaCount = 0;
        while (!feof(file)) {
            Manga m;
            char volumes[MAX_RECORD_SIZE];
            if (fscanf(file, "%19[^|]|%99[^|]|%99[^|]|%d|%d|%49[^|]|%49[^|]|%49[^|]|%d|%d|%d|%[^|]|",
                       m.ISBN, m.title, m.author, &m.startYear, &m.endYear,
                       m.genre, m.magazine, m.publisher, &m.editionYear,
                       &m.totalVolumes, &m.acquiredVolumes, volumes) == 12) {
                char *token = strtok(volumes, " ");
                int j = 0;
                while (token != NULL && j < MAX_VOLUMES) {
                    m.acquiredVolumeList[j++] = atoi(token);
                    token = strtok(NULL, " ");
                }
                mangas[mangaCount++] = m;
            }
        }
        fclose(file);
    } else {
        printf("Error opening mangas.txt for reading.\n");
    }
}