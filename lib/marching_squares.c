#include "./dom.h"

void marchingSquares(float *f, float cutoff, int m, int n, int hContext) {
	float *a, *b, *c, *d;
	float t;
	unsigned char type;
	int i, j;

	context2d_beginPath(hContext);

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

			if (0 < type && type < 0b1111) {
				switch (type) {
					case 0b0001:
					case 0b1110:
						t = (cutoff - *b)/(*d - *b);
						context2d_moveTo(hContext, j + t, i + 1);
						t = (cutoff - *c)/(*d - *c);
						context2d_lineTo(hContext, j + 1, i + t);
						break;
					case 0b0010:
					case 0b1101:
						t = (cutoff - *a)/(*c - *a);
						context2d_moveTo(hContext, j + t, i);
						t = (cutoff - *c)/(*d - *c);
						context2d_lineTo(hContext, j + 1, i + t);
						break;
					case 0b0011:
					case 0b1100:
						t = (cutoff - *a)/(*c - *a);
						context2d_moveTo(hContext, j + t, i);
						t = (cutoff - *b)/(*d - *b);
						context2d_lineTo(hContext, j + t, i + 1);
						break;
					case 0b0100:
					case 0b1011:
						t = (cutoff - *a)/(*b - *a);
						context2d_moveTo(hContext, j, i + t);
						t = (cutoff - *b)/(*d - *b);
						context2d_lineTo(hContext, j + t, i + 1);
						break;
					case 0b0101:
					case 0b1010:
						t = (cutoff - *a)/(*b - *a);
						context2d_moveTo(hContext, j, i + t);
						t = (cutoff - *c)/(*d - *c);
						context2d_lineTo(hContext, j + 1, i + t);
						break;
					case 0b0110:
					case 0b1001:
						t = (cutoff - *a)/(*b - *a);
						context2d_moveTo(hContext, j, i + t);
						t = (cutoff - *b)/(*d - *b);
						context2d_lineTo(hContext, j + t, i + 1);

						t = (cutoff - *a)/(*c - *a);
						context2d_moveTo(hContext, j + t, i);
						t = (cutoff - *c)/(*d - *c);
						context2d_lineTo(hContext, j + 1, i + t);
						break;
					case 0b0111:
					case 0b1000:
						t = (cutoff - *a)/(*b - *a);
						context2d_moveTo(hContext, j, i + t);
						t = (cutoff - *a)/(*c - *a);
						context2d_lineTo(hContext, j + t, i);
						break;
				}
			}

			a++; b++; c++; d++;
		}
	}

	context2d_stroke(hContext);
}
