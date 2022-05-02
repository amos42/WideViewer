#ifndef __CALCLENS_H_
#define __CALCLENS_H_

#include <Windows.h>

double __declspec(dllexport) __stdcall PosToAngle(int CenterX, int CenterY, int x, int y);
double __declspec(dllexport) __stdcall AngleToDist(double angle);


#endif
