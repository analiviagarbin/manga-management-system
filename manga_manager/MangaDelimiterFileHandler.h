#ifndef MANGA_DELIMITER_FILE_HANDLER_H
#define MANGA_DELIMITER_FILE_HANDLER_H

void readString(char *prompt, char *str, int length);
void readInt(char *prompt, int *num);
void addManga();
void listMangas();
void updateManga();
void deleteManga();
void savePrimaryIndex();
void saveSecondaryIndex();
void saveData();
void loadData();

#endif