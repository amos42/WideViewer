#ifndef __TRANSRGBBITMAPBYTABLE_H_
#define __TRANSRGBBITMAPBYTABLE_H_


#include <Windows.h>


#include "TransBitmapTableDef.h"
#include "TransRGBBitmapTableDef.h"
#include "GenTransTableDef.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifndef RENDER_OPT_MASK
#define RENDER_OPT_MASK			0x00FF0000
#endif
#define RENDER_OPT_RGB			0x00000000
#define RENDER_OPT_BGR			0x00010000


BOOL __declspec(dllexport) __stdcall MakeRGBTransTable0(int transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, 
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam);

BOOL __declspec(dllexport) __stdcall MakeRGBBitmap0(int tbl[], BYTE outBm[], int outWidth, int outHeight, BYTE srcBm[], RGBColor clBorderColor, RGBColor clBlankColor, DWORD dwRenderOpt);

BOOL __declspec(dllexport) __stdcall MakeRGBBitmapF0(CoordF tbl[], BYTE outBm[], int outWidth, int outHeight, BYTE srcBm[], int srcWidth, int srcHeight, RGBColor clBorderColor, RGBColor clBlankColor, DWORD dwRenderOpt);

BOOL __declspec(dllexport) __stdcall SrcRGBMark0(int tbl[], int outWidth, int outHeight, BYTE srcBm[], RGBColor clMakrColor1, RGBColor clMakrColor2, DWORD dwRenderOpt);

BOOL __declspec(dllexport) __stdcall SrcRGBMarkF0(CoordF tbl[], int outWidth, int outHeight, BYTE srcBm[], int srcWidth, int srcHeight, RGBColor clMakrColor1, RGBColor clMakrColor2, DWORD dwRenderOpt);


#ifdef __cplusplus
}
#endif


#endif
