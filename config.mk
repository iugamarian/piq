# GENERAL
CXX = g++
# STANDARD = -std=gnu99


# DIRS
BUILD_PATH = $(PWD)/build
BIN_DIR = $(BUILD_PATH)/bin
OBJ_DIR = $(BUILD_PATH)/obj
LIB_DIR = $(BUILD_PATH)/lib


# INCLUDE AND LIBRARY PATHS
INCLUDES = -I$(PWD)/include -I/usr/local/include/
LIBS = -L/usr/local/lib \
	   -L/usr/lib \
	   -L$(LIB_DIR) \
	   -lm \
	   -lpthread


# COMPILER FLAGS
DEBUG_FLAGS = -g -gdwarf-2
WARN_FLAGS = -Wall

CXXFLAGS = $(DEBUG_FLAGS) \
			$(WARN_FLAGS) \
			$(STANDARD) \
			$(INCLUDES)


# COMMANDS
MAKE_EXE = \
	echo "CXX [$@]"; \
	$(CXX) $(CXXFLAGS) -c $@.c -o $(OBJ_DIR)/$@.o; \
	$(CXX) $(OBJ_DIR)/$@.o -o $(BIN_DIR)/$@ $(LIBS);

MAKE_OBJ = \
	@echo "CXX [$<]"; \
	$(CXX) $(CXXFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $@);

MAKE_STATIC_LIB = \
	@echo "AR [$@]"; \
	$(AR) $(ARFLAGS) $(LIB_DIR)/$@.a $(wildcard $(OBJ_DIR)/*.o);

MAKE_TEST = \
	echo "TEST [$@]"; \
	$(CXX) $(CFLAGS) -c $@.c -o $(OBJ_DIR)/$@.o; \
	$(CXX) $(OBJ_DIR)/$@.o -o $(BIN_DIR)/$@ $(LIBS);
