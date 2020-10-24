CC=clang++
FLAGS=-std=c++11
BIN=makereq

$(BIN): *.cpp
	$(CC) $(FLAGS) $^ -o $(BIN)

check:
	./$(BIN) --url foobiebletch.net

profile:
	./$(BIN) --url foobiebletch.net --profile 4

clean:
	rm $(BIN)
