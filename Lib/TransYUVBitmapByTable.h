#ifndef __TRANSYUVBITMAPBYTABLE_H_
#define __TRANSYUVBITMAPBYTABLE_H_


#include <Windows.h>


#include "TransBitmapTableDef.h"


#ifdef __cplusplus
extern "C" {
#endif


#define MAKE_YUV(y, u, v)		(((DWORD)(y) << 16) | ((DWORD)(u) << 8) | ((DWORD)(v)))
#define YUV_Y(color)			(BYTE)(((DWORD)(color) & 0x00FF0000) >> 16)
#define YUV_U(color)			(BYTE)(((DWORD)(color) & 0x0000FF00) >> 8)
#define YUV_V(color)			(BYTE)(((DWORD)(color) & 0x000000FF))

typedef DWORD YUVColor;


BOOL __declspec(dllexport) __stdcall MakeYUV422Bitmap0(int tblY[], int tblU[], int tblV[], BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int outWidth, int outHeight, BYTE srcBmY[], BYTE srcBmU[], BYTE srcBmV[], YUVColor clBorderColor, YUVColor clBlankColor);
BOOL __declspec(dllexport) __stdcall MakeYUV420Bitmap0(int tblY[], int tblU[], int tblV[], BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int outWidth, int outHeight, BYTE srcBmY[], BYTE srcBmU[], BYTE srcBmV[], YUVColor clBorderColor, YUVColor clBlankColor);
BOOL __declspec(dllexport) __stdcall MakeYUV422Bitmap1(int tblY[], int tblU[], int tblV[], BYTE outBmYUV[], int outWidth, int outHeight, BYTE srcBmYUV[], int srcWidth, int srcHeight, YUVColor clBorderColor, YUVColor clBlankColor);
BOOL __declspec(dllexport) __stdcall MakeYUV420Bitmap1(int tblY[], int tblU[], int tblV[], BYTE outBmYUV[], int outWidth, int outHeight, BYTE srcBmYUV[], int srcWidth, int srcHeight, YUVColor clBorderColor, YUVColor clBlankColor);

BOOL __declspec(dllexport) __stdcall MakeYUV422BitmapF0(CoordF tbl[], BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int outWidth, int outHeight, BYTE srcBmY[], BYTE srcBmU[], BYTE srcBmV[], int srcWidth, int srcHeight, YUVColor clBorderColor, YUVColor clBlankColor);
BOOL __declspec(dllexport) __stdcall MakeYUV420BitmapF0(CoordF tbl[], BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int outWidth, int outHeight, BYTE srcBmY[], BYTE srcBmU[], BYTE srcBmV[], int srcWidth, int srcHeight, YUVColor clBorderColor, YUVColor clBlankColor);
BOOL __declspec(dllexport) __stdcall MakeYUV422BitmapF1(CoordF tbl[], BYTE outBmYUV[], int outWidth, int outHeight, BYTE srcBmYUV[], int srcWidth, int srcHeight, YUVColor clBorderColor, YUVColor clBlankColor);
BOOL __declspec(dllexport) __stdcall MakeYUV420BitmapF1(CoordF tbl[], BYTE outBmYUV[], int outWidth, int outHeight, BYTE srcBmYUV[], int srcWidth, int srcHeight, YUVColor clBorderColor, YUVColor clBlankColor);

BOOL __declspec(dllexport) __stdcall SrcYUV422Mark0(int tblY[], int tblU[], int tblV[], int outWidth, int outHeight, BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);
BOOL __declspec(dllexport) __stdcall SrcYUV420Mark0(int tblY[], int tblU[], int tblV[], int outWidth, int outHeight, BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);
BOOL __declspec(dllexport) __stdcall SrcYUV422Mark1(int tblY[], int tblU[], int tblV[], int outWidth, int outHeight, BYTE outBmYUV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);
BOOL __declspec(dllexport) __stdcall SrcYUV420Mark1(int tblY[], int tblU[], int tblV[], int outWidth, int outHeight, BYTE outBmYUV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);

BOOL __declspec(dllexport) __stdcall SrcYUV422MarkF0(CoordF tbl[], int outWidth, int outHeight, BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);
BOOL __declspec(dllexport) __stdcall SrcYUV420MarkF0(CoordF tbl[], int outWidth, int outHeight, BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);
BOOL __declspec(dllexport) __stdcall SrcYUV422MarkF1(CoordF tbl[], int outWidth, int outHeight, BYTE outBmYUV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);
BOOL __declspec(dllexport) __stdcall SrcYUV420MarkF1(CoordF tbl[], int outWidth, int outHeight, BYTE outBmYUV[], int srcWidth, int srcHeight, YUVColor clMakrColor1, YUVColor clMakrColor2);


#ifdef __cplusplus
}
#endif


#endif
