#ifndef __TRANSYUVBITMAPTABLEDEF_H_
#define __TRANSYUVBITMAPTABLEDEF_H_


#include <Windows.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef DWORD YUVColor;

#define MAKE_YUV(y, u, v)		(((DWORD)(y) << 16) | ((DWORD)(u) << 8) | ((DWORD)(v)))
#define YUV_Y(color)			(BYTE)(((DWORD)(color) & 0x00FF0000) >> 16)
#define YUV_U(color)			(BYTE)(((DWORD)(color) & 0x0000FF00) >> 8)
#define YUV_V(color)			(BYTE)(((DWORD)(color) & 0x000000FF))

#define BLANK_Y (0)
#define BLANK_U (128)
#define BLANK_V (128)
#define BLANK_YUV       (MAKE_YUV(BLANK_Y, BLANK_U, BLANK_V))


#ifdef __cplusplus
}
#endif


#endif
