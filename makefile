SRC_DIR = src
LIB_DIR = lib
BIN_DIR = bin

SRC_FILES = $(SRC_DIR)/dispatcher.c $(SRC_DIR)/queueAlgorithms.c

OUTPUT = $(BIN_DIR)/cpu_schedular

CC = gcc

CFLAGS = -I$(LIB_DIR)

$(OUTPUT): $(SRC_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC_FILES)

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)