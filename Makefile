CC = emcc

all: main.c lib/marching_squares.c
	$(CC) -Wall -s FETCH=1 --pre-js module.js --js-library lib/dom.js -o index.html --shell-file index.html.tpl $^
