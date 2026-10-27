CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -MMD -MP
EXEC = programme

SRC = $(filter-out test_%.cpp,$(wildcard *.cpp))
OBJ = $(SRC:%.cpp=build/%.o)
LIB_OBJ = $(filter-out build/main.o,$(OBJ))

TEST_SRC = $(wildcard test_*.cpp)
TEST_BIN = $(TEST_SRC:%.cpp=build/%)

DEP = $(OBJ:.o=.d) $(TEST_SRC:%.cpp=build/%.d)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC)

build/%.o: %.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/test_%: build/test_%.o $(LIB_OBJ)
	$(CXX) $^ -o $@

build:
	mkdir -p build

run: $(EXEC)
	./$(EXEC)

test: $(TEST_BIN)
	@for t in $(TEST_BIN); do echo "== $$t"; ./$$t || exit 1; done

clean:
	rm -rf build $(EXEC)

-include $(DEP)

.PHONY: all run test clean