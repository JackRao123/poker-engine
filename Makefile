CXX=clang++ # Use clang++ as the compiler
CXXFLAGS=-g -std=c++20 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -Wall
LDFLAGS=-fsanitize=address -fsanitize=undefined
EXEC=main
SRC=$(wildcard *.cpp) # Assuming all your .cpp files are in the same directory
OBJ=$(SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
