include $(PWD)/config.mk
SRCS = $(wildcard *.c)
TESTS = $(SRCS:.c=)

# default: subdirs $(TESTS)
default: subdirs

subdirs:
	make -s -C comm
	make -s -C imu

%: %.c
	$(MAKE_TEST)
