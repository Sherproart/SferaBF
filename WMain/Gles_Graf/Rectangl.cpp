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
   //glClearColor ( 0.0, 1.0, 0.0, 1.0);
   //glClear ( GL_COLOR_BUFFER_BIT );
   //Bar(0,0,w_Width, w_Heigh, clmBlue);
//static int cnt=0; cnt++;
static int cnt=1;
    //if(cnt)
        SetViewport(0, 0, w_Width, w_Heigh);
	cnt=0;

    //Bar(-20000,-20000,20000, 20000, clmBlue);
    Bar(10, 50, 100, 200, clmPurple);
	//Bar(0, 0, 0.5, 0.5, clmPurple);

    //Bar(0.2, 0.3, 0.9, 0.9, clmPurple);


   mSwapBuffers();
printf("*");

}//--------

