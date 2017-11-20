#include "sys_wnds.h"
#include "math.h"
//#include "stdlib.h"
//#include "gdata.h"
#include "graph.h"

INT32U MaskLine,FlMaskLine,FlColorBack;
//INT32S Verify_XY_for_Line(FP32 &sx,FP32 &sy,FP32 &ex,FP32 &ey);

INT32U RotateLeft(INT32U Mask,INT32S k)
{
Mask&=0xffff;
k%=16;
Mask|=(Mask<<16);
Mask>>=16-k;
Mask&=0xffff;
return(Mask);
}

INT32U RotateRight(INT32U Mask,INT32S k)
{
Mask&=0xffff;
k%=16;
Mask|=(Mask<<16);
Mask>>=k;
Mask&=0xffff;
return(Mask);
}

void MaskLine_ON()
{
FlMaskLine=1;
}

void SetMaskLine(INT32U Mask)
{
if(FlMaskLine) {
 MaskLine=Mask;
// FlColorBack=Mask & _COLORBACK_;
 FlMaskLine=0;
 }
}

#ifdef _OPENGL_

void Line(FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
glColor4fv(Color);
glBegin(GL_LINES);
glVertex2f(SX,SY);
glVertex2f(EX,EY);
glEnd();
}

void NLine(FP32 N,FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
glLineWidth(N);
glColor4fv(Color);
glBegin(GL_LINES);
glVertex2f(SX,SY);
glVertex2f(EX,EY);
glEnd();
glLineWidth(1);
}

INT32U LineMask(INT32S Mask,FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
glLineStipple(1,Mask);
glEnable(GL_LINE_STIPPLE);
Line(SX,SY,EX,EY,Color);
glDisable(GL_LINE_STIPPLE);
//коррекция маски
FP32 dX=EX-SX;
FP32 dY=EY-SY;
INT32S l=(INT32S)sqrt(dX*dX + dY*dY);
l=RotateLeft(Mask,l);
return(l);
}

//рисование линии толщиной N пикселей
void NLineMask(FP32 N,FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
glLineStipple(1,MaskLine);
glEnable(GL_LINE_STIPPLE);
NLine(N,SX,SY,EX,EY,Color);
glDisable(GL_LINE_STIPPLE);
//коррекция маски
FP32 dX=EX-SX;
FP32 dY=EY-SY;
INT32S l=(INT32S)sqrt(dX*dX + dY*dY);
MaskLine=RotateRight(MaskLine,l);
}


#elif defined(_OPENGL_ES20_)

void Line(FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
	FP32 PointCoord[]={SX,SY, EX,EY};
	PDraw (PointCoord, 2, GL_LINES, Color );
}

void RectAngLe(FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
	FP32 PointCoord[]={SX,SY, EX,SY, EX,EY, SX,EY};
	PDraw (PointCoord, 4, GL_LINE_LOOP, Color );
}//RectAngLe--------------


INT32U LineMask(INT32S Mask,FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
INT32S Msk[8][2],n=0,Fl=0,Ln;
INT32S Mask2=Mask;
for(INT32S i=0;i<16;i++) {
 Mask2<<=1;
 if(Mask2 & 0x10000) {
  if(!Fl) {Ln=1; Fl=1; n++;}  else Ln++;
  }
 else {
  if(Fl) {Msk[n-1][0]=i; Msk[n-1][1]=Ln; Fl=0;}
  }
 }
FP32 v[16][2];
FP32 DX=EX-SX;
FP32 DY=EY-SY;
//FP32 CosA;
//if(fabs(DX)<0.01) { if(DX>=0.) CosA=DY/0.01; else CosA=-DY/0.01; }
//else CosA=DY/DX;
FP32 Len=sqrt(DX*DX+DY*DY);
if(fabs(DX)<0.01) { if(DX>=0.) DX=0.01; else DX=-0.01; }
if(fabs(DY)<0.01) { if(DY>=0.) DY=0.01; else DY=-0.01; }
Ln=(INT32S)Len;
if(!Ln) return 0;
INT32S k=Ln/16;
if(Ln%16) k++;
INT32S L,L2,i;
for(i=0;i<k;i++) {
 for(INT32S j=0;j<n;j++) {
  L=i*16+Msk[j][0];
  L2=L+Msk[j][1];
  if(L2>Ln) L2=Ln;
  v[j*2][0]= DX*L/Len;
  v[j*2][1]= DY*L/Len;
  v[j*2+1][0]= DX*L2/Len;
  v[j*2+1][1]= DY*L2/Len;
  }
 PDraw (&v[0][0], n*2, GL_LINES, Color );
 }
k=Ln%16;
if(!k) return Mask;
return( RotateLeft(Mask,16-k));
}

//рисование линии толщиной N пикселей
void NLineMask(FP32 N,FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
}

#endif

void TwoNLine(INT32S N,FP32 SX,FP32 SY,FP32 EX,FP32 EY,TmColor Color)
{
INT32S N2;
FP32 dx,dy,dx2,dy2,ad;
FP32 DY=SY-EY;
FP32 DX=SX-EX;
FP32 CosAngl;
if(N>1) {
 N2=N>>1;
 N--;
 if(fabs(DX)<=fabs(DY)) {
  if(!DY) return;
  CosAngl=sqrt(1./(1.+(DX*DX)/(DY*DY)));
  dx=CosAngl*N2;
  dy=dx*DX;
  ad=fabs(DY)/2.;
  dy=(dy>=0)?(dy+ad)/DY:(dy-ad)/DY;
  dx2=dx-CosAngl*N;
  dy2=dx2*DX;
  dy2=(dy2>=0)?(dy2+ad)/DY:(dy2-ad)/DY;
  }
 else {
  CosAngl=sqrt(1./(1.+(DY*DY)/(DX*DX)));
  dy=CosAngl*N2;
  dx=dy*DY;
  ad=fabs(DX)/2.;
  dx=(dx>=0)?(dx+ad)/DX:(dx-ad)/DX;
  dy2=dy-CosAngl*N;
  dx2=dy2*DY;
  dx2=(dx2>=0)?(dx2+ad)/DX:(dx2-ad)/DX;
  }
 Line(SX+dx,SY-dy,EX+dx,EY-dy,Color);
 Line(SX+dx2,SY-dy2,EX+dx2,EY-dy2,Color);
 }
}


//***!!! исправлено 22.01.2016 нужно, чтобы glGenTextures вызывалась только раз для данной линии!----- 
//
unsigned LSmoothTexture[10];// потом, если нужно увеличить, пока же такая текстура только одна
void initSmoothTexture1()
{
	glGenTextures(1,LSmoothTexture+0);

}//initSmoothTexture1-------------

//================= smoothed line (use texture linear filter)================================
// d = (width of line)*2. width of line = d/2;
// on both sides by 1/4 width of line = transparent color.
// Line mixed by GL_LINEAR filter texture rendering.
void Line2Smooth(unsigned txtr,float d,float x1, float y1, float x2, float y2, unsigned color)
{
 glUniform1i(UnifGModeLoc,GMODE_BMP );
 //unsigned txtr; 
 //glGenTextures(1,&txtr);
 glBindTexture(GL_TEXTURE_2D,txtr);

 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

 unsigned txData[]={
 0x00ffffff,0x00ffffff, // transparent
 color,color,
 color,color,
 0x00ffffff,0x00ffffff  // transparent
 };

   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,2,4,0, GL_RGBA, GL_UNSIGNED_BYTE, txData);
   // Load the texture coordinate
   float T[]={0,0,  1,0, 1,1,  0,1 };
   glVertexAttribPointer ( TXCOORD_INDEX, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), T );
   glEnableVertexAttribArray ( TXCOORD_INDEX );


  float line[4];
  line[0]=x1; line[1]=y1;
  line[2]=x2; line[3]=y2;

  float line1[8]; float* line2=line1+4;
  float A=line[1]-line[3];
  float B=line[2]-line[0];
  float D2=A*A+B*B;
  float D=sqrt(D2);
  
  A=A*d/2/D; B=B*d/2/D;
  line1[0]=line[0]+A; line2[2]=line[0]-A;
  line1[1]=line[1]+B; line2[3]=line[1]-B;
  line1[2]=line[2]+A; line2[0]=line[2]-A;
  line1[3]=line[3]+B; line2[1]=line[3]-B;

   // Load the vertex position
   glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), line1 );
   glEnableVertexAttribArray ( POS_INDEX );                        


   glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );

 // back parameter
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

}//Line2Smooth ---------------------------------------------------


// Эта функция должна использоваться внутри цикла с общей инициализацией текстурой
// то есть для отрисовки полилиний.
// Line2Smooth_min = Line2Smooth without general initialization
void Line2Smooth_min(float d,float x1, float y1, float x2, float y2)
{
  float line[4];
  line[0]=x1; line[1]=y1;
  line[2]=x2; line[3]=y2;

  float line1[8]; float* line2=line1+4;
  float A=line[1]-line[3];
  float B=line[2]-line[0];
  float D2=A*A+B*B;
  float D=sqrt(D2);
  if(D==0)return;
  
  A=A*d/2/D; B=B*d/2/D;
  line1[0]=line[0]+A; line2[2]=line[0]-A;
  line1[1]=line[1]+B; line2[3]=line[1]-B;
  line1[2]=line[2]+A; line2[0]=line[2]-A;
  line1[3]=line[3]+B; line2[1]=line[3]-B;

   // Load the vertex position
   glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), line1 );
   //glEnableVertexAttribArray ( POS_INDEX );                        

   glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );


}//Line2Smooth_min ---------------------------------------------------

void Line2Smooth_Arr(float d,float x1, float y1, float x2, float y2)
{
  float line[4];
  line[0]=x1; line[1]=y1;
  line[2]=x2; line[3]=y2;

  float line1[12]; float* line2=line1+6;
  float A=line[1]-line[3];
  float B=line[2]-line[0];
  float D2=A*A+B*B;
  float D=sqrt(D2);
  if(D==0)return;
  
  A=A*d/2/D; B=B*d/2/D;
  //line1[0]=line[0]+A; line2[2]=line[0]-A;
  //line1[1]=line[1]+B; line2[3]=line[1]-B;
  //line1[2]=line[2]+A; line2[0]=line[2]-A;
  //line1[3]=line[3]+B; line2[1]=line[3]-B;

  line1[0]=line[0]+A; line1[4]=line[0]-A;
  line1[1]=line[1]+B; line1[5]=line[1]-B;
  line1[2]=line[2]; 
  line1[3]=line[3]; 

  line2[0]=line[0]-A; line2[4]=line[0]+A;
  line2[1]=line[1]-B; line2[5]=line[1]+B;
  line2[2]=line[2]; 
  line2[3]=line[3]; 

   // Load the vertex position
   glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), line1 );
   //glEnableVertexAttribArray ( POS_INDEX );                        
   glDrawArrays ( GL_TRIANGLES, 0, 3 );

   //glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), line2 );
   //glDrawArrays ( GL_TRIANGLES, 0, 3 );


}//Line2Smooth_Arr ---------------------------------------------------


void Line4PixSmooth_min(float x1, float y1, float x2, float y2)
{
  float line[4];
  line[0]=x1; line[1]=y1;
  line[2]=x2; line[3]=y2;

  float line1[8]; float* line2=line1+4;
  float A=line[1]-line[3];
  float B=line[2]-line[0];
  float D2=A*A+B*B;
  float D=sqrt(D2);
  if(D==0)return;
  
  A=A*3/D; B=B*3/D;
  line1[0]=line[0]+A; line2[2]=line[0]-A;
  line1[1]=line[1]+B; line2[3]=line[1]-B;
  line1[2]=line[2]+A; line2[0]=line[2]-A;
  line1[3]=line[3]+B; line2[1]=line[3]-B;
  // Load the vertex position
  glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), line1 );
  glEnableVertexAttribArray ( POS_INDEX );                        
  glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );

}//Line4PixSmooth_min ---------------------------------------------------
/*
//unsigned Ltexture1;
//float Tsline[]={0,0,  1,0, 1,1,  0,1 };

 unsigned Reka[]={
 0x00fdd6af,0x00fdd6af, // transparent
 0x00fdd6af,0x00fdd6af, // transparent
 0xfffdd6af,0xfffdd6af, // transparent
 0xfff7b28c,0xfff7b28c,
 0xfff7b28c,0xfff7b28c,
 0xfffdd6af,0xfffdd6af, // transparent
 0x00fdd6af,0x00fdd6af, // transparent
 0x00fdd6af,0x00fdd6af // transparent
 };

 unsigned Ruchej[]={
 0x00f7b28c,0x00f7b28c, // transparent
 0xfff7b28c,0xfff7b28c,
 0xfff7b28c,0xfff7b28c,
 0x00f7b28c,0x00f7b28c, // transparent
 };

unsigned LineTextureID[20];

void InitTexture(unsigned* txr,int texID,int w, int h)
{

   glGenTextures(1,LineTextureID+texID);
   glBindTexture(GL_TEXTURE_2D,LineTextureID[texID]);

   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0, GL_RGBA, GL_UNSIGNED_BYTE, txr);
//glEnableVertexAttribArray ( TXCOORD_INDEX );
   //glVertexAttribPointer ( TXCOORD_INDEX, 2, GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat), Tsline );
   //glEnableVertexAttribArray ( TXCOORD_INDEX );


}//InitTexture----------------------------------------
*/
/*
void InitLineTextures()
{
   InitTexture(Ruchej,0,2,4);
   InitTexture(Reka,1,2,8);
}
*/