SRC = bst.cc
CXXFLAGS = -g -std=c++14 -Wall -Wextra -Wpedantic -O3
EXE = $(SRC:.cc=.x)
OBJ = $(SRC:.cc=.o)
INC = bst.h

all: $(EXE)

$(EXE): $(OBJ)
	g++ -o $@ $^

%.o: %.cc 
	$(CXX) -c $< -o $@ $(CXXFLAGS)

format: $(SRC) $(INC)

test: CXXFLAGS += -DTEST
test: all
	./bst.x	

bench: CXXFLAGS += -DBENCH
bench: all

memory: 
	valgrind --leak-check=full -v ./bst.x
clean:
	rm -f $(EXE) *~ $(OBJ)

.PHONY: clean all format

