CC=clang++
FLAGS=-std=c++11
BIN=connector

$(BIN): *.cpp
	$(CC) $(FLAGS) $^ -o $(BIN)

check:
	./$(BIN)

clean:
	rm $(BIN)
