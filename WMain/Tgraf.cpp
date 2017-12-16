#include "stdafx.h"
#include "esUtil.h"
#include "WMain.h"
#include "graph.h"
#include <stdio.h>
#include "PVRTMatrix.h"

unsigned Tex1;
extern int ButDown;

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
//SetViewport(0, 0, w_Width, w_Heigh);
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

  //GetWindowSise(MainHwnd);
  //SetViewport(0, 0, w_Width, w_Heigh);

  glUniform1i(UnifGModeLoc, GMODE_PAINT);


  float V[]={0,0,0, 200,0,0,  100,100,32,  0,100,0};
  //float V[]={0,0,0, 200,0,0, 100,100,5,  0,100,0};
  //float V[]={0, 0,  1,0,  1,1, 0,1};
  glVertexAttribPointer ( POS_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), V );
  glEnableVertexAttribArray ( POS_INDEX );

  //glUniform4fv(Un_ClrLoc,1 ,clmGreen);

  // Атрибут-константа для передачи цвета, можно взять 4fv для прозрачности.
  //float clr[]={0,1,0};glVertexAttrib3fv(COL_INDEX, clmGreen);
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
  
  //glDrawArrays ( GL_LINE_LOOP, 0, 3 );
  glDrawArrays ( GL_TRIANGLE_FAN, 0, 3 );//

  //Sleep(300);
  mSwapBuffers();

}//--------------

#include "sfera.h"

void TestS()
{
 static int raz=1;
 if(raz){
	//GetWindowSise(MainHwnd);
	//SetViewport(0, 0, w_Width, w_Heigh);
    SetSimetricOrtho(w_Width, w_Heigh, 1000);
	glUniform1i(UnifGModeLoc, GMODE_PAINT);
	//glVertexAttrib3fv(COL_INDEX, clmGreen);

	//glmRotate_X(0);
	//V_rotate(-20,0,1,0);
	//glmRotate_Z(90);

	V_rotate(45,0,0,1);
	V_rotate(20,1,0,0);

	raz=0;
 }

 glClearColor ( 0.0, 0.0, 0.0, 1.0);
 //glClearColor (1,1,1,1);
 glClear ( GL_COLOR_BUFFER_BIT );

 static float ug=0;
 if(ButDown)
 {
     glmRotate_Z(0.1);
 }

 Sf.DrawPoligonR();
 /*
 Sf.DrawLine();
 Sf.pgn[0].DrawMrk(6,clmRed);
 Sf.pgn[6].DrawLineP();
 */

 mSwapBuffers();

}//----

void KubeDraw()
{
	static float ABCD[]={-200,150,-200, 200,150,-200,  200,-150,-200, -200,-150,-200};
	static float ABCD1[]={-200,150,0, 200,150,0,  200,-150,0, -200,-150,0};
	static float ABCD2[]={-200,150,200, 200,150,200,  200,-150,200, -200,-150,200};

	static float AA[]={-200,150,-200, -200,150,200};
	static float BB[]={200,150,-200,   200,150,200};
	static float CC[]={200,-150,-200,  200,-150,200,};
	static float DD[]={-200,-150,-200, -200,-150,200};

    static int raz = 1;
    if (raz) {
        SetSimetricOrtho(w_Width, w_Heigh, 1000);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glUniform1i(UnifGModeLoc, GMODE_PAINT);

        V_rotate(15, 1, 1, 0);

		static PVRTMATRIXf Msc;
		PVRTMatrixScalingF(Msc, 0.5, 0.5, 0.5);
		PVRTMatrixMultiplyF(mvpMatrix,mvpMatrix,Msc);
		glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);

		//V_rotate(15,1,1,0);


		raz=0;
	}
 
	glClearColor ( 1, 1, 1, 1);
	glClear ( GL_COLOR_BUFFER_BIT );

	PDraw3D(ABCD,4,GL_LINE_LOOP,clmBlack);
	//PDraw3D(ABCD1,4,GL_LINE_LOOP,clmBlue);
	PDraw3D(AA,2,GL_LINES,clmBlack);
	float clr[]={0.5,0.5,1, 1};
	PDraw3D(ABCD1,4,GL_TRIANGLE_FAN,clr);
	PDraw3D(ABCD2,4,GL_LINE_LOOP,clmRed);


	PDraw3D(BB,2,GL_LINES,clmBlack);
	PDraw3D(CC,2,GL_LINES,clmBlack);
	PDraw3D(DD,2,GL_LINES,clmBlack);

	mSwapBuffers();


}//----

void Triang3D(float x, float y, float z, float* clr)
{
    float V[] = { x,y,z, x + 5,y,z, x + 5,y + 5,z };
    PDraw3D(V,3, GL_TRIANGLES, clr);
}//-------

void TestABBA()
{

    static int raz = 1;
    if (raz) {

        //SetSimetricOrtho(w_Width, w_Heigh, 2000); // на начальную матрицу


        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform1i(UnifGModeLoc, GMODE_PAINT);

     mvpMatrix.print5();
        //glmTranslatef(100, 0, 0);
        glmScale(2. / w_Width, 2. / w_Heigh, 2. / 2000);

		// координатная сетка
        Line(-400, 0, 400, 0, clmWhite);
        Line(0, -400, 0, 400, clmWhite);
        Bar(0, 100, 10, 110, clmWhite); // начальное положение

        Triang3D(30,30,0, clmGreen);
        Triang3D(30, 30, -200, clmRed);

       // glmRotate_Z(20);
       // Bar(0, 100, 10, 110, clmGreen);


 

        raz = 0;
    }
    mSwapBuffers(); return;

    //---------------

	SetSimetricOrtho(w_Width, w_Heigh, 2000); // на начальную матрицу

    glmTranslatef(100, 0, 0);
    Bar(0, 100, 10, 110, clmBlue);

    glmRotate_Z(20);
    Bar(0, 100, 10, 110, clmYellow);

    glmRotate_Z(20);
    Bar(0, 100, 10, 110, clmYellow);


    glmRotate_Z(20);
    Bar(0, 100, 10, 110, clmYellow);

    glmRotate_Z(20);
    Bar(0, 100, 10, 110, clmYellow);

    glmRotate_Z(20);
    Bar(0, 100, 10, 110, clmYellow);

    mSwapBuffers();

}//----



