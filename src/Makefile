include $(PWD)/config.mk
SRCS = $(filter-out piq.cpp, $(wildcard *.cpp))
OBJS = $(SRCS:.cpp=.o)

# default: subdirs libpiq piq
default: subdirs libpiq

subdirs:
	make -s -C comm
	make -s -C control
	make -s -C imu
	make -s -C motor

%.o: %.cpp
	$(MAKE_OBJ)

libpiq: $(OBJS)
	$(MAKE_STATIC_LIB)

piq:
	$(MAKE_EXE)
