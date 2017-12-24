#pragma once

#include "resource.h"

void Test3();
void TestS();

int Video_init();

extern float s_Width, s_Heigh;
extern int w_Width, w_Heigh; // current window from msg sizeng

extern HDC hdc_mem;

void OnPaintDraw();
void ShowConsoleWindow();

void Test1();
void Test2();
extern HWND MainHwnd;
void GetWindowSise(HWND hWnd);
void SpecularEnDis();
void DiffuseEnDis();
