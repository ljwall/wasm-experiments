#include <stdio.h>
#include <string.h>
#include <emscripten/fetch.h>
#include "lib/dom.h"

#define HEIGHT 720
#define WIDTH 1440

#define M 361
#define N 720

#define ZOOM 1

int hContext;

void marchingSquares(float *f, float cutoff) {
	float *a, *b, *c, *d;
	unsigned char type;
	unsigned char types[M * N];
	int i, j, count = 0;

	context2d_setTransform(hContext, ZOOM*(WIDTH + 0.0)/ N, 0, 0, ZOOM*(HEIGHT + 0.0) / M, 0, 0);

	for (i = 0; i < M - 1; i++) {
		a = &(f[i*N]);		c = &(a[1]);
		b = &(f[(i+1)*N]);	d = &(b[1]);

		for (j = 0; j < N; j++) {
			type = 0;

			if (j == N -1) {
				// Wrap
				c = &f[i*N];
				d = &f[(i+1)*N];
			}

			if (*a >= cutoff) type  = 0b1000;
			if (*b >= cutoff) type |= 0b0100;
			if (*c >= cutoff) type |= 0b0010;
			if (*d >= cutoff) type |= 0b0001;

			types[i*N + j] = type;

			if (0 < type && type < 0b1111) {
				count++;
			}

			a++; b++; c++; d++;
		}
	}

	unsigned char t;
	context2d_beginPath(hContext);

	for (i = 0; i < M - 1; i++) {
		for (j = 0; j < N; j++) {
			t = types[i*N + j];

			if (0 < t && t < 0b1111) {
				//context2d_fillRect(hContext, j, i, 1, 1);

				switch (t) {
					case 0b0001:
					case 0b1110:
						context2d_moveTo(hContext, j + 0.5, i + 1);
						context2d_lineTo(hContext, j + 1, i + 0.5);
						break;
					case 0b0010:
					case 0b1101:
						context2d_moveTo(hContext, j + 0.5, i);
						context2d_lineTo(hContext, j + 1, i + 0.5);
						break;
					case 0b0011:
					case 0b1100:
						context2d_moveTo(hContext, j + 0.5, i);
						context2d_lineTo(hContext, j + 0.5, i + 1);
						break;
					case 0b0100:
					case 0b1011:
						context2d_moveTo(hContext, j, i + 0.5);
						context2d_lineTo(hContext, j + 0.5, i + 1);
						break;
					case 0b0101:
					case 0b1010:
						context2d_moveTo(hContext, j, i + 0.5);
						context2d_lineTo(hContext, j + 1, i + 0.5);
						break;
					case 0b0110:
					case 0b1001:
						context2d_moveTo(hContext, j, i + 0.5);
						context2d_lineTo(hContext, j + 0.5, i + 1);
						context2d_moveTo(hContext, j + 0.5, i);
						context2d_lineTo(hContext, j + 1, i + 0.5);
						break;
					case 0b0111:
					case 0b1000:
						context2d_moveTo(hContext, j, i + 0.5);
						context2d_lineTo(hContext, j + 0.5, i);
						break;
				}

				if (--count == 0) {
					// Break from both for loops
					i = M - 1; break;
				}
			}
		}
	}

	context2d_setLineWidth(hContext, 0.5);
	context2d_stroke(hContext);
}

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

	// Reshape data so the longitues go from -180 to +179.5 instead of 0 to 359.5
	int half = N/2;
	for (i = 0; i < M; i++) {
		for (j = 0; j < half; j++) {
			tmp = f[i*N + j];
			f[i*N + j] = f[i*N + j + half];
			f[i*N + j + half] = tmp;
		}
	}

	for (i = 1; i < length; i++) {
		if (f[i] < min) min = f[i];
		if (f[i] > max) max = f[i];
	}

	printf("First MSLP: %f, min %f, max %f\n", f[0], min, max);

	context2d_setFillStyle(hContext, "#2e2e2e");

	//marchingSquares(f,  100400);
	for (i = ((int)min / 400) * 400; i < ((int)max / 400 + 1)*400; i += 400) {
		marchingSquares(f,  (float)i);
	}

	emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch) {
	printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
	emscripten_fetch_close(fetch); // Also free data on failure.
}

void cb (int i) {
	printf("Got the number %d\n", i);
}

int main(void) {
	hContext = canvas_getContext2d(getElementById("chart"));

	context2d_setStrokeStyle(hContext, "#1e1e1e");

	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.onsuccess = &downloadSucceeded;
	attr.onerror = &downloadFailed;
	emscripten_fetch(&attr, "data/gfs.t00z.ieee.0p50.f000");

	test_cb(&cb);
}
