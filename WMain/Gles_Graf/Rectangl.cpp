#include "stdafx.h"
#include "esUtil.h"
#include "graph.h"


void Rectangle(float SX,float SY,float EX,float EY,TmColor Color)
{
float PointCoord[8]; EY+=0.5;
PointCoord[0]=SX; PointCoord[1]=SY;
PointCoord[2]=EX; PointCoord[3]=SY;
PointCoord[4]=EX; PointCoord[5]=EY;
PointCoord[6]=SX; PointCoord[7]=EY;
PDraw (PointCoord, 4, GL_LINE_LOOP, Color );
}

void Bar(float SX,float SY,float EX,float EY,TmColor Color)
{
float PointCoord[8];
PointCoord[0]=SX; PointCoord[1]=SY;
PointCoord[2]=EX; PointCoord[3]=SY;
PointCoord[4]=EX; PointCoord[5]=EY;
PointCoord[6]=SX; PointCoord[7]=EY;
PDraw (PointCoord, 4, GL_TRIANGLE_FAN, Color );
}


//==============
#include "WMain.h"
#include <stdio.h>
void Test1()
{
   Bar(10, 50, 100, 200, clmPurple);
   mSwapBuffers();

}//--------

