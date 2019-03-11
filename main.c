#include <stdio.h>
#include <string.h>
#include <emscripten/fetch.h>
#include "lib/dom.h"

void downloadSucceeded(emscripten_fetch_t *fetch) {
	printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
	// The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
	float *f;

	f = (float *)&fetch->data[4];

	printf("First MSLP: %f\n", f[0]);

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
	int hContext = canvas_getContext2d(getElementById("chart"));

	context2d_setStrokeStyle(hContext, "#1e1e1e");
	context2d_setLineWidth(hContext, 12);

	// Write the letter M.
	context2d_moveTo(hContext, 90, 130);
	context2d_lineTo(hContext, 95, 25);
	context2d_lineTo(hContext, 150, 80);
	context2d_lineTo(hContext, 205, 25);
	context2d_lineTo(hContext, 210, 130);
	context2d_stroke(hContext);

	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.onsuccess = &downloadSucceeded;
	attr.onerror = &downloadFailed;
	emscripten_fetch(&attr, "data/gfs.t00z.ieee.0p50.f000");

	test_cb(&cb);
}
