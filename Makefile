a.out: main.cc map.h player.h inventory.h item.h
	g++ -Wall -Wextra -Wpedantic -pedantic-errors -Wno-unused-variable -Wno-unused-parameter -std=c++17 -DMADE_USING_MAKEFILE -D_GLIBCXX_DEBUG -fsanitize=undefined -fsanitize=address main.cc -lncurses

clean:
	rm a.out core *.o
