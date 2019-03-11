CC = emcc

all: main.c
	$(CC) --pre-js module.js --js-library lib/dom.js -o index.html --shell-file index.html.tpl $^
