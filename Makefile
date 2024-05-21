TARGET = MangaManager

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = ./manga_manager/main.c ./manga_manager/MangaDelimiterFileHandler.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: clean
.PHONY: clean distclean

