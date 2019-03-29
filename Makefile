CC = emcc

all: main.c lib/marching_squares.c
	$(CC) -Wall -s FETCH=1 -s 'RESERVED_FUNCTION_POINTERS=10' -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["addFunction", "cwrap"]'  --pre-js module.js --js-library lib/dom.js --js-library main.js -o index.html --shell-file index.html.tpl $^
