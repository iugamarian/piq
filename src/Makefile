include $(PWD)/config.mk
FOLDER_NAME = src

SRCS = $(filter-out piq.c, $(wildcard *.c))
OBJS = $(SRCS:.c=.o)


default: libpiq piq

%.o: %.c
	$(MAKE_OBJ)

libpiq: $(OBJS)
	$(MAKE_STATIC_LIB)

piq:
	$(MAKE_EXE)
