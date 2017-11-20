#include "sys_wnds.h"
#include <math.h>
#include "graph.h"
#define HowPoints 100

#ifdef _OPENGL_

void Circle(FP32 x0,FP32 y0,FP32 radius,TmColor Color)
{
glColor4fv(Color);
INT32S i;
INT32S circle_points=HowPoints;
FP32 x,y,angle,k;
k=M_2PI/circle_points;
glBegin(GL_LINE_LOOP);
      for(i=0,angle=0.;i<circle_points;i++,angle+=k)
      {
            x=cos(angle)*radius;
            y=sin(angle)*radius;
            glVertex2f(x+x0,y+y0);
      }
glEnd();
}

void Round(FP32 x_center, FP32 y_center, FP32 radius, TmColor color)
{
glColor4fv(color);
  INT32S n=HowPoints;
  INT32S q=n;
  FP32 a;
  FP32 dlt=M_2PI/n;
  FP32 x,y;
glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x_center,y_center);
  for(a=0; q; a+=dlt,q--)
  {
    x=cos(a)*radius;
    y=sin(a)*radius;
    glVertex2f(x+x_center,y+y_center);
  }
  glVertex2f(radius+x_center,0.+y_center); //x=cos(0)*radius  y=sin(0)*radius
glEnd();
}

#elif defined(_OPENGL_ES20_)
extern GLint Un_ClrLoc;

void DrawLineTexture(float* Pt,int TextureID, float TwoWidth, int cnt);
// begin - bigin angle, end - end angle (in Radians)
void DrawSectorA(int TextureID,float toW, float R, float xc, float yc,float begin, float end){
    float D=0.01;
	float x=R*cos(begin)+xc, y=R*sin(begin)+yc;
	for(float alf=begin+D; alf<end; alf+=D){

		float x1=R*cos(alf)+xc;
		float y1=R*sin(alf)+yc;

		float x2=R*cos(alf+D)+xc;
		float y2=R*sin(alf+D)+yc;

		float x3=R*cos(alf+D+D)+xc;
		float y3=R*sin(alf+D+D)+yc;

		float p[]={x,y, x2,y2};
		DrawLineTexture(p,TextureID,toW,2);

		float p1[]={x1,y1, x3,y3};
		DrawLineTexture(p1,TextureID,toW,2);

		x=x1; y=y1;

	}

}//DrawSectorA------------------------

// begin - bigin angle, end - end angle (in Radians)
// !!!!!! R!=0 !!!!
void DrawSectorB(int TextureID,float toW, float R, float xc, float yc,float begin, float end){
    //float D=0.01;
	float D=0.5/R;
	float x=R*cos(begin)+xc, y=R*sin(begin)+yc;
	for(float alf=begin+D; alf<end; alf+=D){

		float x1=R*cos(alf)+xc;
		float y1=R*sin(alf)+yc;

		float x2=R*cos(alf+D)+xc;
		float y2=R*sin(alf+D)+yc;

		float x3=R*cos(alf+D+D)+xc;
		float y3=R*sin(alf+D+D)+yc;

		float p[]={x,y, x2,y2};
		DrawLineTexture(p,TextureID,toW,2);

		float p1[]={x1,y1, x3,y3};
		DrawLineTexture(p1,TextureID,toW,2);

		x=x1; y=y1;

	}

}//DrawSectorB------------------------

void StartLineTexture(int texID);

// намного оптимальное рисование сектора из текстуры - разбиение на трапеции
// (это чтобы заменить функцию DrawSectorA(...))
// оптимальные расчет количества трапеций исходя из кривизны в 0.2 пиксела
// ang1<ang2 !!!!!
void SectorTrap(int TextureID,float toW, float R, float x, float y,float ang1, float ang2)
{
	float R1 = R-toW/2, R2=R+toW/2;
	float dlt = 2*acos((R2-0.2)/R2); // * кривизна в 0.2 пиксела
	int n = (ang2-ang1)/dlt+1;
	dlt=(ang2-ang1)/n; // точное значение
	float Vt[8];
	int i=0;

	glUniform1i(UnifGModeLoc,GMODE_BMP );
	StartLineTexture(TextureID);
	glEnableVertexAttribArray ( POS_INDEX );

	for(float a=ang1; i<n; a+=dlt)
	{
		Vt[0]=R2*cos(a)+x;
		Vt[1]=R2*sin(a)+y;

		float a1=a+dlt;
		Vt[2]=R2*cos(a1)+x;
		Vt[3]=R2*sin(a1)+y;

		Vt[4]=R1*cos(a1)+x;
		Vt[5]=R1*sin(a1)+y;

		Vt[6]=R1*cos(a)+x;
		Vt[7]=R1*sin(a)+y;

		i++;
		
		glVertexAttribPointer ( POS_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), Vt);
		glDrawArrays ( GL_TRIANGLE_FAN, 0, 4 );
		
	}


}//SectorTrap----------------------------------------





void Circle(FP32 x_center, FP32 y_center, FP32 radius, TmColor color)
{
  INT32S n=HowPoints;
  FP32 v[2*HowPoints];
  INT32S i=0,q=n;
  FP32 a;
  FP32 dlt=M_2PI/n;
  for(a=0.; q; a+=dlt,q--)
  {
    v[i++]=radius*cos(a)+x_center;
    v[i++]=radius*sin(a)+y_center;
  }

  glUniform1i(UnifGModeLoc,GMODE_PAINT );
  glDisableVertexAttribArray ( TXCOORD_INDEX );// is required !!!!
  glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, v );
  glEnableVertexAttribArray ( 0 );
  //glVertexAttrib4fv(1,(FP32*) color);
  glUniform4fv(Un_ClrLoc,1 ,color);
  glDrawArrays ( GL_LINE_LOOP, 0, n );

  //free(v);
}

void Arc100(FP32 x_center, FP32 y_center, FP32 radius,float ang1, float ang2 ,TmColor color)
{
  INT32S n=HowPoints;
  FP32 v[2*HowPoints];
  INT32S i=0,q=n;
  FP32 a;
  FP32 dlt=M_2PI/n;
  for(a=ang1; a<ang2; a+=dlt)
  {
    v[i++]=-radius*cos(a)+x_center;
    v[i++]=-radius*sin(a)+y_center;
  }

  glUniform1i(UnifGModeLoc,GMODE_PAINT );
  glDisableVertexAttribArray ( TXCOORD_INDEX );// is required !!!!
  glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, v );
  glEnableVertexAttribArray ( 0 );
  
  glUniform4fv(Un_ClrLoc,1 ,color);
  glDrawArrays ( GL_LINE_STRIP, 0, i/2 );

}//Arc100---------------------------------


void Round(FP32 x_center, FP32 y_center, FP32 radius, TmColor color)
{
  glUniform1i(UnifGModeLoc,GMODE_PAINT );
  glDisableVertexAttribArray ( TXCOORD_INDEX );// is required !!!!
  ///INT32S n=HowPoints;
  ///FP32 v[2*(HowPoints+2)];
  INT32S n=7*radius;
  if(n>100)n=100;
  FP32* v= new FP32[2*(n+2)];
  INT32S i=2,q=n;
  FP32 a;
  FP32 dlt=M_2PI/n;

  v[0]=x_center;
  v[1]=y_center;
  for(a=0; q; a+=dlt,q--)
  {
    v[i++]=radius*cos(a)+x_center;
    v[i++]=radius*sin(a)+y_center;
  }
  v[i++]=v[2];
  v[i++]=v[3];

  glVertexAttribPointer ( 0, 2, GL_FLOAT, GL_FALSE, 0, v );
  glEnableVertexAttribArray ( 0 );
  //glVertexAttrib4fv(1, (FP32*)color);
  glUniform4fv(Un_ClrLoc,1 ,color);
  glDrawArrays ( GL_TRIANGLE_FAN, 0, n+2 );

  //free(v);
  delete[] v;
}

#endif

void NCircle(INT32S N,FP32 x0,FP32 y0,FP32 radius,TmColor Color)
{
for(int i=0;i<N;i++) {
 if(i & 1) radius-=i;
 else radius+=i;
 Circle(x0,y0,radius,Color);
 }
}
