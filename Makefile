CXX ?= g++
CXXFLAGS ?= -fsanitize=address -fsanitize=undefined --std=c++11

test.o: board.test.cpp board.cpp
	$(CXX) $(CXXFLAGS) $< board.cpp -o $@

test: test.o
	./test.o

main.o: main.cpp board.cpp solve.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

main: main.o
	./main.o

clean:
	rm -rf *.bin *.dSYM *.o board board.test

sync:
	rsync \
	-rtv \
	--delete \
	--exclude '.git*' \
	--filter=':- .gitignore' \
	./ \
	$(SYNCPATH)