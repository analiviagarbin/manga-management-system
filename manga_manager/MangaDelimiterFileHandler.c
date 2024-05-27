#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MangaFileHandler.h"
#include "MangaDelimiterFileHandler.h"

void readString(char *prompt, char *str, int length) {
    printf("%s", prompt);
    fgets(str, length, stdin);
    str[strcspn(str, "\n")] = '\0'; // Remove newline character
}

void readInt(char *prompt, int *num) {
    printf("%s", prompt);
    scanf("%d", num);
    getchar(); // Consume newline character left by scanf
}

void addManga() {
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
        scanf("%d", &m.acquiredVolumeList[i]);
    }
    getchar(); // enter

    saveData(m);
    savePrimaryIndex();
    saveSecondaryIndex();
    printf("Manga successfully added to your library!\n");
}

void listMangas() {
    FILE *file = fopen("mangas.txt", "r");
    if (file) {
        char buffer[MAX_RECORD_SIZE];
        while (fgets(buffer, sizeof(buffer), file)) {
            Manga m;
            sscanf(buffer, "%19[^|]|%99[^|]|%99[^|]|%d|%d|%49[^|]|%49[^|]|%49[^|]|%d|%d|%d|",
                   m.ISBN, m.title, m.author, &m.startYear, &m.endYear,
                   m.genre, m.magazine, m.publisher, &m.editionYear,
                   &m.totalVolumes, &m.acquiredVolumes);
            
            printf("\nISBN: %s, Title: %s\n", m.ISBN, m.title);
        }
        fclose(file);
    } else {
        printf("Error opening mangas.txt for reading.\n");
    }
}

void updateManga() {
    char ISBN[20];
    readString("Enter ISBN of the manga to update: ", ISBN, 20);

    FILE *file = fopen("mangas.txt", "r+");
    if (file) {
        Manga m;
        long offset = 0;
        int found = 0;
        char buffer[MAX_RECORD_SIZE];

        while (fgets(buffer, sizeof(buffer), file)) {
            sscanf(buffer, "%19[^|]", m.ISBN);
            if (strcmp(m.ISBN, ISBN) == 0) {
                found = 1;
                break;
            }
            offset = ftell(file);
        }

        if (found) {
            fseek(file, offset, SEEK_SET);
            printf("Enter the new data: \n");
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

            for (int j = 0; j < m.acquiredVolumes; j++) {
                printf("Volume %d: ", j + 1);
                scanf("%d", &m.acquiredVolumeList[j]);
            }
            getchar();

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
            fprintf(file, "|\n");

            savePrimaryIndex();
            saveSecondaryIndex();
            printf("Manga updated successfully.\n");
        } else {
            printf("You don't have this manga in your library yet!\n");
        }

        fclose(file);
    } else {
        printf("Error opening mangas.txt for updating.\n");
    }
}

void deleteManga() {
    char ISBN[20];
    readString("Enter ISBN of the manga to delete: ", ISBN, 20);

    FILE *file = fopen("mangas.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file && tempFile) {
        Manga m;
        int found = 0;
        char buffer[MAX_RECORD_SIZE];

        while (fgets(buffer, sizeof(buffer), file)) {
            sscanf(buffer, "%19[^|]", m.ISBN);
            if (strcmp(m.ISBN, ISBN) == 0) {
                found = 1;
                continue; // Skip this manga
            }
            fprintf(tempFile, "%s", buffer);
        }

        fclose(file);
        fclose(tempFile);

        if (found) {
            printf("Are you sure you want to delete? (y|Y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            getchar();

            if (confirm == 'y' || confirm == 'Y') {
                remove("mangas.txt");
                rename("temp.txt", "mangas.txt");
                savePrimaryIndex();
                saveSecondaryIndex();
                printf("Manga deleted successfully.\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            
            } else {
            remove("temp.txt");
            printf("You don't have this manga in your library yet!\n");
        }
    } else {
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        printf("Error opening files for deletion.\n");
    }
}

void saveData(Manga manga) {
    FILE *file = fopen("mangas.txt", "a"); // Open in append mode
    if (file) {
        Manga m = manga;
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
        fprintf(file, "|\n");
        fclose(file);
    } else {
        printf("Error opening mangas.txt for writing.\n");
    }
}

void savePrimaryIndex() {
    FILE *dataFile = fopen("mangas.txt", "r");
    FILE *indexFile = fopen("primary_index.txt", "w");

    if (dataFile && indexFile) {
        char buffer[MAX_RECORD_SIZE];
        int index = 0;
        while (fgets(buffer, sizeof(buffer), dataFile)) {
            char ISBN[20];
            sscanf(buffer, "%19[^|]", ISBN);
            fprintf(indexFile, "%s %d\n", ISBN, index++);
        }
        fclose(dataFile);
        fclose(indexFile);
    } else {
        if (dataFile) fclose(dataFile);
        if (indexFile) fclose(indexFile);
        printf("Error creating primary index file.\n");
    }
}

void saveSecondaryIndex() {
    FILE *dataFile = fopen("mangas.txt", "r");
    FILE *indexFile = fopen("secondary_index.txt", "w");

    if (dataFile && indexFile) {
        char buffer[MAX_RECORD_SIZE];
        int index = 0;
        while (fgets(buffer, sizeof(buffer), dataFile)) {
            char title[MAX_TITLE_LEN];
            sscanf(buffer, "%*[^|]|%99[^|]", title); // skip ISBN and read title
            fprintf(indexFile, "%s %d\n", title, index++);
        }
        fclose(dataFile);
        fclose(indexFile);
    } else {
        if (dataFile) fclose(dataFile);
        if (indexFile) fclose(indexFile);
        printf("Error creating secondary index file.\n");
    }
}

void searchByPrimaryIndex(char *ISBN) {
    FILE *indexFile = fopen("primary_index.txt", "r");

    if (indexFile) {
        char buffer[50];
        char storedISBN[20];
        int index;
        int found = 0;

        while (fgets(buffer, sizeof(buffer), indexFile)) {
            sscanf(buffer, "%19s %d", storedISBN, &index);
            if (strcmp(storedISBN, ISBN) == 0) {
                found = 1;
                break;
            }
        }

        fclose(indexFile);

        if (found) {
            searchByIndex(index);
        } else {
            printf("Manga with ISBN %s not found.\n", ISBN);
        }
    } else {
        printf("Error opening primary index file for reading.\n");
    }
}

void searchBySecondaryIndex(char *title) {
    FILE *indexFile = fopen("secondary_index.txt", "r");

    if (indexFile) {
        char buffer[150];
        char storedTitle[MAX_TITLE_LEN];
        int index;
        int found = 0;

        while (fgets(buffer, sizeof(buffer), indexFile)) {
            sscanf(buffer, "%99[^|]| %d", storedTitle, &index);
            if (strcmp(storedTitle, title) == 0) {
                found = 1;
                break;
            }
        }

        fclose(indexFile);

        if (found) {
            searchByIndex(index);
        } else {
            printf("Manga with title %s not found.\n", title);
        }
    } else {
        printf("Error opening secondary index file for reading.\n");
    }
}

void searchByIndex(int recordNumber) {
    FILE *dataFile = fopen("mangas.txt", "r");
    FILE *primaryIndexFile = fopen("primary_index.txt", "r");

    if (dataFile == NULL) {
        printf("Error opening mangas.txt for reading.\n");
        return;
    }

    if (primaryIndexFile == NULL) {
        printf("Error opening primary_index.txt for reading.\n");
        fclose(dataFile);
        return;
    }

    char buffer[50];
    char ISBN[20];
    int index;
    int found = 0;

    while (fgets(buffer, sizeof(buffer), primaryIndexFile)) {
        sscanf(buffer, "%19s %d", ISBN, &index);
        if (index == recordNumber) {
            found = 1;
            break;
        }
    }

    fclose(primaryIndexFile);

    if (found) {
        while (fgets(buffer, sizeof(buffer), dataFile)) {
            if (strstr(buffer, ISBN) != NULL) {
                Manga m;
                sscanf(buffer, "%19[^|]|%99[^|]|%99[^|]|%d|%d|%49[^|]|%49[^|]|%49[^|]|%d|%d|%d|",
                       m.ISBN, m.title, m.author, &m.startYear, &m.endYear,
                       m.genre, m.magazine, m.publisher, &m.editionYear,
                       &m.totalVolumes, &m.acquiredVolumes);
                printf("Record %d:\n", recordNumber);
                printf("ISBN: %s\n", m.ISBN);
                printf("Title: %s\n", m.title);
                printf("Author: %s\n", m.author);
                printf("Start Year: %d\n", m.startYear);
                printf("End Year: %d\n", m.endYear);
                printf("Genre: %s\n", m.genre);
                printf("Magazine: %s\n", m.magazine);
                printf("Publisher: %s\n", m.publisher);
                printf("Edition Year: %d\n", m.editionYear);
                printf("Total Volumes: %d\n", m.totalVolumes);
                printf("Acquired Volumes: %d\n", m.acquiredVolumes);
                printf("Acquired Volume List: ");
                char *token = strtok(buffer, "|");
                for (int i = 0; i < 11; i++) {
                    token = strtok(NULL, "|");
                }
                char *volumes = strtok(token, " ");
                while (volumes != NULL) {
                    printf("%s ", volumes);
                    volumes = strtok(NULL, " ");
                }
                printf("\n\n");
                break;
            }
        }
    } else {
        printf("Record %d not found.\n", recordNumber);
    }

    fclose(dataFile);
}
