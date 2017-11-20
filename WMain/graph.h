#ifndef _graph_h_
#define _graph_h_

typedef float* TmColor;

extern float  clmRed[];
extern float  clmGreen[];
extern float  clmGreen2[];
extern float  clmBlue[];
extern float  clmBlue2[];
extern float  clmYellow[];
extern float  clmYellow3[];
extern float  clmYellow4[];
extern float  clmPurple[];

void Bar(float SX,float SY,float EX,float EY,TmColor Color);
void Rectangle(float SX,float SY,float EX,float EY,TmColor Color);
void PDraw (GLfloat* PointCoord, GLint n, GLenum mode, GLfloat* color );


#endif