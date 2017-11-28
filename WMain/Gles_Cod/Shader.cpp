#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "esUtil.h"
#include "PVRTMatrix.h"
#include "WMain.h"

int sizeX=800, sizeY=600;
const double pi = 4 * atan(1.); 
const double RadGr = 180 / pi;
const double GrRad = pi / 180;

float ClipPlane[] = {0,1,0,0};

GLuint m_vertShaderHandle;   // The OpenGL vertex shader id
GLuint m_fragShaderHandle;   // The OpenGL fragment shader id
GLuint m_progHandle;         // The OpenGL program id

// Шрифты:
// Attribute locations
GLint AttribPosSimvLoc;  // для передачи координат вершин символа данного фонта
GLint AttribTexCoordLoc; // для передачи соотв. координат текстуры для данной вершины
// GLint AttribColorLoc;
// Uniforms location
GLint UnifTexLoc;
GLint Un_ClrLoc;
GLint UnifGModeLoc; // печать шрифта, шрифта с фоном, рисование
GLint UnifMvpMatrixLoc;
GLint UnifClipPlane;
//-----------------------

void GetAtribAndUniformLocation()
{
   UnifTexLoc  = glGetUniformLocation(m_progHandle, "Texture");
   UnifGModeLoc = glGetUniformLocation(m_progHandle, "GMode");
   Un_ClrLoc = glGetUniformLocation(m_progHandle, "PColor");
   UnifClipPlane = glGetUniformLocation(m_progHandle, "u_clipPlane");
}


//ESMatrix mvpMatrix;
//ESMatrix IdentMatrix;
PVRTMATRIXf mvpMatrix;
PVRTMATRIXf IdentMatrix;

float Ident[] = {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};

/*
 * В теории discard снижает производительность графического конвейера. Почитать про это можно вбив в Google запрос "Is discard bad for
 * program performance in OpenGL". НА ПРАКТИКЕ же discard увеличивает производительность шейдера отменяя обработку определенных
 * фрагментов на последуующих ступенях графического конвейера.
 *
 * Для рисования шрифта на фоне используется прорисовка фона в режиме GMODE_PAINT + прорисовка шрифта в режиме GMODE_FONT_TON
 * Количество if-ов уменьшено с 5-ти до 3-х
 * Уменьшено количество глобальных и локальных переменных шейдера
 *
 * Можно было бы использовать атрибут PColor вместо атрибута u_FntCol, соответственно избавиться от u_FntCol, но тогда придется править
 * пользовательские функции прорисовки текста. Потерь в производительности использование атрибута u_FntCol НЕ НЕСЕТ
 */
GLbyte FragShader[] =
{
  "#define GMODE_PAINT      0\n"
  "#define GMODE_FONT_TON   1\n"
  "#define GMODE_BMP        4\n"

  "precision mediump float;"
  "uniform sampler2D Texture;"
  //"uniform vec4 u_FntCol;"
  "uniform int GMode;"
  "uniform vec4 PColor;"
  "varying vec2 v_texCoord;"
  "varying mediump vec4 v_Color;"
    "varying float u_clipDist;"

  "void main()"
  "{"
      // Reject fragments behind the clip plane
    "if (u_clipDist < 0.0) discard;"
       
     "if (GMode == GMODE_PAINT)"
	    "gl_FragColor = v_Color;" /* отображение геометрической фигуры */
        //"gl_FragColor = PColor;" /* отображение геометрической фигуры */
     "else {"
        "gl_FragColor = texture2D(Texture, v_texCoord);" /* отображение шрифта или картинки */
		//"if (gl_FragCoord.y < 500.0)"
        "if (gl_FragColor.a == 0.0)"
           "discard;" /* отменить обработку полностью прозрачного фрагмента */
     "}"
  "}"
};

GLbyte VertShader[] =
{
   "uniform mediump mat4 u_mvpMatrix;"
   "attribute highp vec4 a_pos;"
   "attribute mediump vec2 a_txCoord;"
   "attribute mediump vec4 a_Color;"
   "varying mediump vec2 v_texCoord;"
   "varying mediump vec4 v_Color;"
    "uniform vec4 u_clipPlane;"
    "varying float u_clipDist;"

   "void main()"
   "{"
       "gl_Position = u_mvpMatrix*a_pos;"
       "v_texCoord = a_txCoord;"
	   "v_Color = a_Color;"
   
       // Compute the distance between the vertex and the clip plane
       "u_clipDist = dot(a_pos.xyz, u_clipPlane.xyz) + u_clipPlane.w;"
       
   "}"
};

ESContext gesContext;
void mSwapBuffers();
void PtintAttribs(int);

typedef struct
{
   // Handle to a program object
   GLuint programObject;
} UserShader;

UserShader  userData1; //  Handle to a program object

//
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
      return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );

   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled )
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char* infoLog = (char*)malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;
}

int Init(ESContext *esContext)
{
   printf("Graph init 1\n");
   GLint linked;
   m_vertShaderHandle = LoadShader( GL_VERTEX_SHADER, (const char*)VertShader );
   m_fragShaderHandle = LoadShader( GL_FRAGMENT_SHADER, (const char*)FragShader );
   m_progHandle = glCreateProgram();

   if ( m_progHandle == 0 ) {
      return 0;
   }

   glAttachShader( m_progHandle, m_vertShaderHandle );
   glAttachShader( m_progHandle, m_fragShaderHandle );

   // связывание индексов атрибутов с переменными шейдера
   // это действует только до линковки данного программного объекта
   glBindAttribLocation ( m_progHandle, POS_INDEX, "a_pos" );
   glBindAttribLocation ( m_progHandle, TXCOORD_INDEX, "a_txCoord" );
   glBindAttribLocation ( m_progHandle, COL_INDEX, "a_Color" );

   glLinkProgram( m_progHandle );

   // Check the link status -------------------------------
   glGetProgramiv ( m_progHandle, GL_LINK_STATUS, &linked );

   if ( !linked )
   {printf("Err glGetProgramiv\n");
      GLint infoLen = 0;
      glGetProgramiv ( m_progHandle, GL_INFO_LOG_LENGTH, &infoLen );
      if ( infoLen > 1 )
      {
         char* infoLog = new char[ sizeof(char) * infoLen ];

         glGetProgramInfoLog ( m_progHandle, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );

         delete[]  infoLog;
      }
      glDeleteProgram ( m_progHandle );
      return 0;
   }
   eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
   glUseProgram( m_progHandle );

   GetAtribAndUniformLocation(); 

   //glViewport ( 0, 0, esContext->width, esContext->height );
   //SetViewport(0, 0, esContext->width, esContext->height);
   //SetViewport(0, 0, w_Width, w_Heigh);

   UnifMvpMatrixLoc=glGetUniformLocation(m_progHandle, "u_mvpMatrix");
   PVRTMatrixIdentityF(mvpMatrix);
   glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);

   SetViewport(0, 0, esContext->width, esContext->height);
   glUniform4fv(UnifClipPlane, 1, ClipPlane);
      //glmOrtho(0, w_Width, w_Heigh, 0, 0, 1);
   //mSwapBuffers();


   //glClearColor ( 0.0, 0.0, 0.0, 1.0);
   //glClear ( GL_COLOR_BUFFER_BIT );

   // PtintAttribs(3);

   return 1;
}
void PtintAttribs(int k)
{
 printf("PtintAttribs %d\n",k);
 EGLint dt;
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_BUFFER_SIZE,&dt);
 printf("EGL_BUFFER_SIZE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_RED_SIZE,&dt);
 printf("EGL_RED_SIZE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_GREEN_SIZE,&dt);
 printf("EGL_GREEN_SIZE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_BLUE_SIZE,&dt);
 printf("EGL_BLUE_SIZE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_ALPHA_SIZE,&dt);
 printf("EGL_ALPHA_SIZE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_CONFIG_CAVEAT,&dt);
 printf("EGL_CONFIG_CAVEAT = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_CONFIG_ID,&dt);
 printf("EGL_CONFIG_ID = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_DEPTH_SIZE,&dt);
 printf("EGL_DEPTH_SIZE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_LEVEL,&dt);
 printf("EGL_LEVEL = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_NATIVE_RENDERABLE,&dt);
 printf("EGL_NATIVE_RENDERABLE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_NATIVE_VISUAL_TYPE,&dt);
 printf("EGL_NATIVE_VISUAL_TYPE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_SAMPLE_BUFFERS,&dt);
 printf("EGL_SAMPLE_BUFFERS = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_SAMPLES,&dt);
 printf("EGL_SAMPLES = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_STENCIL_SIZE,&dt);
 printf("EGL_STENCIL_SIZE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_SURFACE_TYPE,&dt);
 printf("EGL_SURFACE_TYPE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_TRANSPARENT_TYPE,&dt);
 printf("EGL_TRANSPARENT_TYPE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_TRANSPARENT_RED_VALUE,&dt);
 printf("EGL_TRANSPARENT_RED_VALUE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_TRANSPARENT_GREEN_VALUE,&dt);
 printf("EGL_TRANSPARENT_GREEN_VALUE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_TRANSPARENT_BLUE_VALUE,&dt);
 printf("EGL_TRANSPARENT_BLUE_VALUE = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_RENDER_BUFFER,&dt);
 printf("EGL_RENDER_BUFFER = %d\n",dt);
eglGetConfigAttrib(gesContext.eglDisplay, gesContext.eglConfig, EGL_RENDERABLE_TYPE,&dt);
 printf("EGL_RENDERABLE_TYPE = %d\n",dt);
}

void ESUTIL_API esInitContext ( ESContext *esContext )
{
   if ( esContext != NULL )
   {
      memset( esContext, 0, sizeof( ESContext) );
   }
}

EGLDisplay display = EGL_NO_DISPLAY;
EGLContext context = EGL_NO_CONTEXT;
EGLSurface surface = EGL_NO_SURFACE;
EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface, EGLConfig* eglConfig,
                              EGLint attribList[] )
{
   EGLint numConfigs;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLConfig config=0;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

   // Get Display
   display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
   if ( display == EGL_NO_DISPLAY )
   {
      printf("eglGetDisplay error\n");
      return EGL_FALSE;
   }

   // Initialize EGL
   if ( !eglInitialize(display, &majorVersion, &minorVersion) )
   {
      printf("eglInitialize error\n");
      return EGL_FALSE;
   }
   printf ("EGL Init version: %d.%d\n", majorVersion, minorVersion);
   printf ("EGL Version: %s\n", eglQueryString (display, EGL_VERSION));

   if (!eglBindAPI(EGL_OPENGL_ES_API))
   {
      printf("eglBindAPI error");
      return EGL_FALSE;
   }

   // Choose config
   if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) ||
        (numConfigs!=1) )
   {
      printf("eglChooseConfig error\n");
      return EGL_FALSE;
   }

   // Create a surface
   surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
   if ( surface == EGL_NO_SURFACE )
   {
      printf("eglCreateWindowSurface error\n");
      return EGL_FALSE;
   }

   // Create a GL context
   // PtintAttribs(1);
   context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
   if ( context == EGL_NO_CONTEXT )
   {
      printf("eglCreateContext error\n");
      return EGL_FALSE;
   }

   // PtintAttribs(11);
   // Make the context current
   int k;
   if ( !(k=eglMakeCurrent(display, surface, surface, context) ))
   {
      printf("eglMakeCurrent error\n");
      return EGL_FALSE;
   }

   *eglDisplay = display;
   *eglSurface = surface;
   *eglContext = context;
   *eglConfig = config;
   // PtintAttribs(2);

   return EGL_TRUE;
}

#ifdef _WINDWS_
HWND Get_hWnd();
#endif
GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags )
{
   EGLint attribList[] =
   {
/*
       EGL_RED_SIZE,       8,
       EGL_GREEN_SIZE,     8,
       EGL_BLUE_SIZE,      8,
       EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
       EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
       EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
       EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
*/
       EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
       EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
//     EGL_RENDER_BUFFER,  EGL_BACK_BUFFER,
       EGL_NONE
   };

   if ( esContext == NULL )
   {
      return GL_FALSE;
	  printf("Err1\n");
   }

   esContext->width = width;
   esContext->height = height;
   extern HWND MainHwnd;
   esContext->hWnd=MainHwnd;//Get_hWnd();

   if ( !CreateEGLContext ( esContext->hWnd,
                            &esContext->eglDisplay,
                            &esContext->eglContext,
                            &esContext->eglSurface,
                            &esContext->eglConfig,
                            attribList) )
   {
      return GL_FALSE;
   }

   return GL_TRUE;
}

void ESUTIL_API esLogMessage ( const char *formatStr, ... )
{
#ifdef _WINDWS_
    va_list params;
    char buf[100];

    va_start ( params, formatStr );
    sprintf ( buf, formatStr, params );
    printf ( "%s", buf );
    va_end ( params );
#endif
}

void mSwapBuffers()
{
   //glFlush();
   //eglWaitGL();
   //eglWaitNative(EGL_CORE_NATIVE_ENGINE);
   eglSwapBuffers ( gesContext.eglDisplay, gesContext.eglSurface );
   int iErr;
   if ((iErr = eglGetError()) != EGL_SUCCESS) {
      printf("eglSwapBuffers failed (%d).\n", iErr);
   }
}

void glmOrtho(float left,float right,float bottom ,float top,float nearZ,float farZ) //левый верхн, право, вниз
{

    float w = right - left;
    float h = top - bottom;
    float deltaZ = farZ - nearZ;

    PVRTMATRIXf ort;
    PVRTMatrixOrthoLHF(ort,w,h,nearZ,farZ);
    
	//ort.f[12]=-0.9998;// сдвиг по X в единицах исходных (то есть исх куб x,y,x=(-1...+1))
	//	ort.f[13]=1.0;    // сдвиг по Y


    PVRTMatrixMultiplyF(mvpMatrix,ort,mvpMatrix);
    glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);
}


int Video_init ()
{
   esInitContext ( &gesContext ); // стирание - запись 0й
   gesContext.userData = &userData1;

   esCreateWindow ( &gesContext, "Primitive", w_Width, w_Heigh, ES_WINDOW_RGB );
   if ( !Init ( &gesContext ) ) {
	   printf("Err Video_init\n");
      return 0;
   }
   //glmOrtho(0,w_Width, w_Heigh,0,0,1);
   //mSwapBuffers();

   return 0;
}

PVRTMATRIXf MatrStack[20];
int MatrStack_ind;
void glmPushMatrix()
{
   MatrStack[MatrStack_ind++]=mvpMatrix;
}

void glmPopMatrix()
{
   if(MatrStack_ind<1)return;
   mvpMatrix=MatrStack[--MatrStack_ind];
   glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);
}

// x,y,z - ось вращения
//
void glmRotate(float Angle, float x,float y,float z)
{
   float sinAngle, cosAngle;
   float mag = sqrtf(x * x + y * y + z * z);

   sinAngle = sinf ( Angle * GrRad );
   cosAngle = cosf ( Angle * GrRad );

   if ( mag > 0.0f )
   {
      GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
      GLfloat oneMinusCos;
      PVRTMATRIXf rotMat;

      x /= mag;
      y /= mag;
      z /= mag;

      xx = x * x;
      yy = y * y;
      zz = z * z;
      xy = x * y;
      yz = y * z;
      zx = z * x;
      xs = x * sinAngle;
      ys = y * sinAngle;
      zs = z * sinAngle;
      oneMinusCos = 1.0f - cosAngle;

      rotMat.f[0] = (oneMinusCos * xx) + cosAngle;
      rotMat.f[4] = (oneMinusCos * xy) - zs;
      rotMat.f[8] = (oneMinusCos * zx) + ys;
      rotMat.f[12] = 0.0F;

      rotMat.f[1] = (oneMinusCos * xy) + zs;
      rotMat.f[5] = (oneMinusCos * yy) + cosAngle;
      rotMat.f[9] = (oneMinusCos * yz) - xs;
      rotMat.f[13]= 0.0F;

      rotMat.f[2] = (oneMinusCos * zx) - ys;
      rotMat.f[6] = (oneMinusCos * yz) + xs;
      rotMat.f[10]= (oneMinusCos * zz) + cosAngle;
      rotMat.f[14]= 0.0F;

      rotMat.f[3] = 0.0F;
      rotMat.f[7] = 0.0F;
      rotMat.f[11]= 0.0F;
      rotMat.f[15]= 1.0F;
   }
}

// вращение вокруг оси, параллельной Z ----------
// x0,y0 - координаты центра вращения
void glmRotate_xy(float Angle,float x0, float y0)
{
   PVRTMATRIXf rot,T,T_;
   PVRTMatrixTranslationF(T,x0,y0,0);
   PVRTMatrixTranslationF(T_,-x0,-y0,0);
   PVRTMatrixRotationZF(rot,Angle*GrRad);

   PVRTMatrixMultiplyF(mvpMatrix,T,mvpMatrix);
   PVRTMatrixMultiplyF(mvpMatrix,rot,mvpMatrix);
   PVRTMatrixMultiplyF(mvpMatrix,T_,mvpMatrix);

   glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);
}

// вращение вокруг оси X ----------
void glmRotate_X(float Angle)
{
   static PVRTMATRIXf rot;
static int raz=1;
if(raz){
   PVRTMatrixRotationXF(rot,Angle*GrRad); // это можно сделать глобальным и вычислять раз
   raz=0;
}
   PVRTMatrixMultiplyF(mvpMatrix,rot,mvpMatrix);
   glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);
}

// вращение вокруг оси Z ----------
void glmRotate_Z(float Angle)
{
   PVRTMATRIXf rot;
   PVRTMatrixRotationZF(rot,Angle*GrRad); // это можно сделать глобальным и вычислять раз
   PVRTMatrixMultiplyF(mvpMatrix,rot,mvpMatrix);
   glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);
}



void V_rotate(float Angle, float vx, float vy, float vz)
{
	PVRTMATRIXf Rotate_mtr; // for constant rotate axis and angle
	glmRotate_V(Rotate_mtr,Angle*GrRad,vx, vy,  vz);
	PVRTMatrixMultiplyF(mvpMatrix,Rotate_mtr,mvpMatrix);
	glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);

}//--------------

void Y_rotate(float Angle)
{
	static PVRTMATRIXf Rotate_mtr; // for constant rotate axis and angle
	static int raz=1;
	if(raz){
		glmRotate_V(Rotate_mtr,Angle*GrRad,0, 1, 0);
		raz=0;
	}
	PVRTMatrixMultiplyF(mvpMatrix,Rotate_mtr,mvpMatrix);
	glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);

}//--------------


void glmTranslatef(float x,float y,float z)
{
   PVRTMATRIXf t;
   PVRTMatrixTranslationF(t,x,y,z);
   PVRTMatrixMultiplyF(mvpMatrix,t,mvpMatrix);//tr

   glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);
}

void freeEGLContext()
{
   glDeleteProgram(m_progHandle);
   glDeleteShader(m_fragShaderHandle);
   glDeleteShader(m_vertShaderHandle);

   eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   eglTerminate(display);
}

void SetViewport(int X0, int  Y0, int  W, int  H)
{
    glViewport(X0, Y0, W, H);
    PVRTMatrixIdentityF(mvpMatrix);
    //glmOrtho(0, W, 0, H, -100, 30);
	glmOrtho(-W/2, W/2, -H/2, H/2, -1000, 1000);
    return;
}
