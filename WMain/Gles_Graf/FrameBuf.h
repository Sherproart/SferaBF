#ifndef FRAMEBUF_H
#define FRAMEBUF_H

#include <GLES2/gl2.h>

struct TFrameBufer
{
   unsigned int name;     // returned from glGenFramebuffers
   unsigned int texture;  // привязываемая текстура
   int status;
   int w,h;
   int num;

   void SetViewport(int X0,int  Y0,int  W,int  H );
   int Create(int texWidth, int texHeight, int TexModule); // TexModule = 1 или 2 (пока что)
   void Delete();
   void EnableDraw();
   void DisableDraw();
   void EnableRead();
   void DisableRead();
   // вывод прямоугольника из текстуры фреймбуфера на весь текущий Viewport:
   void DrawMap(float x,float y,float x_disp,float y_disp,float w_disp, float h_disp);
   void DrawDef(float x_disp, float y_disp, float w_disp, float h_disp);
   void DrawRotateMap(float x,float y,float x_disp,float y_disp,float w_disp, float h_disp, float sin_ang, float cos_ang,float xc, float yc);
   void DrawRotateMap1(float x,float y,float x_disp,float y_disp,float w_disp, float h_disp, float sin_ang, float cos_ang,float xc, float yc);

};


#endif
