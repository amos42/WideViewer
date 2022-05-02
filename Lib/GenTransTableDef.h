#ifndef __GENTRANSTABLEDEF_H_
#define __GENTRANSTABLEDEF_H_

#include <Windows.h>


#include "TransBitmapTableDef.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct __BitmapInfo {
	int x, y;
	int width, height;
	int widthScride;
	int pixelBytes;
} BitmapInfo;

typedef struct __Distortion {
	BYTE algorithm[8];
	float param[8];
} Distortion;


typedef void (CALLBACKFUNC)(void *param, int desX, int desY, int desWidth, int desHeight, float srcX, float srcY, int srcWidth, int srcHeight, int srcWidthStride, float pixelWSize, float pixelHSize);


#ifdef __cplusplus
}
#endif

#endif
