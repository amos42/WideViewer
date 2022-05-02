// TransBitmap.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include "CalcLens.h"
#include <windows.h>

#include "Ang2Rih.h"
#include "Rih2Ang.h"


// 정확히 일치하지 않더라도 중간 인덱스를 리턴해야 한다.
//static int _bsearch( double key, const double (*base)[2], int nelem )
//{
//    double *kmin, *probe;
//    double *probe0, min = 0x7FFFFFFF;
//    double i, j;
//
//    kmin = (double *)base;
//    while( nelem > 0 ){
//      i = nelem >> 1; // nelem / 2
//      probe = kmin + i;
//      j = key - *probe;
//      if( j == 0 )
//        return probe;
//      else {
//        if( j < min ){
//          min = j;
//          probe0 = probe;
//        }
//        if( j < 0 )
//          nelem = i;
//        else {
//          kmin = probe + 1;
//          nelem -= i + 1;
//        }
//      }
//    }
//
//    return *(probe0 + 1];
//}


double __declspec(dllexport) __stdcall PosToAngle(int CenterX, int CenterY, int x, int y)
{
	double dist = sqrt((x - CenterX) * (x - CenterX) + (y - CenterY) * (y - CenterY));
	double value = 0;
	dist *= 0.0022;
	for(int i = 1; i < 973; i++ ){
		if(Rih2Ang[i][0] > dist) {
			value = Rih2Ang[i-1][0];
			break;
		}
	}
	return value;
}

double __declspec(dllexport) __stdcall AngleToDist(double angle)
{
	double value = 0;

	if(angle < 0 || angle > 100) {
		return -1;
	}

	int ang2 = (int)(angle * 10);

	double dist = Ang2Rih[ang2][0];
	dist *= 0.0022;

	return dist;
}
