#ifndef __TRANSRGBBITMAPTABLEDEF_H_
#define __TRANSRGBBITMAPTABLEDEF_H_


#include <Windows.h>


#include "TransBitmapTableDef.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef DWORD RGBColor;

#define MAKE_RGB(r, g, b)		(((DWORD)(r) << 16) | ((DWORD)(g) << 8) | ((DWORD)(b)))
#define RGB_R(color)			(BYTE)(((DWORD)(color) & 0x00FF0000) >> 16)
#define RGB_G(color)			(BYTE)(((DWORD)(color) & 0x0000FF00) >> 8)
#define RGB_B(color)			(BYTE)(((DWORD)(color) & 0x000000FF))



#ifdef __cplusplus
}
#endif


#endif
