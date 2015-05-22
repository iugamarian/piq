# GENERAL
CC = gcc
STANDARD = -std=gnu99


# DIRS
BIN_DIR = $(PWD)/bin
OBJ_DIR = $(PWD)/obj


# INCLUDE AND LIBRARY PATHS
INCLUDES = -I$(PWD)/include -I/usr/local/include/
LIBS = -L/usr/local/lib -lbcm2835


# COMPILER FLAGS
DEBUG_FLAGS = -g3 -gdwarf-2
WARN_FLAGS = -Wall -Wstrict-prototypes

CFLAGS = $(DEBUG_FLAGS) \
			$(WARN_FLAGS) \
			$(STANDARD) \
			$(INCLUDES)


# COMMANDS
# MAKE_OBJ = \
# 	@echo "MAKE OBJ [$@]"; \
# 	$(CC) -c $@.c -o $(OBJ_DIR)/$@.o $(CFLAGS) $(LIBS)

MAKE_EXE = \
	echo "EXE [$@]"; \
	$(CC) $(CFLAGS) -c $@.c -o $(OBJ_DIR)/$@.o; \
	$(CC) $(OBJ_DIR)/$@.o -o $(BIN_DIR)/$@ $(LIBS);
