CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
EXEC = programme
SRC = $(wildcard *.cpp)
HDR = $(wildcard *.hpp)

all: $(EXEC)

$(EXEC): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC)

.PHONY: all run clean