#ifndef __GENTRANSTABLE_H_
#define __GENTRANSTABLE_H_

#include <Windows.h>


#include "TransBitmapTableDef.h"
#include "GenTransTableDef.h"


#ifdef __cplusplus
extern "C" {
#endif


BOOL __declspec(dllexport) MakeTransTable0(CALLBACKFUNC *func, void *param, int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, int srcWidthStride, float srcPixelWSize, float pixelHSize, 
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam);

BOOL __declspec(dllexport) MakeTransTable(int transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, int srcWidthStride, float srcPixelWSize, float pixelHSize,
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam);

BOOL __declspec(dllexport) __stdcall MakeTransTableF(CoordF transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, 
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam);


#ifdef __cplusplus
}
#endif

#endif
