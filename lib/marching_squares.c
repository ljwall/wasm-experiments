#include <stdio.h>
#include "./dom.h"

typedef enum {UP, DOWN, LEFT, RIGHT, INIT} direction;

void marchingSquares(float *f, float cutoff, int m, int n, int hContext) {
	float *a, *b, *c, *d;
	float t;
	unsigned char type;
	unsigned char types[m][n];
	line_operation ops[m * n];
	direction dir;
	int i, j, p, q, count = 0;

	unsigned long t1 = now();

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

			types[i][j] = type;

			a++; b++; c++; d++;
		}
	}

	for (i = 0; i < m - 1; i++) {
		for (j = 0; j < n; j++) {
			dir = INIT; p = i; q = j;

			while (0 < (type = types[p][q]) && type < 0b1111 && 0 <= p && p < m - 1 && 0 <= q && q < n) {
				a = &(f[p*n + q]);
				b = &(f[(p+1)*n + q]);

				if (q < n -1) {
					c = &(a[1]);
					d = &(b[1]);
				} else {
					// Wrap
					c = &f[p*n];
					d = &f[(p+1)*n];
				}

				switch (types[p][q]) {
					case 0b0001:
					case 0b1110:
						types[p][q] = 0;

						if (dir == INIT || dir == LEFT) {
							if (dir == INIT) {
								t = (cutoff - *c)/(*d - *c);
								ops[count++] = (line_operation) {MOVE_TO, q + 1, p + t};
								//context2d_moveTo(hContext, q + 1, p + t);

							}

							t = (cutoff - *b)/(*d - *b);
							ops[count++] = (line_operation) {LINE_TO, q + t, p + 1};
							//context2d_lineTo(hContext, q + t, p + 1);

							dir = DOWN;
							p++;
						} else if (dir == UP) {
							t = (cutoff - *c)/(*d - *c);
							ops[count++] = (line_operation) {LINE_TO, q + 1, p + t};
							//context2d_lineTo(hContext, q + 1, p + t);
							dir = RIGHT;
							q++;
						}

						break;
					case 0b0010:
					case 0b1101:
						types[p][q] = 0;

						if (dir == INIT || dir == DOWN) {
							if (dir == INIT) {
								t = (cutoff - *a)/(*c - *a);
								ops[count++] = (line_operation) {MOVE_TO, q + t, p};
								//context2d_moveTo(hContext, q + t, p);
							}

							t = (cutoff - *c)/(*d - *c);
							ops[count++] = (line_operation) {LINE_TO, q + 1, p + t};
							//context2d_lineTo(hContext, q + 1, p + t);
							dir = RIGHT;
							q++;
						} else if (dir == LEFT) {
							t = (cutoff - *a)/(*c - *a);
							ops[count++] = (line_operation) {LINE_TO, q + t, p};
							//context2d_lineTo(hContext, q + t, p);
							dir = UP;
							p--;
						}

						break;
					case 0b0011:
					case 0b1100:
						types[p][q] = 0;

						if (dir == INIT || dir == DOWN) {
							if (dir == INIT) {
								t = (cutoff - *a)/(*c - *a);
								ops[count++] = (line_operation) {MOVE_TO, q + t, p};
								//context2d_moveTo(hContext, q + t, p);
							}

							t = (cutoff - *b)/(*d - *b);
							ops[count++] = (line_operation) {LINE_TO, q + t, p + 1};
							//context2d_lineTo(hContext, q + t, p + 1);
							p++;
						} else if (dir == UP) {
							t = (cutoff - *a)/(*c - *a);
							ops[count++] = (line_operation) {LINE_TO, q + t, p};
							//context2d_lineTo(hContext, q + t, p);
							p--;
						}

						break;
					case 0b0100:
					case 0b1011:
						types[p][q] = 0;

						if (dir == INIT || dir == RIGHT) {
							if (dir == INIT) {
								t = (cutoff - *a)/(*b - *a);
								ops[count++] = (line_operation) {MOVE_TO, q, p + t};
								//context2d_moveTo(hContext, q, p + t);
							}

							t = (cutoff - *b)/(*d - *b);
							ops[count++] = (line_operation) {LINE_TO, q + t, p + 1};
							//context2d_lineTo(hContext, q + t, p + 1);
							dir = DOWN;
							p++;
						} else if (dir == UP) {
							t = (cutoff - *a)/(*b - *a);
							ops[count++] = (line_operation) {LINE_TO, q, p + t};
							//context2d_lineTo(hContext, q, p + t);
						}
						break;
					case 0b0101:
					case 0b1010:
						types[p][q] = 0;

						if (dir == INIT || dir == RIGHT) {
							if (dir == INIT) {
								t = (cutoff - *a)/(*b - *a);
								ops[count++] = (line_operation) {MOVE_TO, q, p + t};
								//context2d_moveTo(hContext, q, p + t);
							}

							t = (cutoff - *c)/(*d - *c);
							ops[count++] = (line_operation) {LINE_TO, q + 1, p + t};
							//context2d_lineTo(hContext, q + 1, p + t);
							q++;
						} else if (dir == LEFT) {
							t = (cutoff - *a)/(*b - *a);
							ops[count++] = (line_operation) {LINE_TO, q, p + t};
							//context2d_lineTo(hContext, q, p + t);
							q--;
						}
						break;
					case 0b0110:
					case 0b1001:
						if (dir == INIT || dir == RIGHT) {
							types[p][q] = 0b0010;
							if (dir == INIT) {
								t = (cutoff - *a)/(*b - *a);
								ops[count++] = (line_operation) {MOVE_TO, q, p + t};
								//context2d_moveTo(hContext, q, p + t);
							}

							t = (cutoff - *b)/(*d - *b);
							ops[count++] = (line_operation) {LINE_TO, q + t, p + 1};
							//context2d_lineTo(hContext, q + t, p + 1);
							dir = DOWN;
							p++;
						} else if (dir == UP) {
							types[p][q] = 0b0010;
							t = (cutoff - *a)/(*b - *a);
							ops[count++] = (line_operation) {LINE_TO, q, p + t};
							//context2d_lineTo(hContext, q, p + t);
							dir = LEFT;
							q--;
						} else if (dir == LEFT) {
							types[p][q] = 0b0100;
							t = (cutoff - *a)/(*c - *a);
							ops[count++] = (line_operation) {LINE_TO, q + t, p};
							//context2d_lineTo(hContext, q + t, p);
							dir = UP;
							p--;
						} else if (dir == DOWN) {
							types[p][q] = 0b0100;
							t = (cutoff - *c)/(*d - *c);
							ops[count++] = (line_operation) {LINE_TO, q + 1, p + t};
							//context2d_lineTo(hContext, q + 1, p + t);
							dir = RIGHT;
							q++;
						}

						break;
					case 0b0111:
					case 0b1000:
						types[p][q] = 0;

						if (dir == INIT || dir == RIGHT) {
							if (dir == INIT) {
								t = (cutoff - *a)/(*b - *a);
								ops[count++] = (line_operation) {MOVE_TO, q, p + t};
								//context2d_moveTo(hContext, q, p + t);
							}

							t = (cutoff - *a)/(*c - *a);
							ops[count++] = (line_operation) {LINE_TO, q + t, p};
							//context2d_lineTo(hContext, q + t, p);
							dir = UP;
							p--;
						} else if (dir == DOWN) {
							t = (cutoff - *a)/(*b - *a);
							ops[count++] = (line_operation) {LINE_TO, q, p + t};
							//context2d_lineTo(hContext, q, p + t);
							dir = LEFT;
							q--;
						}
						break;
				}
			}

		}
	}
	unsigned long t2 = now();
	context2d_lineOperations(hContext, ops, count);
	unsigned long t3 = now();
	context2d_setLineWidth(hContext, 0.5);
	context2d_stroke(hContext);
	unsigned long t4 = now();

	printf("cutoff %f, count %d ,calculating %lu, calling line %lu, rendering %lu\n", cutoff, count, t2 - t1, t3 - t2, t4 - t3);
}
