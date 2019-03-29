#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten/emscripten.h>
#include <emscripten/fetch.h>
#include "lib/dom.h"
#include "lib/marching_squares.h"

#define HEIGHT 771
#define WIDTH 1440

#define M 361
#define N 720

int hImage;

extern void initalized(void);

typedef struct {
	int h_canvas;
	int h_context;
	void (*done)(int);
} user_data;

void downloadSucceeded(emscripten_fetch_t *fetch) {
	printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
	// The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
	float *f;
	float min, max, tmp;
	// The forst four bytes of the data are a long describing how many bytes of data are following
	long byte_count = *(long *)fetch->data,
		 length = M * N;
	int i, j;

	// Check we have the expected amount of data
	if (byte_count != 4 * M * N) {
		// The data file is not report the correct amount of data.
		printf("unexpected byte count. Expect %d, have %ld\n", 4 + 4 * M * N, byte_count);
	} else if (fetch->numBytes < byte_count + 4) {
		// The data returned isnt long enough..
		printf("Data to short. Should be at least %ld long, but only have %llu\n", byte_count + 4, fetch->numBytes);
	}

	// Make it in to an array of longs
	f = (float *)&fetch->data[4];
	min = max = f[0];

	// Reshape data so the longitues go from -180 to +179.5 instead of 0 to 359.5 and latitues
	// to go from +90 to -90 rather than -90 to +90
	int half = N/2;
	for (i = 0; i < M; i++) {
		for (j = 0; j < half; j++) {
			tmp = f[i*N + j];
			f[i*N + j] = f[(M-1-i)*N + j + half];
			f[(M-1-i)*N + j + half] = tmp;
		}
	}

	unsigned long t1 = now();

	for (i = 1; i < length; i++) {
		if (f[i] < min) min = f[i];
		if (f[i] > max) max = f[i];
	}

	// Advance f to 72.5 deg latitude
	f += 35 * N;

	for (i = ((int)min / 400) * 400; i < ((int)max / 400 + 1)*400; i += 400) {
		marchingSquares(f,  (float)i, 276, N, ((user_data*)fetch->userData)->h_context);
	}

	(*(((user_data*)fetch->userData)->done))(((user_data*)fetch->userData)->h_canvas);

	unsigned long t2 = now();

	printf("render time: %lu\n", (t2-t1));

	free(fetch->userData);
	emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch) {
	printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
	free(fetch->userData);
	emscripten_fetch_close(fetch); // Also free data on failure.
}

void EMSCRIPTEN_KEEPALIVE draw(char* datafile, int h_canvas, void (*done)(int)) {
	int h_context = canvas_getContext2d(h_canvas);
	// fill space white
	context2d_setFillStyle(h_context, "#ffffff");
	context2d_fillRect(h_context, 0, 0, WIDTH, HEIGHT);

	context2d_drawImage(h_context, hImage, 0, 0, WIDTH, HEIGHT);

	// Add transparent cavas to lighten map..
	context2d_setFillStyle(h_context, "#ffffffaa");
	context2d_fillRect(h_context, 0, 0, WIDTH, HEIGHT);

	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.onsuccess = &downloadSucceeded;
	attr.onerror = &downloadFailed;

	attr.userData = malloc(sizeof(user_data));
	((user_data*)attr.userData)->h_canvas = h_canvas;
	((user_data*)attr.userData)->h_context = h_context;
	((user_data*)attr.userData)->done = done;

	emscripten_fetch(&attr, datafile);
}

void onImage(int im) {
	hImage = im;
	initalized();
}

int main(void) {
	loadImage("data/nww3Swell-940-M1440p-88.gif", &onImage);
}
