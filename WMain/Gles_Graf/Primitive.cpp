#include "stdafx.h"
#include <math.h>
#include "esUtil.h"
#include "graph.h"
#include <string.h>
#include <stdio.h>

float  clmRed[]={1.,0.,0.,1.0};
float  clmGreen[]={0.,1.,0.,1.};
float  clmGreen2[]={0.,0.5,0.,1.};
float  clmBlue[]={0.,0.,1.,0.1};
float  clmBlue2[]={0.3,0.3,1.,1.};
float  clmYellow[]={1.,1.,0.,1.};
float  clmYellow3[]={0.5,0.5,0.,1.};
float  clmYellow4[]={0.7,0.7,0.,1.};
float  clmPurple[]={1.,0.,1.,1.};



// PointCoord - координаты вершин примитива (полилинии, полигона, точки),
//              на каждую вершину по 2 float числа
// n - количество используемых верщин
// mode - тип примитива - точка (GL_POINTS) или полилиния (GL_LINES,GL_LINE_STRIP,GL_LINE_LOOP)
//        или полигон как множество треугольников (GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN)
// color - цвет заливки примитива
// входные координаты от -1 до 1
void PDraw (GLfloat* PointCoord, GLint n, GLenum mode, GLfloat* color )
{
   glUniform1i(UnifGModeLoc,GMODE_PAINT );
   glDisableVertexAttribArray ( TXCOORD_INDEX );// is required !!!! obligatorily !!!!!
   // Load the vertex data
   glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, PointCoord );
   glEnableVertexAttribArray ( 0 );
   glUniform4fv(Un_ClrLoc,1 ,color);
   glDrawArrays ( mode, 0, n );

}

int w_Width, w_Heigh; // current window from msg sizeng
void GetWindowSise(HWND hWnd)
{
	RECT w_Rect;
	GetClientRect(hWnd, &w_Rect);
	w_Width = w_Rect.right - w_Rect.left;
	w_Heigh = w_Rect.bottom - w_Rect.top;

}//GetWindowSise---------

