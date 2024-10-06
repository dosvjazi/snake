DESTDIR := ./bin/
BRICK_COMMON := brick_game/common/
BRICK_TETRIS := brick_game/tetris/
BRICK_SNAKE := brick_game/snake/
GUI_COMMON := gui/common/
GUI_CLI := gui/cli/
GUI_DESKTOP := gui/desktop/
TESTS_DIR := tests/
LCOVFLAGS= --ignore-errors inconsistent --ignore-errors mismatch

CC := g++
PPFLAGS := -I $(BRICK_COMMON) -I $(BRICK_TETRIS) -I $(BRICK_SNAKE) -I $(GUI_COMMON) -I $(GUI_CLI) -Wall -Werror -Wextra -std=c++17 -g
LDFLAGS := -lncurses -lstdc++

.PHONY: all clean install uninstall dvi dist tests gcov_report

all: install

install: mkdir brick_game_cli brick_game_desktop

uninstall:
	rm -rf $(DESTDIR)

brick_game_desktop:
	cd $(GUI_COMMON) && qmake DESTDIR=./../.$(DESTDIR) && make

brick_game_cli:
	$(CC) $(PPFLAGS) $(BRICK_TETRIS)*.cc $(BRICK_SNAKE)*.cc $(GUI_COMMON)*.cc $(GUI_CLI)*.cc -o $(DESTDIR)brick_game_cli $(LDFLAGS)

clean:
	rm -rf $(DESTDIR) *.txt $(TESTS_DIR)tests gcov_report $(TESTS_DIR)*.info $(TESTS_DIR)*.gcno $(TESTS_DIR)*.gcda *.tar.gz $(GUI_COMMON).qmake.stash $(GUI_COMMON)*.o $(GUI_COMMON)Makefile dvi/*.aux dvi/*.dvi dvi/*.log dvi/*.toc *.info *.gcno *.gcda *.a

dvi:
	cd dvi && texi2dvi file.texi

dist:
	tar -czvf brick_game.tar.gz . makefile

back_lib_tetris:
	$(CC) $(PPFLAGS) $(BRICK_TETRIS)tetris_model.cc -c -o tetris_model.o
	ar rcs back_lib_tetris.a *.o
	rm *.o

back_lib_snake:
	$(CC) $(PPFLAGS) $(BRICK_SNAKE)snake_model.cc -c -o snake_model.o
	ar rcs back_lib_snake.a *.o
	rm *.o

tests: back_lib_tetris back_lib_snake
	$(CC) $(PPFLAGS) $(TESTS_DIR)*.cc back_lib_tetris.a back_lib_snake.a -o $(TESTS_DIR)tests -lgtest 
	./$(TESTS_DIR)tests

mkdir:
	mkdir -p $(DESTDIR)

add_coverage_flag:
	$(eval PPFLAGS += --coverage)

gcov_report: add_coverage_flag tests
	lcov -t "check" -o check.info -c -d ./  --no-external $(LCOVFLAGS)
	genhtml -o report check.info $(LCOVFLAGS)

# leaks: test
# ifeq ($(shell uname -s), Linux)
# 	valgrind --leak-check=yes ./$(TESTS_DIR)tests
# else
# 	leaks --atExit -- ./$(TESTS_DIR)tests
# endif