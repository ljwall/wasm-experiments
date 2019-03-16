#include "./dom.h"

void marchingSquares(float *f, float cutoff, int m, int n, int hContext) {
	float *a, *b, *c, *d;
	unsigned char type;
	unsigned char types[m * n];
	int i, j, count = 0;

	for (i = 0; i < m - 1; i++) {
		a = &(f[i*n]);		c = &(a[1]);
		b = &(f[(i+1)*n]);	d = &(b[1]);

		for (j = 0; j < n; j++) {
			type = 0;

			if (j == n -1) {
				// Wrap
				c = &f[i*n];
				d = &f[(i+1)*n];
			}

			if (*a >= cutoff) type  = 0b1000;
			if (*b >= cutoff) type |= 0b0100;
			if (*c >= cutoff) type |= 0b0010;
			if (*d >= cutoff) type |= 0b0001;

			types[i*n + j] = type;

			if (0 < type && type < 0b1111) {
				count++;
			}

			a++; b++; c++; d++;
		}
	}

	unsigned char t;

	context2d_beginPath(hContext);

	for (i = 0; i < m - 1; i++) {
		for (j = 0; j < n; j++) {
			t = types[i*n + j];

			if (0 < t && t < 0b1111) {
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
					i = m - 1; break;
				}
			}
		}
	}

	context2d_stroke(hContext);
}
