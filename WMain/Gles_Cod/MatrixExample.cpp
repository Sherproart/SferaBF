#include "stdafx.h"
#include <math.h>
#include "PVRTMatrix.h"
#include <stdio.h>

void PVRTMATRIXf:: print()
{
    printf("%.0f\t%.0f\t%.0f\t%.0f\r\n", f[0], f[1], f[2], f[3]);
    printf("%.0f\t%.0f\t%.0f\t%.0f\r\n", f[4], f[5], f[6], f[7]);
    printf("%.0f\t%.0f\t%.0f\t%.0f\r\n", f[8], f[9], f[10], f[11]);
    printf("%.0f\t%.0f\t%.0f\t%.0f\r\n\r\n", f[12], f[13], f[14], f[15]);

}// PVRTMATRIXf:: print-------------


void PVRTMATRIXf::Set(
    float a0, float a1, float a2, float a3,
    float a4, float a5, float a6, float a7,
    float a8, float a9, float a10, float a11,
    float a12, float a13, float a14, float a15
    )
{
        f[0] = a0, f[4] = a4, f[8] = a8, f[12] = a12,
        f[1] = a1, f[5] = a5, f[9] = a9, f[13] = a13,
        f[2] = a2, f[6] = a6, f[10] = a10, f[14] = a14,
        f[3] = a3, f[7] = a7, f[11] = a11, f[15] = a15;

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
        2, 0, 0, 0,
        0, 11, 0, 0,
        0, 0, 8,  0,
        0, 0, 0,  1
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


    PVRTMatrixMultiplyF(C, P, V);
    P.print();
    V.print();
    C.print();


}//------------------