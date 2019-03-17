#include <math.h>

#include "./dom.h"

typedef enum {UP, DOWN, LEFT, RIGHT, INIT} direction;

void applyMercator(line_operation *ops, int count, int m, int n, float north, float south, int width, int height) {
	float x_scale = ((float)width)/n,
		  pi_by_180 = M_PI / 180,
		  tan_north = tanf(north * pi_by_180),
		  tan_south = tanf(south * pi_by_180),
		  y_scale = ((float)height)/(tan_north - tan_south),
		  lat_resoltion = (north - south)/(m - 1);

	while (count--) {
		ops->x *= x_scale;
		ops->y = y_scale*(tan_north - tanf(pi_by_180*(north - ops->y*lat_resoltion)));

		ops++;
	}
}

void marchingSquares(float *f, float cutoff, int m, int n, int hContext) {
	float *a, *b, *c, *d;
	float t;
	unsigned char type;
	unsigned char types[m][n];
	line_operation ops[m * n];
	direction dir;
	int i, j, p, q, count = 0;

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
							}

							t = (cutoff - *b)/(*d - *b);
							ops[count++] = (line_operation) {LINE_TO, q + t, p + 1};

							dir = DOWN;
							p++;
						} else if (dir == UP) {
							t = (cutoff - *c)/(*d - *c);
							ops[count++] = (line_operation) {LINE_TO, q + 1, p + t};
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
							dir = RIGHT;
							q++;
						} else if (dir == LEFT) {
							t = (cutoff - *a)/(*c - *a);
							ops[count++] = (line_operation) {LINE_TO, q + t, p};
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
							}

							t = (cutoff - *b)/(*d - *b);
							ops[count++] = (line_operation) {LINE_TO, q + t, p + 1};
							p++;
						} else if (dir == UP) {
							t = (cutoff - *a)/(*c - *a);
							ops[count++] = (line_operation) {LINE_TO, q + t, p};
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
							}

							t = (cutoff - *b)/(*d - *b);
							ops[count++] = (line_operation) {LINE_TO, q + t, p + 1};
							dir = DOWN;
							p++;
						} else if (dir == UP) {
							t = (cutoff - *a)/(*b - *a);
							ops[count++] = (line_operation) {LINE_TO, q, p + t};
						}
						break;
					case 0b0101:
					case 0b1010:
						types[p][q] = 0;

						if (dir == INIT || dir == RIGHT) {
							if (dir == INIT) {
								t = (cutoff - *a)/(*b - *a);
								ops[count++] = (line_operation) {MOVE_TO, q, p + t};
							}

							t = (cutoff - *c)/(*d - *c);
							ops[count++] = (line_operation) {LINE_TO, q + 1, p + t};
							q++;
						} else if (dir == LEFT) {
							t = (cutoff - *a)/(*b - *a);
							ops[count++] = (line_operation) {LINE_TO, q, p + t};
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
							}

							t = (cutoff - *b)/(*d - *b);
							ops[count++] = (line_operation) {LINE_TO, q + t, p + 1};
							dir = DOWN;
							p++;
						} else if (dir == UP) {
							types[p][q] = 0b0010;
							t = (cutoff - *a)/(*b - *a);
							ops[count++] = (line_operation) {LINE_TO, q, p + t};
							dir = LEFT;
							q--;
						} else if (dir == LEFT) {
							types[p][q] = 0b0100;
							t = (cutoff - *a)/(*c - *a);
							ops[count++] = (line_operation) {LINE_TO, q + t, p};
							dir = UP;
							p--;
						} else if (dir == DOWN) {
							types[p][q] = 0b0100;
							t = (cutoff - *c)/(*d - *c);
							ops[count++] = (line_operation) {LINE_TO, q + 1, p + t};
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
							}

							t = (cutoff - *a)/(*c - *a);
							ops[count++] = (line_operation) {LINE_TO, q + t, p};
							dir = UP;
							p--;
						} else if (dir == DOWN) {
							t = (cutoff - *a)/(*b - *a);
							ops[count++] = (line_operation) {LINE_TO, q, p + t};
							dir = LEFT;
							q--;
						}
						break;
				}
			}

		}
	}

	applyMercator(ops, count, m, n, 72.5, -65.0, 1440, 771);
	context2d_lineOperations(hContext, ops, count);
	context2d_setLineWidth(hContext, 1);
	context2d_setStrokeStyle(hContext, "#2e2e2e");
	context2d_stroke(hContext);
}
