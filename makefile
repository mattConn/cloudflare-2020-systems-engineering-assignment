CC=clang++
FLAGS=-std=c++11
BIN=makereq
URL=cloudflare2020-assignment.foobiebletch.net

$(BIN): *.cpp
	$(CC) $(FLAGS) $^ -o $(BIN)

check:
	./$(BIN) --url $(URL) 

profile:
	./$(BIN) --url $(URL)/links --profile 4

links:
	./$(BIN) --url $(URL)/links 

clean:
	rm $(BIN)
