#ifndef _CANVAS_H
#define _CANVAS_H

extern int getElementById(char *id);
extern char element_hasAttribute(int h, char *attr);
extern int canvas_getContext2d(int h);

extern void context2d_beginPath(int i);
extern void context2d_setStrokeStyle(int i, char *str_ptr);
extern void context2d_setLineWidth(int i, float w);
extern void context2d_moveTo(int i, float x, float y);
extern void context2d_lineTo(int i, float x, float y);
extern void context2d_stroke(int i);

#endif

