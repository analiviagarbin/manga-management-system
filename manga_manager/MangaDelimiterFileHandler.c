#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MangaFileHandler.h"
#include "MangaDelimiterFileHandler.h"

Manga mangas[MAX_MANGAS];
int mC = 0; // manga counter 

// Function rInt reads int inputs
void rInt(char *p, int *num) {
    printf("%s", p);
    scanf("%d", num);
    getchar(); // Consume newline character left by scanf
}

// Function rString reads string inputs
void rString(char *p, char *str, int len) {
    printf("%s", p);
    fgets(str, len, stdin);
    str[strcspn(str, "\n")] = '\0';  // Remove newline character
}


// adds a new manga to the list
void addManga() {
    if (mC >= MAX_MANGAS) {
        printf("Error: Your library is full!\n");
        return;
    } 

    // new manga
    Manga m; 
    printf("Enter the data: \n");
    readString("ISBN: ", m.ISBN, 20);
    readString("Title: ", m.title, MAX_TITLE);
    readString("Author: ", m.author, MAX_AUTHOR);
    readInt("Start Year: ", &m.startYear);
    readInt("End Year: ", &m.endYear);
    readString("Genre: ", m.genre, MAX_GENRE);
    readString("Magazine: ", m.magazine, MAX_MAGAZINE);
    readString("Publisher: ", m.publisher, MAX_PUBLISHER);
    readInt("Edition Year: ", &m.editionYear);
    readInt("Total Volumes: ", &m.totalVolumes);
    readInt("Acquired Volumes: ", &m.acquiredVolumes);

    for (int i = 0; i < m.acquiredVolumes; i++) {
        printf("Enter Volume %d: ", i + 1);
        scanf("%d", &m.acquiredVolumeList[i]); // receives the informed quantity of volumes 
    }
    getchar(); 

    printf("Manga successfully added to your library!\n");
    mangas[mC++] = m; 
}


// Function updates the manga according to the ISBN
void updateManga() {
    char ISBN[20];
    readString("Enter ISBN of the manga that you want to update: ", ISBN, 20);

    for (int i = 0; i < mC; i++) {
        if (strcmp(mangas[i].ISBN, ISBN) == 0) {
            printf("Enter the new data: \n");
            readString("Title: ", mangas[i].title, MAX_TITLE);
            readString("Author: ", mangas[i].author, MAX_AUTHOR);
            readInt("Start Year: ", &mangas[i].startYear);
            readInt("End Year: ", &mangas[i].endYear);
            readString("Genre: ", mangas[i].genre, MAX_GENRE);
            readString("Magazine: ", mangas[i].magazine, MAX_MAGAZINE);
            readString("Publisher: ", mangas[i].publisher, MAX_PUBLISHER);
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

// Function lists the mangas uploaded according to the counter mangaCount
void listMangas() {
    for (int i = 0; i < mC; i++) {
        printf("Manga %d: %s (%s)\n", i + 1, mangas[i].title, mangas[i].ISBN);
    }
}


// Function delete the manga according to the ISBN
void deleteManga() {
    char ISBN[20];
    readString("Enter ISBN of the manga you want to delete: ", ISBN, 20);

    for (int i = 0; i < mC; i++) {
        if (strcmp(mangas[i].ISBN, ISBN) == 0) {
            printf("Are you sure you want to delete %s? (y|Y/n): ", mangas[i].title);
            char sure;
            scanf(" %c", &sure);
            getchar();
            if (sure == 'y' || sure == 'Y') {
                for (int j = i; j < mC - 1; j++) {
                    mangas[j] = mangas[j + 1];
                }
                mC--;
                printf("Manga deleted successfully.\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            return;
        }
    }

    printf("Error: You don't have this manga in your library yet!\n");
}


// Function savePrimaryIndex saves in the file according to ISBN and index
void savePrimaryIndex() {
    FILE *file = fopen("primary_index.txt", "w");
    if (file) {
        for (int i = 0; i < mC; i++) {
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
        for (int i = 0; i < mC; i++) {
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
        for (int i = 0; i < mC; i++) {
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
        mC = 0;
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
                mangas[mC++] = m;
            }
        }
        fclose(file);
    } else {
        printf("Error opening mangas.txt for reading.\n");
    }
}
