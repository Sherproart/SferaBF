#include "stdafx.h"
#include <math.h>
#include "esUtil.h"
#include "PVRTMatrix.h"
#include <stdio.h>
#include "graph.h"
#include "WMain.h"

extern PVRTMATRIXf mvpMatrix;

void PVRTMATRIXf:: print()
{
    printf("%.0f\t%.0f\t%.0f\t%.0f\r\n", f[0], f[4], f[8], f[12]);
    printf("%.0f\t%.0f\t%.0f\t%.0f\r\n", f[1], f[5], f[9], f[13]);
    printf("%.0f\t%.0f\t%.0f\t%.0f\r\n", f[2], f[6], f[10], f[14]);
    printf("%.0f\t%.0f\t%.0f\t%.0f\r\n\r\n", f[3], f[7], f[11], f[15]);

}// PVRTMATRIXf:: print-------------


void PVRTMATRIXf:: print2()
{
    printf("%.2f\t%.2f\t%.2f\t%.2f\r\n",   f[0], f[4], f[8], f[12]);
    printf("%.2f\t%.2f\t%.2f\t%.2f\r\n",   f[1], f[5], f[9], f[13]);
    printf("%.2f\t%.2f\t%.2f\t%.2f\r\n",   f[2], f[6], f[10], f[14]);
    printf("%.2f\t%.2f\t%.2f\t%.2f\r\n\r\n", f[3], f[7], f[11], f[15]);

}// PVRTMATRIXf:: print-------------

void PVRTMATRIXf::print5()
{
    printf("%.5f\t%.5f\t%.5f\t%.5f\r\n", f[0], f[4], f[8], f[12]);
    printf("%.5f\t%.5f\t%.5f\t%.5f\r\n", f[1], f[5], f[9], f[13]);
    printf("%.5f\t%.5f\t%.5f\t%.5f\r\n", f[2], f[6], f[10], f[14]);
    printf("%.5f\t%.5f\t%.5f\t%.5f\r\n\r\n", f[3], f[7], f[11], f[15]);

}// PVRTMATRIXf:: print-------------

void PVRTMATRIXf::Set(
    float a0, float a1, float a2, float a3,
    float a4, float a5, float a6, float a7,
    float a8, float a9, float a10, float a11,
    float a12, float a13, float a14, float a15
    )
{
        // сканирование по столбцам
        f[0] = a0, f[4] = a1, f[8] = a2, f[12] = a3,
        f[1] = a4, f[5] = a5, f[9] = a6, f[13] = a7,
        f[2] = a8, f[6] = a9, f[10] = a10, f[14] = a11,
        f[3] = a12, f[7] = a13, f[11] = a14, f[15] = a15;

}//-----


void TestMatr1()
{
    PVRTMATRIXf A;
    PVRTMATRIXf B;
    PVRTMATRIXf C;
    PVRTMATRIXf V;
    PVRTMATRIXf E;
    PVRTMATRIXf Sc;
    PVRTMATRIXf R,P;

    E.Set(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
        );
    
    B.Set(
        1, 0, 0, 2,
        0, 1, 0, 3,
        0, 0, 1, 4,
        0, 0, 0, 1
        );

    Sc.Set(
        0.1, 0, 0, 0,
        0, .1, 0,  0,
        0, 0, .1,  0,
        0, 0,  0,   1
        );

    P.Set(
        2, 0,  0, 2,
        0, 11, 0, 3,
        0, 0,  8, 4,
        0, 0,  0, 1
        );


    V.Set(
        5, 0, 0, 0,
        7, 0, 0, 0,
        6, 0, 0, 0,
        1, 0, 0, 0
        );

    R.Set(
        10, 0, 0, 0,
        77, 0, 0, 0,
        48, 0, 0, 0,
        1, 0, 0, 0
        );


    //PVRTMatrixMultiplyF(C, B, V);
    PVRTMatrixMultiplyF(C, Sc, B);
    Sc.print5();
    B.print5();
    C.print5();


}//------------------


// как умножается в шейдере матрица на вектор ?
// справа на вектор столбец?
void TestMatrVect()
{

	//float v[] = { 0.1,0.3,0.5,    0.2,0.3, 0.5};
    float v[] = { 0,0,0,    100,200, 0 };

	PVRTMATRIXf A,C,B;

    /*
    mvpMatrix.Set(
         1,  2, 0,  0,
         0,  0, 0,  0,
         0,  0, 0,  0,
         0,  0, .5, 1
        );
	glUniformMatrix4fv( UnifMvpMatrixLoc, 1, GL_FALSE, mvpMatrix.f);
    */

    A.Set(
        100, 0, 0, 0,
        200, 0, 0, 0,
         0, 0, 0, 0,
        1, 0, 0, 0
        );

    B.Set(
        .1,.3,.5, 1,
         0, 0, 0, 0,
         0, 0, 0, 0,
         0, 0, 0, 0
        );

    SetSimetricOrtho(w_Width, w_Heigh, 1000);

    PDraw3D(v, 2, GL_LINES, clmWhite);
    glmTranslatef(50, 40, 0);
	//PVRTMatrixMultiplyF(C, mvpMatrix, A);
	PVRTMatrixMultiplyF(C, mvpMatrix, A);
	//PVRTMatrixMultiplyF(C,  A, mvpMatrix);
    mvpMatrix.print5();
	C.print5();



	PDraw3D(v,2,GL_LINES, clmBlue);

	mSwapBuffers();

}//-----

