#include <stdio.h>
#include <string.h>
#include <emscripten/fetch.h>
#include "lib/dom.h"
#include "lib/marching_squares.h"

#define HEIGHT 771
#define WIDTH 1440

#define M 361
#define N 720

#define ZOOM 1

int hContext;

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

	// Transform to use indexes of f as coordinates, also zoom to see only the latitue range [-65, 72.5]
	//context2d_setTransform(hContext, ZOOM*(WIDTH + 0.0)/ N, 0, 0, ZOOM*(HEIGHT + 0.0) / M, 0, 0);
	//context2d_setTransform(hContext, ZOOM*(WIDTH + 0.0)/ N, 0, 0, ZOOM*(HEIGHT*(180.0/137.5)) / M, 0, -HEIGHT*(90.0 - 72.5)/180.0);

	// Advance f to 72.5 deg latitude
	f += 35 * N;

	for (i = ((int)min / 400) * 400; i < ((int)max / 400 + 1)*400; i += 400) {
		marchingSquares(f,  (float)i, 276, N, hContext);
	}

	unsigned long t2 = now();

	printf("render time: %lu\n", (t2-t1));

	emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch) {
	printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
	emscripten_fetch_close(fetch); // Also free data on failure.
}

int main(void) {
	hContext = canvas_getContext2d(getElementById("chart"));

	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.onsuccess = &downloadSucceeded;
	attr.onerror = &downloadFailed;
	emscripten_fetch(&attr, "data/gfs.t00z.ieee.0p50.f000");
}
