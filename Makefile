SRC = $(wildcard src/*.cpp)
OBJS := $(SRC:.cpp=.o)
DEPS := $(SRC:.cpp=.d)
CXXFLAGS := -O2 -Wall -Wextra -std=c++17


all: main

main: $(OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $< -c -o $@ $(CXXFLAGS) -MMD -MF $*.d

debug: CXXFLAGS := -g -Wall -Wextra -Werror -std=c++17
debug: clean
debug: main

.PHONY: clean mrproper debug

clean:
	$(RM) $(DEPS)
	$(RM) $(OBJS)
	$(RM) d_main

mrproper: clean
	$(RM) main

-include $(DEPS)