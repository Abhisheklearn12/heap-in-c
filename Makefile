
# --- Compiler & Flags ---
CC       := clang
CFLAGS   := -std=c11 -O2 -Wall -Wextra -pedantic -Werror
INCLUDES := -I.
LDFLAGS  :=

# --- Files ---
SRC      := heap.c
OBJ      := $(SRC:.c=.o)
TARGET   := libheap.a
DEMO     := heap_demo

# --- Default rule ---
all: $(TARGET)

# --- Build static library ---
$(TARGET): $(OBJ)
	@echo "  AR     $@"
	ar rcs $@ $^

# --- Build demo executable (with main) ---
demo: CFLAGS += -DDEMO_HEAP_MAIN
demo: $(SRC)
	@echo "  CC     $(DEMO)"
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $(DEMO) $(LDFLAGS)
	@echo "Run demo with: ./heap_demo"

# --- Debug build (no optimizations, with symbols) ---
debug: CFLAGS := -std=c11 -O0 -g3 -Wall -Wextra -pedantic
debug: $(SRC)
	@echo "  CC     $(DEMO) [debug]"
	$(CC) $(CFLAGS) $(INCLUDES) -DDEMO_HEAP_MAIN $^ -o $(DEMO)

# --- Clean up ---
clean:
	@echo "  CLEAN"
	rm -f $(OBJ) $(TARGET) $(DEMO)

# --- Dependencies ---
.PHONY: all demo debug clean
