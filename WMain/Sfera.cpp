#include "stdafx.h"
#include "workGL.h"
#include <stdio.h>
#include "math.h"
#include "sfera.h"

float Pi = atan(1) * 4;

TSfera Sf(200,4,3);

/**
 создает пустой массив точек длиной cnt
*/
void TPoligon::Create(int cnt_)
{
    cnt = cnt_; 
    if (cnt == 0)return;
    pnt = new FPoint[cnt];

}//TPoligon::TPoligon---------------

float RndColor[4];
int randInt(int min, int max)
{
    int r = rand() % (max - min + 1) + min;
    return r;
}

void RandomColor(float* out) {
    out[0] = randInt(50, 255); out[0] /= 255;
    out[1] = randInt(50, 255); out[1] /= 255;
    out[2] = randInt(50, 255); out[2] /= 255;
    out[3] = 1;
}

void TPoligon::Draw()
{
    //glDisable(GL_TEXTURE_2D);
    //glColor4fv(White);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(FPoint), pnt); // 2 - число координат вершины

    //float* Norm = (float*)pnt; Norm += 3;
    //glEnableClientState(GL_NORMAL_ARRAY);
    //glNormalPointer(GL_FLOAT, 3 * sizeof(FPoint), Norm);

    glDrawArrays(GL_TRIANGLE_FAN, 0, cnt);

}

// полигон со случайным цветом
void TPoligon::DrawRndColor()
{
    glColor4fv(Color);
    Draw();

}//TPoligon::DrawRndColor---------------

void TPoligon::DrawLine()
{
    //glDisable(GL_TEXTURE_2D);
    glColor4fv(White);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(FPoint), pnt); // 2 - число координат вершины
    glDrawArrays(GL_LINE_STRIP, 0, cnt);

}
//TPoligon::Draw----------------------

TSfera::TSfera(float R_, int Nz_, int Na_)
{
    R = R_; Nz = Nz_; Na = Na_;
    dFi = Pi / Nz;
    dLa = 2*Pi / Na;
    Create();
    //CreateFull();
}

void TSfera::Create()
{
    // ссилки на полігони
    int Np = (Nz - 2)*Na + 2;
    pgn = new TPoligon[Np]; // Nz-количество групп полигонов (поясов)

    float R1 = R*sin(dFi);
    float Z1 = R*cos(dFi);

    pgn[0].Create(Na + 2); // веерн полигон-конус
    pgn[0].pnt[0].Set(0, 0, R); // ближний полюс (вершина веера)
    pgn[0].pnt[0].SetN(0, 0, 1);
    RandomColor(RndColor);
    pgn[0].SetColor(RndColor);
    for (int i = 0; i <= Na; i++)
    {
        pgn[0].pnt[i+1].Set(R1*cos(dLa*i), R1*sin(dLa*i), Z1);
        pgn[0].pnt[i + 1].PosToN();
    }

    int ind = 1;
    // Na 4х вершинних окремих полигонів в кожному поясі
    for (int u = 1; u < (Nz-1); u++)
    {
        float AngTp = dFi*u; // for Top level
        float AngDn = dFi*(u + 1); // for down level
        float Rd = R*sin(AngDn);
        float Zd = R*cos(AngDn);
        float Rt = R*sin(AngTp);
        float Zt = R*cos(AngTp);

        for (int i = 0; i < Na; i++)
        {
            pgn[ind].Create(4); // 4 вершини
            float La1 = dLa*i; float La2 = dLa*(i + 1);
            pgn[ind].pnt[4 * i].Set(Rt*cos(La1), Rt*sin(La1), Zt);
            pgn[ind].pnt[4 * i + 1].Set(Rt*cos(La2), Rt*sin(La2), Zt);
            pgn[ind].pnt[4 * i + 2].Set(Rd*cos(La2), Rd*sin(La2), Zd);
            pgn[ind].pnt[4 * i + 3].Set(Rd*cos(La1), Rd*sin(La1), Zd);

            RandomColor(RndColor);
            pgn[ind].SetColor(RndColor);
            ind++;
        }
    }

    pgn[Np-1].Create(Na + 2); // веерн полигон-конус
    pgn[Np-1].pnt[0].Set(0, 0, -R); // дальний полюс (вершина веера)
    pgn[Np-1].pnt[0].SetN(0, 0, -1);
    for (int i = 0; i <= Na; i++)
    {
        pgn[Np-1].pnt[i + 1].Set(R1*cos(dLa*i), R1*sin(dLa*i), -Z1);
        pgn[Np-1].pnt[i + 1].PosToN();
    }
    RandomColor(RndColor);
    pgn[Np-1].SetColor(RndColor);



}//TSfera::Create-----------------------------

void TSfera::CreateFull()
{
    // ссилки на полігони
    pgn = new TPoligon[Nz]; // Nz-количество групп полигонов (поясов)

    float R1 = R*sin(dFi);
    float Z1 = R*cos(dFi);

    pgn[0].Create(Na*3); // веерн полигон-конус
    for (int i = 0; i < Na; i++)
    {
        pgn[0].pnt[3*i].Set(0, 0, R); // ближний полюс (вершина веера)
        pgn[0].pnt[3*i + 1].Set(R1*cos(dLa*i), R1*sin(dLa*i), Z1);
        pgn[0].pnt[3*i + 2].Set(R1*cos(dLa*(i+1)), R1*sin(dLa*(i+1)), Z1);
    }

    for (int u = 1; u < Nz - 1; u++)
    {
        float AngTp = dFi*u; // for Top level
        float AngDn = dFi*(u+1); // for down level
        float Rd = R*sin(AngDn);
        float Zd = R*cos(AngDn);
        float Rt = R*sin(AngTp);
        float Zt = R*cos(AngTp);

        pgn[u].Create(Na * 5); // веерн полигон-конус
        for (int i = 0; i < Na; i++)
        {
            float La1 = dLa*i; float La2 = dLa*(i+1);
            pgn[u].pnt[5*i].Set(Rt*cos(La1), Rt*sin(La1), Zt);
            pgn[u].pnt[5*i + 1].Set(Rt*cos(La2), Rt*sin(La2), Zt);
            pgn[u].pnt[5*i + 2].Set(Rd*cos(La2), Rd*sin(La2), Zd);
            pgn[u].pnt[5*i + 3].Set(Rd*cos(La1), Rd*sin(La1), Zd);
            pgn[u].pnt[5*i + 4].Set(Rt*cos(La1), Rt*sin(La1), Zt);
        }

    }

    pgn[Nz-1].Create(Na * 3); // веерн полигон-конус
    for (int i = 0; i < Na; i++)
    {
        pgn[Nz-1].pnt[3*i].Set(0, 0, -R); // ближний полюс (вершина веера)
        pgn[Nz-1].pnt[3*i + 1].Set(R1*cos(dLa*i), R1*sin(dLa*i), -Z1);
        pgn[Nz-1].pnt[3*i + 2].Set(R1*cos(dLa*(i + 1)), R1*sin(dLa*(i + 1)), -Z1);
    }

    cnt = Nz;

    
}
void TSfera::DrawPoligonR()
{
    for (int i = 0; i < cnt; i++)
    {
        pgn[i].DrawRndColor();
    }

}//TSfera::DrawPoligonR----------

void TSfera::DrawLine()
{
    //pgn[3].DrawLine();return;
    
    for (int i = 0; i < cnt; i++)
    {
        pgn[i].DrawLine();
    }
}
//TSfera::TSfera-----------------

TCone::TCone(float R_, int Na_)
{
    R = R_; Na = Na_;
    dLa = 2 * Pi / Na;
}

