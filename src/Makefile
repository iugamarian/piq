include $(PWD)/config.mk
FOLDER_NAME = src

SRCS = $(filter-out piq.c, $(wildcard *.c))
OBJS = $(SRCS:.c=.o)


default: subdirs libpiq piq

subdirs:
	make -s -C comm
	make -s -C controller
	make -s -C imu
	# make -s -C motor

%.o: %.c
	$(MAKE_OBJ)

libpiq: $(OBJS)
	$(MAKE_STATIC_LIB)

piq:
	$(MAKE_EXE)
