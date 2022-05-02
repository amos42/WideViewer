#ifndef __TRANSBITMAPAUX_H_
#define __TRANSBITMAPAUX_H_

#include <Windows.h>
#include "TransBitmapTableDef.h"
#include "GenTransTableDef.h"


#ifdef __cplusplus
extern "C" {
#endif


#define TRANS_OPT_MASK				0x7F000000
#define TRANS_OPT_RGB24				0x01000000
#define TRANS_OPT_YUV422			0x02000000
#define TRANS_OPT_YUV420			0x04000000
#define TRANS_OPT_YUV444			0x08000000
#define TRANS_OPT_SPEED				0x80000000
#define TRANS_OPT_QUALITY			0x40000000

#ifndef RENDER_OPT_MASK
#define RENDER_OPT_MASK				0x00FF0000
#endif
//#define RENDER_OPT_DEBUG_SRCMARK	0x00800000


int __declspec(dllexport) __stdcall GetRequireHandleSize(int outWidth, int outHeight, DWORD dwTransOpt);
BOOL __declspec(dllexport) __stdcall InitTransBitmapHandle(void *handlePtr, int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
                                          int srcWidth, int srcHeight,
                                          int srcStride[], int srcStrideCnt,
										  int rowCount, int colCount, int splitWidth,
										  DWORD dwTransOpt,
										  Distortion *distParam);
BOOL __declspec(dllexport) __stdcall GetSrcBitmapSize(void *handlePtr, SizeT *size);
BOOL __declspec(dllexport) __stdcall GetOutBitmapSize(void *handlePtr, SizeT *size);
BOOL __declspec(dllexport) __stdcall TransBitmapByHandle0(void *handlePtr, BYTE *outBm[], BYTE *srcBm[], UniColor clBorderColor, UniColor clBlankColor, DWORD dwRenderOpt);
BOOL __declspec(dllexport) __stdcall TransBitmapByHandle(void *handlePtr, BYTE outBm[], BYTE srcBm[], DWORD dwRenderOpt);
BOOL __declspec(dllexport) __stdcall SrcMarkBitmapByHandle0(void *handlePtr, BYTE *outBm[], BYTE *srcBm[], UniColor clBorderColor, UniColor clBlankColor, DWORD dwRenderOpt);
BOOL __declspec(dllexport) __stdcall SrcMarkBitmapByHandle(void *handlePtr, BYTE outBm[], BYTE srcBm[], DWORD dwRenderOpt);

BOOL __declspec(dllexport) __stdcall MakeTransBitmap(BYTE *outBm[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
										  BYTE *srcBm[],
                                          int srcWidth, int srcHeight,
                                          int srcStride[], int srcStrideCnt,
										  DWORD dwTransOpt, DWORD dwRenderOpt,
										  Distortion *distParam);


#ifdef __cplusplus
}
#endif

#endif
