# GENERAL
CC = gcc
STANDARD = -std=gnu99


# DIRS
BIN_DIR = $(PWD)/bin
OBJ_DIR = $(PWD)/obj
LIB_DIR = $(PWD)/lib


# INCLUDE AND LIBRARY PATHS
INCLUDES = -I$(PWD)/include -I/usr/local/include/
LIBS = -L/usr/local/lib -L/usr/lib -L$(LIB_DIR) -lpiq -lbcm2835 -lm -lpthread


# COMPILER FLAGS
DEBUG_FLAGS = -g -gdwarf-2
WARN_FLAGS = -Wall -Wstrict-prototypes

CFLAGS = $(DEBUG_FLAGS) \
			$(WARN_FLAGS) \
			$(STANDARD) \
			$(INCLUDES)


# COMMANDS
MAKE_EXE = \
	echo "CC [$@]"; \
	$(CC) $(CFLAGS) -c $@.c -o $(OBJ_DIR)/$@.o; \
	$(CC) $(OBJ_DIR)/$@.o -o $(BIN_DIR)/$@ $(LIBS);

MAKE_OBJ = \
	@echo "CC [$<]"; \
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $@);

MAKE_STATIC_LIB = \
	@echo "AR [$@]"; \
	$(AR) $(ARFLAGS) $(LIB_DIR)/$@.a $(wildcard $(OBJ_DIR)/*.o);

MAKE_TEST = \
	echo "TEST [$@]"; \
	$(CC) $(CFLAGS) -c $@.c -o $(OBJ_DIR)/$@.o; \
	$(CC) $(OBJ_DIR)/$@.o -o $(BIN_DIR)/$@ $(LIBS);
