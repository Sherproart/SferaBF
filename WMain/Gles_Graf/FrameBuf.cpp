#include "stdafx.h"
#include <stdio.h>
#include "esUtil.h"
#include "PVRTMatrix.h"
#include "graph.h"
#include "FrameBuf.h"
extern PVRTMATRIXf mvpMatrix;
//
//  если создан, то возврат ненулевой
//   * здесь принято, что основной рабочий текстурный модуль 0,
//     а модуль созданной текстуры 1й или 2й
//
int TFrameBufer:: Create(int texWidth, int texHeight, int TexModule)
{
   h=texHeight;	w=texWidth; 
   // создание текстуры нужных размеров, 
   // она будет приязана к текущему ткстурному модулю (по умол. к 0-му)
   if(TexModule==1){
   glActiveTexture ( GL_TEXTURE1 );
   glUniform1i(UnifFontTexLoc, 1); num=1;}
   if(TexModule==2){
   glActiveTexture ( GL_TEXTURE2 );
   glUniform1i(UnifFontTexLoc, 2); num=2;}
   if(TexModule==3){
   glActiveTexture ( GL_TEXTURE3 );
   glUniform1i(UnifFontTexLoc, 3); num=3;}
   if(TexModule==4){
   glActiveTexture ( GL_TEXTURE4 );
   glUniform1i(UnifFontTexLoc, 4); num=4;}
     glGenTextures(1, &texture);
     glBindTexture(GL_TEXTURE_2D, texture);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
     //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
     
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight,0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
   glActiveTexture ( GL_TEXTURE0 );
   glUniform1i(UnifFontTexLoc, 0);
                
   // создание фреймбуферного объекта
   glGenFramebuffers(1, &name);
   glBindFramebuffer(GL_FRAMEBUFFER, name); // включение

   // привязывание текстуры к фреймбуферу
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, texture, 0);


   // check for framebuffer complete
   status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
   if(status == GL_FRAMEBUFFER_COMPLETE)
   {
      // отключение фреймбуфера
      glBindFramebuffer(GL_FRAMEBUFFER, 0); 
      printf("Create framebuffer %d OK\n",TexModule);
	  return 0;
   }

   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   printf("Error create framebuffer %d\n",TexModule);
   return 1;

}

void TFrameBufer:: Delete()
{
   glDeleteFramebuffers(1, &name);
   glDeleteTextures(1, &texture);
}

void TFrameBufer:: EnableDraw()
{
  glBindFramebuffer(GL_FRAMEBUFFER, name); 
}

void TFrameBufer:: DisableDraw()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void TFrameBufer:: EnableRead()
{
   glBindTexture(GL_TEXTURE_2D, texture);
/*   
   if(num==1) {
    glActiveTexture ( GL_TEXTURE1 );
    glUniform1i(UnifFontTexLoc, 1);
   }
   else {
    glActiveTexture ( GL_TEXTURE2 );
    glUniform1i(UnifFontTexLoc, 2);
   } */
    // или было раньше: 
   // glActiveTexture ( GL_TEXTURE1 );
   // glUniform1i(UnifFontTexLoc, 1);
}

void TFrameBufer:: DisableRead()
{
	glActiveTexture ( GL_TEXTURE0 );
    glUniform1i(UnifFontTexLoc, 0);
}


//extern int MapWindow_X;     // размеры области вывода карты
//extern int MapWindow_Y;     // они могут изменяться во время работы

//
//  вывод прямоугольника карты из текстуры фреймбуфера
//  x,y - координаты левого верх угла в пикселах на карте текстуры
//  x_disp, y_disp - координаты левого верх угла в пикселах экране
//  w_disp,h_disp - ширина, высота в пикселах на экране
//  размеры области вывода карты - MapWindow_X, MapWindow_Y
//
void TFrameBufer::DrawMap(float x,float y,float x_disp,float y_disp,float w_disp, float h_disp)
{
	 EnableRead();
     glClearColor ( 1, 1, 1, 1.0);glClear ( GL_COLOR_BUFFER_BIT ); // else blendind with black color
   
             glUniform1i(UnifGModeLoc,GMODE_BMP );
			 float V[]={   x_disp,y_disp+h_disp, x_disp+w_disp,y_disp+h_disp, x_disp+w_disp,y_disp, x_disp,y_disp,};
             glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), V );
             glEnableVertexAttribArray ( POS_INDEX ); 

			 float T[]={   x/w,y/h,   (x+w_disp)/w,y/h,   (x+w_disp)/w,(y+h_disp)/h, x/w,(y+h_disp)/h};
			 //float T[]={0,1, 1,1,   1,0,    0,0 };
             glVertexAttribPointer ( TXCOORD_INDEX, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), T );
             glEnableVertexAttribArray ( TXCOORD_INDEX );

	     glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );
	 DisableRead();


}

void TFrameBufer::DrawDef(float x_disp, float y_disp, float w_disp, float h_disp)
{
    EnableRead();

    glUniform1i(UnifGModeLoc, GMODE_BMP);
    float V[] = { x_disp,y_disp + h_disp, x_disp + w_disp,y_disp + h_disp, x_disp + w_disp,y_disp, x_disp,y_disp, };
    glVertexAttribPointer(POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), V);
    glEnableVertexAttribArray(POS_INDEX);

    float T[] = { 0,0,   1,0,   1,1,  0,1 };
    glVertexAttribPointer(TXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), T);
    glEnableVertexAttribArray(TXCOORD_INDEX);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    DisableRead();


}//TFrameBufer::DrawDef---------------------------------------



void TFrameBufer::DrawRotateMap(float x,float y,float x_disp,float y_disp,float w_disp, float h_disp, float sin_ang, float cos_ang,float xc, float yc)
{
	 EnableRead();

             glUniform1i(UnifGModeLoc,GMODE_BMP );
			 //float V[]={x_disp,y_disp, x_disp+w_disp,y_disp, x_disp+w_disp,y_disp+h_disp, x_disp,y_disp+h_disp};
			 float V[]={   x_disp,y_disp+h_disp, x_disp+w_disp,y_disp+h_disp, x_disp+w_disp,y_disp, x_disp,y_disp,};
             glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), V );
             glEnableVertexAttribArray ( POS_INDEX ); 

			 //float T[]={   x/w,(h-y)/h,   (x+w_disp)/w,(h-y)/h,   (x+w_disp)/w,(h-h_disp-y)/h, x/w,(h-h_disp-y)/h};
			 // ----  rotate points ----
			 float T[]={   ((x-xc)*cos_ang+xc)/w + (y-yc)*sin_ang/h , // x1
				           -(x-xc)*sin_ang/w+ ((y-yc)*cos_ang+yc)/h,    // y1
						      ((x+w_disp-xc)*cos_ang+xc)/w + (y-yc)*sin_ang/h, // x2
							  -(x+w_disp-xc)*sin_ang/w + ((y-yc)*cos_ang+yc)/h,  // y2
						    ((x+w_disp-xc)*cos_ang+xc)/w + (h_disp+y-yc)*sin_ang/h,  // x3
							-(x+w_disp-xc)*sin_ang/w + ((h_disp+y-yc)*cos_ang+yc)/h, // y3
							   ((x-xc)*cos_ang+xc)/w + (h_disp+y-yc)*sin_ang/h,  // x4
							   -(x-xc)*sin_ang/w + ((h_disp+y-yc)*cos_ang+yc)/h  // y4

							   };

			 //float T[]={0,1, 1,1,   1,0,    0,0 };
             glVertexAttribPointer ( TXCOORD_INDEX, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), T );
             glEnableVertexAttribArray ( TXCOORD_INDEX );
             glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );

	 DisableRead();
}

void TFrameBufer::DrawRotateMap1(float x,float y,float x_disp,float y_disp,float w_disp, float h_disp, float sin_ang, float cos_ang,float xc, float yc)
{
	 EnableRead();
  
             glUniform1i(UnifGModeLoc,GMODE_BMP );
			 //float V[]={x_disp,y_disp, x_disp+w_disp,y_disp, x_disp+w_disp,y_disp+h_disp, x_disp,y_disp+h_disp};
			 
			 float V[]={   x_disp,y_disp+h_disp, x_disp+w_disp,y_disp+h_disp, x_disp+w_disp,y_disp, x_disp,y_disp,};
             glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), V );
             glEnableVertexAttribArray ( POS_INDEX ); 

			 //float T[]={   x/w,(h-y)/h,   (x+w_disp)/w,(h-y)/h,   (x+w_disp)/w,(h-h_disp-y)/h, x/w,(h-h_disp-y)/h};
			 // ----  rotate points ----
			 float T[]={   ((x-xc)*cos_ang+xc)/w + (y-yc)*sin_ang/h , // x1
				           -(x-xc)*sin_ang/w+ ((y-yc)*cos_ang+yc)/h,    // y1
						      ((x+w_disp-xc)*cos_ang+xc)/w + (y-yc)*sin_ang/h, // x2
							  -(x+w_disp-xc)*sin_ang/w + ((y-yc)*cos_ang+yc)/h,  // y2
						    ((x+w_disp-xc)*cos_ang+xc)/w + (h_disp+y-yc)*sin_ang/h,  // x3
							-(x+w_disp-xc)*sin_ang/w + ((h_disp+y-yc)*cos_ang+yc)/h, // y3
							   ((x-xc)*cos_ang+xc)/w + (h_disp+y-yc)*sin_ang/h,  // x4
							   -(x-xc)*sin_ang/w + ((h_disp+y-yc)*cos_ang+yc)/h  // y4

							   };

			 //float T[]={0,1, 1,1,   1,0,    0,0 };
             glVertexAttribPointer ( TXCOORD_INDEX, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), T );
             glEnableVertexAttribArray ( TXCOORD_INDEX );
             glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );

	 DisableRead();

}

void glmOrtho(float X1,float X2,float Y1 ,float Y2,float Z1,float Z2); //левый верхн, право, вниз
void TFrameBufer::SetViewport(int X0,int  Y0,int  W,int  H )
{
glViewport( X0, h-Y0-H, W, H );

PVRTMatrixIdentityF(mvpMatrix);
glmOrtho(X0,X0+W,Y0+H,Y0,0,1);
  
}


//------- example------
#if 0
TFrameBufer FrameBufer;
TFrameBufer FrameBufer1;


void Demo_Draw_fromTexture()
{

 static int raz=0;
 if(raz==0) //----------------------------------
 {

   
   FrameBufer.Create(640*2,480*2,1);
   FrameBufer.SetViewport(0,0,640*2,480*2);

   FrameBufer.EnableDraw();
    
              
            for(int i=0;i<300;i++)
			{
			  float x1 = rand()%640*2;   
			  float y1 = rand()%480*2;   
			  float w = rand()%200;   
			  float h = rand()%200;   
                float clr[4]; clr[3]=1.0;
				clr[0]=(rand()%100)/200.+0.5;
				clr[1]=(rand()%100)/200.+0.5;
				clr[2]=(rand()%100)/200.+0.5;
              Bar(x1,y1,x1+w,y1+h,clr);   

			}
            //Bar(0,0,640,480,clmBlue); 
			glmPushMatrix();
			glmTranslatef(320,240,0);
            //float lines_txt_color[]={0.8,1,1,1};
			float lines_txt_color[]={0,0,0,1};
			float fon[]={0.9,0.9,0.9,1};

			StringPrintNoFon(80,20,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont7x11);
			StringPrintNoFon(80,40,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont8x11);
			StringPrintNoFon(80,60,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont9x13);
			StringPrintNoFon(80,80,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont10x15);
			StringPrintNoFon(80,100,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont10x17);
			StringPrintNoFon(80,120,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont10x17);
			StringPrintNoFon(80,150,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont11x19);
			StringPrintNoFon(80,180,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont12x20);
			StringPrintNoFon(80,210,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont13x20);
			StringPrintNoFon(80,240,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont14x23);
			StringPrintNoFon(80,280,"0100 Мегаполис Более 10 млн.",lines_txt_color, &MFont19x31);
			glmPopMatrix();


   FrameBufer.DisableDraw();




   FrameBufer1.Create(640*2,480*2,2);
   FrameBufer1.SetViewport(0,0,640*2,480*2);

     FrameBufer1.EnableDraw();

            for(int i=0;i<300;i++)
			{
			  float x1 = rand()%640*2;   
			  float y1 = rand()%480*2;   
			  float w = rand()%200;   
			  float h = rand()%200;   
                float clr[4]; clr[3]=1.0;
				clr[0]=(rand()%100)/200.;
				clr[1]=(rand()%100)/200.;
				clr[2]=(rand()%100)/200.;
              Bar(x1,y1,x1+w,y1+h,clr);   

			}
            //Bar(0,0,640,480,clmBlue); 
			glmPushMatrix();
			glmTranslatef(320,240,0);
            //float lines_txt_color[]={0.8,1,1,1};
			lines_txt_color[0]=0;
			lines_txt_color[1]=0;
			lines_txt_color[2]=0;
			fon[0]=0.9; fon[1]=0.9; fon[2]=0.9;

			StringPrintNoFon(80,20,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont7x11);
			StringPrintNoFon(80,40,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont8x11);
			StringPrintNoFon(80,60,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont9x13);
			StringPrintNoFon(80,80,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont10x15);
			StringPrintNoFon(80,100,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont10x17);
			StringPrintNoFon(80,120,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont10x17);
			StringPrintNoFon(80,150,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont11x19);
			StringPrintNoFon(80,180,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont12x20);
			StringPrintNoFon(80,210,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont13x20);
			StringPrintNoFon(80,240,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont14x23);
			StringPrintNoFon(80,280,"Fbuf1 Мегаполис Более 10 млн.",lines_txt_color, &MFont19x31);
			glmPopMatrix();


     FrameBufer1.DisableDraw();





   SetViewport(0,0,640,480);
   raz++;
 }
 
 if(raz<30)
 {
    raz++; 
	 FrameBufer.EnableRead();


			 double pxw=1/640./2.; // один пиксел по x для текстурных нормализ. координат
			 double pxh=1/480./2.; // один пиксел по y для текстурных нормализ. координат
			 static float d=pxw;
             glUniform1i(UnifGModeLoc,GMODE_BMP );
			 float V[]={0,479, 639,479, 639,0, 0,0};
             glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), V );
             glEnableVertexAttribArray ( POS_INDEX ); 

             static float T[]={0.25,0.25,  0.75,0.25, 0.75,0.75,  0.25,0.75 };
			 T[0]+=d; T[2]+=d; T[4]+=d; T[6]+=d;
			 //float T[]={0,0,  1,0, 1,1,  0,1 };
             glVertexAttribPointer ( TXCOORD_INDEX, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), T );
             glEnableVertexAttribArray ( TXCOORD_INDEX );
             glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );
			 //d+=pxw;

		//	 FrameBufer.DrawMap(0,0,10,10,500,500);

	 FrameBufer.DisableRead();


 }


 if(raz>=30)
 {
	 
    raz++; 
    if(raz>70)raz=1;
	 FrameBufer1.EnableRead();

/*	          
			 double pxw=1/640./2.; // один пиксел по x для текстурных нормализ. координат
			 double pxh=1/480./2.; // один пиксел по y для текстурных нормализ. координат
			 static float d=pxw;
             glUniform1i(UnifGModeLoc,GMODE_BMP );
			 float V[]={0,479, 639,479, 639,0, 0,0};
             glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), V );
             glEnableVertexAttribArray ( POS_INDEX ); 

             static float T[]={0.25,0.25,  0.75,0.25, 0.75,0.75,  0.25,0.75 };
			 T[0]+=d; T[2]+=d; T[4]+=d; T[6]+=d;
			 //float T[]={0,0,  1,0, 1,1,  0,1 };
             glVertexAttribPointer ( TXCOORD_INDEX, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), T );
             glEnableVertexAttribArray ( TXCOORD_INDEX );
             glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );
			 //d+=pxw;
*/
            FrameBufer1.DrawMap(0,200,10,10,500,400);


	 FrameBufer1.DisableRead();


 }




}


#endif
