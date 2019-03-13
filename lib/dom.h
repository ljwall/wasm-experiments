#ifndef _CANVAS_H
#define _CANVAS_H

extern int getElementById(char *id);
extern char element_hasAttribute(int h, char *attr);
extern int canvas_getContext2d(int h);

extern void context2d_beginPath(int i);
extern void context2d_setStrokeStyle(int i, char *style);
extern void context2d_setLineWidth(int i, float w);
extern void context2d_moveTo(int i, float x, float y);
extern void context2d_lineTo(int i, float x, float y);
extern void context2d_stroke(int i);
extern void context2d_setTransform(int i, float a, float b, float c, float d, float e, float f);
extern void context2d_fillRect(int i, float x, float y, float width, float height);
extern void context2d_setFillStyle(int i, char *style);

extern void test_cb(void (*cb)(int i));

#endif

