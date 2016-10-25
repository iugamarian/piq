include $(PWD)/config.mk
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

default: $(OBJS)

%.o: %.c
	$(MAKE_OBJ)
