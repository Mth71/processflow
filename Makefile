CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
TARGET = $(SRC_DIR)/processflow

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/interactive.c $(SRC_DIR)/workflow.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean