#include <stdio.h>
#include "lib/dom.h"

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
}
