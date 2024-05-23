#ifndef MANGA_FILE_HANDLER_H
#define MANGA_FILE_HANDLER_H

#define MAX_MANGAS 100
#define MAX_TITLE 100
#define MAX_AUTHOR 100
#define MAX_GENRE 50
#define MAX_MAGAZINE 50
#define MAX_PUBLISHER 50
#define MAX_VOLUMES 100
#define MAX_RECORD_SIZE 1024

typedef struct {
    char ISBN[20];
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int startYear;
    int endYear;
    char genre[MAX_GENRE];
    char magazine[MAX_MAGAZINE];
    char publisher[MAX_PUBLISHER];
    int editionYear;
    int totalVolumes;
    int acquiredVolumes;
    int acquiredVolumeList[MAX_VOLUMES];
} Manga;

#endif
