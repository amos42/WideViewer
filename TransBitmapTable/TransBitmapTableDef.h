#ifndef __TRANSBITMAPTABLEDEF_H_
#define __TRANSBITMAPTABLEDEF_H_


#include <Windows.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct __SizeT {
	int width, height;
} SizeT;

typedef struct __CoordF {
	float x, y;
} CoordF;

typedef struct __BitmapTableT {
	BYTE *ptr;
	int stride;
} BitmapTableT;

typedef DWORD UniColor;


#ifdef __cplusplus
}
#endif


#endif
