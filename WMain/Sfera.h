struct SPoint {
    float x, y, z;
    float n[3];
    void Set(float px, float py, float pz) { x = px; y = py; z = pz; };
    void SetN(float px, float py, float pz) { n[0] = px; n[1] = py; n[2] = pz; };
    void PosToN() { SetN(x,y,z); };
    //void PosToN() { SetN(-x, -y, -z ); };
    void Copy(SPoint* pp) { x = pp->x; y = pp->y; };
    SPoint(){};
};

struct TPoligon {
    SPoint* pnt;
    int Type; // GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
    float Color[4];
    void SetColor(float* clr) { memcpy(Color, clr, 4 * sizeof(float)); };
    void Create(int cnt_);// clear points
    TPoligon() {};
    void Draw();
    void DrawRndColor();
    void DrawLine();
	void DrawLineP();
	void DrawMrk(int Pcnt, float* clr); // первые Pcnt точек

    int cnt;
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
    void DrawPoligonR1();
    void DrawLine();

};

extern TSfera Sf;

extern float Blue[];
extern float White[];
extern float Green[];
extern float Red[];
extern int Stop;
extern float Pi;


/*
В модели освещения член, учитывающий расстояние до источника, имеет вид:
K= 1/(a+ b*d+ c*d^2)
и постоянную, линейную и квадратичную составляющие. Соответствующие коэффициенты для 
каждого источника задаются индивидуально с помощью функции установки скалярных параметров, например:
glLightf(GL_LIGHT0, GL_CONSTANT_ATTENATION, a);

все нюанси тут: http://media.ls.urfu.ru/435/1147/2452/

шейдерное освещение по Фонгу
http://eax.me/opengl-lighting/

освещение по Фонгу и Блинну-Фонгу !
http://compgraphics.info/3D/lighting/phong_reflection_model.php

// карта (текстура) затенения
http://tiarum.com/wiki/Modding:Особенности_моделирования_света:_Аппроксимации_Фонга_(Phong_Shading)

Отраженное (бликовое, specular) освещение:
 Ispec = Ks*dot(H,N)^a*Is;
 H = (L + V)нормализованное   (как вектора)

 Ispec - Бликовая интенсивность
 Ks - коэффициент зеркального отражения
 Is - мощность бликового освещения
 Н - вектор полупути  (halfway vector)
 N - нормаль
 L - направление из точки на источник
 V - направление на наблюдателя

*/