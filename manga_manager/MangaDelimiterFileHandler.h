#ifndef MANGA_DELIMITER_FILE_HANDLER_H
#define MANGA_DELIMITER_FILE_HANDLER_H

void rInt(char *p, int *num);
void rString(char *p, char *str, int length);
void addManga();
void listMangas();
void updateManga();
void deleteManga();
void saveData(Manga manga);
void searchByIndex(int recordNumber);
void savePrimaryIndex();
void saveSecondaryIndex();
void searchByPrimaryIndex(char *ISBN);
void searchBySecondaryIndex(char *title);

#endif
