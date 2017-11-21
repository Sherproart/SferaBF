#include "stdafx.h"
#include "esUtil.h"
#include "WMain.h"
#include "graph.h"
#include <stdio.h>
#include "PVRTMatrix.h"

unsigned Tex1;

unsigned Tex1Data[]={ // 0x00BBGGRR

  0x8000ff00, 0, 0xff0000ff, 0xff0000ff, 0xffff0000,
  0, 0, 0xFF865A4D, 0xFF865A4D, 0xffff0000,
  0xff0000ff, 0xFF865A4D, 0xff0000ff, 0xff0000ff, 0xffff0000,
  0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xffff0000

};


void texture_init()
{
static int rr=0;
if(rr)return;
 glGenTextures(1,&Tex1);
 glBindTexture(GL_TEXTURE_2D,Tex1);

 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
 //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //!!!! tmp!!!
 //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 //glDisable(GL_BLEND);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

 //glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,5,4,0, GL_RGBA, GL_UNSIGNED_BYTE, Tex1Data);
 //0x80E1
 glTexImage2D(GL_TEXTURE_2D,0,0x80E1,5,4,0, 0x80E1, GL_UNSIGNED_BYTE, Tex1Data);
 printf("init texture\n");
rr=1;

}//----------


void Test2()
{
texture_init();
SetViewport(0, 0, w_Width, w_Heigh);
/*
glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,5,4,0, GL_RGBA, GL_UNSIGNED_BYTE, Tex1Data);
static int cnt=0; cnt++; if(cnt>2)cnt=0;
switch(cnt){
   case 0:	Tex1Data[7]=0xff0000ff; break;
   case 1:	Tex1Data[7]=0xff00ff00; break;
   case 2:	Tex1Data[7]=0xffff0000; break;
}
*/

  glUniform1i(UnifGModeLoc, GMODE_BMP);
  glBindTexture(GL_TEXTURE_2D,Tex1);

  float V[]={10, 10,  110,10,  110,110, 10,110};
  glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), V );
  glEnableVertexAttribArray ( POS_INDEX );


  float T[]={0,0,  1,0, 1,1,  0,1 };
  glVertexAttribPointer ( TXCOORD_INDEX, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(float), T );
  glEnableVertexAttribArray ( TXCOORD_INDEX );

  glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );
  printf("show Texture\n");
  Sleep(300);
  mSwapBuffers();


}//----------

extern PVRTMATRIXf mvpMatrix;

void Test3()
{
	GetWindowSise(MainHwnd);
	SetViewport(0, 0, w_Width, w_Heigh);

  glUniform1i(UnifGModeLoc, GMODE_PAINT);


  float V[]={0,0,0, 200,0,0,  100,100,32,  0,100,0};
  //float V[]={0,0,0, 200,0,0, 100,100,5,  0,100,0};
  //float V[]={0, 0,  1,0,  1,1, 0,1};
  glVertexAttribPointer ( POS_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), V );
  glEnableVertexAttribArray ( POS_INDEX );

  //glUniform4fv(Un_ClrLoc,1 ,clmGreen);

  // Атрибут-константа для передачи цвета, можно взять 4fv для прозрачности.
  //float clr[]={0,1,0};glVertexAttrib3fv(COL_INDEX, clr);
  //------------------------------------------------------------------------


  float Color[]={
	1,0,0,1,
	0,1,0,1,
	0,0,1,1,
	1,1,0,1
  };
  glVertexAttribPointer ( COL_INDEX, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), Color );
  glEnableVertexAttribArray ( COL_INDEX );


  //glDrawArrays ( GL_LINE_STRIP, 0, 2 );

  /*
  mvpMatrix.f[0]=0.1;
  mvpMatrix.f[1]=0.2;
  mvpMatrix.f[2]=0.3;
  mvpMatrix.f[3]=0;
  */

  //mvpMatrix.f[5]=-1;
  //glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);
  
  
  //glDrawArrays ( GL_LINE_STRIP, 0, 2 );

  glDrawArrays ( GL_TRIANGLE_FAN, 0, 3 );//
  




  //Sleep(300);
  mSwapBuffers();


}//--------------