#ifndef MANGA_FILE_HANDLER_H
#define MANGA_FILE_HANDLER_H

#define MAX_MANGAS 100
#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100
#define MAX_GENRE_LEN 50
#define MAX_MAGAZINE_LEN 50
#define MAX_PUBLISHER_LEN 50
#define MAX_VOLUMES 100
#define MAX_RECORD_SIZE 1024

typedef struct {
    char ISBN[20];
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    int startYear;
    int endYear;
    char genre[MAX_GENRE_LEN];
    char magazine[MAX_MAGAZINE_LEN];
    char publisher[MAX_PUBLISHER_LEN];
    int editionYear;
    int totalVolumes;
    int acquiredVolumes;
    int acquiredVolumeList[MAX_VOLUMES];
} Manga;

#endif