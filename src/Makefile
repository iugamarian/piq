include $(PWD)/config.mk
FOLDER_NAME = src

SRCS = $(wildcard *.c)
EXES = $(SRCS:.c=)


default: $(EXES)

%: %.c
	echo "HERE"
	$(MAKE_EXE)
