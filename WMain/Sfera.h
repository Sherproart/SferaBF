struct FPoint {
    float x, y, z;
    float n[3];
    void Set(float px, float py, float pz) { x = px; y = py; z = pz; };
    void SetN(float px, float py, float pz) { n[0] = px; n[1] = py; n[2] = pz; };
    void PosToN() { SetN(1,0,0); };
    //void PosToN() { SetN(-x, -y, -z ); };
    void Copy(FPoint* pp) { x = pp->x; y = pp->y; };
    FPoint(){};
};

struct TPoligon {
    FPoint* pnt;
    int Type; // GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
    float Color[4];
    void SetColor(float* clr) { memcpy(Color, clr, 4 * sizeof(float)); };
    void Create(int cnt_);// clear points
    TPoligon() {};
    void Draw();
    void DrawRndColor();
    void DrawLine();

    int cnt;
};

// конус, вписаний в нап≥всферу
struct TCone {
    float R; int Na;
    float dLa;
    TPoligon* pgn;
    int cnt;
    TCone(float R_, int  Na_);
    void Create();
};

struct TSfera {
    TPoligon* pgn;
    float R; int Nz, Na;
    float dFi, dLa;
    int cnt;
    TSfera(float R_, int Nz_, int  Na_);
    void Create();
    void CreateFull();
    void DrawPoligonR();
    void DrawLine();

};

extern TSfera Sf;

/*
¬ модели освещени€ член, учитывающий рассто€ние до источника, имеет вид:
K= 1/(a+ b*d+ c*d^2)
и посто€нную, линейную и квадратичную составл€ющие. —оответствующие коэффициенты дл€ 
каждого источника задаютс€ индивидуально с помощью функции установки скал€рных параметров, например:
glLightf(GL_LIGHT0, GL_CONSTANT_ATTENATION, a);

все нюанси тут: http://media.ls.urfu.ru/435/1147/2452/

шейдерное освещение по ‘онгу
http://eax.me/opengl-lighting/
*/