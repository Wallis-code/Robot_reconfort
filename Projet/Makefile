CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -MMD -MP
EXEC = programme
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=build/%.o)
DEP = $(OBJ:.o=.d)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC)

build/%.o: %.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

run: $(EXEC)
	./$(EXEC)

clean:
	rm -rf build $(EXEC)

-include $(DEP)

.PHONY: all run clean