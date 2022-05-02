#ifndef __TRANSYUV422BITMAPBYTABLE_H_
#define __TRANSYUV422BITMAPBYTABLE_H_


#include <Windows.h>


#include "TransBitmapTableDef.h"
#include "TransYUVBitmapTableDef.h"
#include "GenTransTableDef.h"


#ifdef __cplusplus
extern "C" {
#endif


#define GET_YUV422PLANE_Y(yuv, width, height)		(yuv)
#define GET_YUV422PLANE_U(yuv, width, height)		((BYTE *)yuv + (width) * (height))
#define GET_YUV422PLANE_V(yuv, width, height)		((BYTE *)yuv + (width) * (height) + ((width) >> 1) * (height))


BOOL __declspec(dllexport) __stdcall MakeYUV422TransTable0(int transYTbl[], int transUTbl[], int transVTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight,
										  int srcStrideY, int srcStrideU, int srcStrideV,
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam);

BOOL __declspec(dllexport) __stdcall MakeYUV422Bitmap0(int tblY[], int tblU[], int tblV[], 
													   BYTE outBmY[], int outBmYStride,
													   BYTE outBmU[], int outBmUStride, 
													   BYTE outBmV[], int outBmVStride,
													   int outWidth, int outHeight, 
													   BYTE srcBmY[], BYTE srcBmU[], BYTE srcBmV[],
													   YUVColor clBorderColor, YUVColor clBlankColor);
BOOL __declspec(dllexport) __stdcall MakeYUV422BitmapF0(CoordF tbl[], BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int outWidth, int outHeight, BYTE srcBmY[], BYTE srcBmU[], BYTE srcBmV[], int srcWidth, int srcHeight, YUVColor clBorderColor, YUVColor clBlankColor);

BOOL __declspec(dllexport) __stdcall SrcYUV422Mark0(int tblY[], int tblU[], int tblV[], int outWidth, int outHeight, BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);
BOOL __declspec(dllexport) __stdcall SrcYUV422MarkF0(CoordF tbl[], int outWidth, int outHeight, BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);


#ifdef __cplusplus
}
#endif


#endif
