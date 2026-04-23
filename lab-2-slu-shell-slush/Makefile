CC = gcc
CFLAGS = -Wall -g
TARGET = slush

all: $(TARGET)

$(TARGET): slush.c
	$(CC) $(CFLAGS) -o $(TARGET) slush.c

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run