include $(PWD)/config.mk
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

default: $(OBJS)

%.o: %.cpp
	$(MAKE_OBJ)
