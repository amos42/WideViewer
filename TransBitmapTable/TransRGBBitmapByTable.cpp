// TransBitmap.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include "TransRGBBitmapByTable.h"
#include "GenTransTable.h"
#include <stdlib.h>


/*
static void func1(void *param, int desX, int desY, int desWidth, int desHeight, float srcX, float srcY, int srcWidth, int srcHeight, int srcWidthStride, float pixelBytes)
{
	int *transTbl = (int *)param;

	int addr = desY * desWidth + desX;
    if (srcX >= 0 && srcX < srcWidth && srcY >= 0 && srcY < srcHeight)
    {
        transTbl[addr] = (int)srcY * srcWidthStride + (int)srcX * pixelBytes;
    }
    else
    {
        transTbl[addr] = -2;
    }
}
*/

BOOL __declspec(dllexport) __stdcall MakeRGBTransTable0(int transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, 
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam)
{
	/*
	return MakeTransTable0(func1, transTbl, outWidth, outHeight, 
                                          centerX, centerY, innerRadius, outerRadius, 
                                          srcWidth, srcHeight, srcWidth * 3, 3,
                                          startAngle, sweepAngle,
										  sx, sy, width, height,
										  distParam);
	*/

	return MakeTransTable(transTbl, outWidth, outHeight, 
                                          centerX, centerY, innerRadius, outerRadius, 
                                          srcWidth, srcHeight, srcWidth * 3, 3, 1,
                                          startAngle, sweepAngle,
										  sx, sy, width, height,
										  distParam);
}

BOOL __declspec(dllexport) __stdcall MakeRGBBitmap0(int tbl[], BYTE outBm[], int outWidth, int outHeight, BYTE srcBm[], RGBColor clBorderColor, RGBColor clBlankColor, DWORD dwRenderOpt)
{
    if (tbl == NULL) return FALSE;

	BYTE blank_r = RGB_R(clBlankColor);
	BYTE blank_g = RGB_G(clBlankColor);
	BYTE blank_b = RGB_B(clBlankColor);
	BYTE border_r = RGB_R(clBorderColor);
	BYTE border_g = RGB_G(clBorderColor);
	BYTE border_b = RGB_B(clBorderColor);

	BYTE *pOutBm = outBm;
	BYTE *pSrcBm;
	int len = outWidth * outHeight;

	if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_RGB) 
	{
		for (int i = 0; i < len; i++)
		{
			int j = tbl[i];
			if (j >= 0)
			{
				pSrcBm = &srcBm[j];
				*pOutBm++ = *pSrcBm++;
				*pOutBm++ = *pSrcBm++;
				*pOutBm++ = *pSrcBm;
			}
			else if (j == -1)
			{
				*pOutBm++ = border_r;
				*pOutBm++ = border_g;
				*pOutBm++ = border_b;
			} 
			else if (j == -2)
			{
				*pOutBm++ = blank_r;
				*pOutBm++ = blank_g;
				*pOutBm++ = blank_b;
			}
			else
			{
				*pOutBm++ = 0;
				*pOutBm++ = 0;
				*pOutBm++ = 0;
			}
		}
	} 
	else if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_BGR) 
	{
		for (int i = 0; i < len; i++)
		{
			int j = tbl[i];
			if (j >= 0)
			{
				pSrcBm = &srcBm[j + 2];
				*pOutBm++ = *pSrcBm--;
				*pOutBm++ = *pSrcBm--;
				*pOutBm++ = *pSrcBm;
			}
			else if (j == -1)
			{
				*pOutBm++ = border_b;
				*pOutBm++ = border_g;
				*pOutBm++ = border_r;
			} 
			else if (j == -2)
			{
				*pOutBm++ = blank_b;
				*pOutBm++ = blank_g;
				*pOutBm++ = blank_r;
			}
			else
			{
				*pOutBm++ = 0;
				*pOutBm++ = 0;
				*pOutBm++ = 0;
			}
		}
	}
    return TRUE;
}


BOOL __declspec(dllexport) __stdcall MakeRGBBitmapF0(CoordF tbl[], BYTE outBm[], int outWidth, int outHeight, BYTE srcBm[], int srcWidth, int srcHeight, RGBColor clBorderColor, RGBColor clBlankColor, DWORD dwRenderOpt)
{
    if (tbl == NULL) return FALSE;

	BYTE blank_r = 0x00;
	BYTE blank_g = 0x00;
	BYTE blank_b = 0x00;
	BYTE border_r = 0x00;
	BYTE border_g = 0x00;
	BYTE border_b = 0x00;
	if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_RGB) {
		blank_r = RGB_R(clBlankColor);
		blank_g = RGB_G(clBlankColor);
		blank_b = RGB_B(clBlankColor);
		border_r = RGB_R(clBorderColor);
		border_g = RGB_G(clBorderColor);
		border_b = RGB_B(clBorderColor);
	} else if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_BGR) {
		blank_r = RGB_B(clBlankColor);
		blank_g = RGB_G(clBlankColor);
		blank_b = RGB_R(clBlankColor);
		border_r = RGB_B(clBorderColor);
		border_g = RGB_G(clBorderColor);
		border_b = RGB_R(clBorderColor);
	}

	int len = outWidth * outHeight;
	BYTE *pOutBm = outBm;
	BYTE *pSrcBm;

	for (int i = 0; i < len; i++)
	{
		float ptx = tbl[i].x;
		float pty = tbl[i].y;

		if (ptx > -9999)
		{
			int iptx = (int)ptx;
			int ipty = (int)pty;

			float x2_t = (float)(ptx - iptx);
			float x1_t = 1 - x2_t;
			float y2_t = (float)(pty - ipty);
			float y1_t = 1 - y2_t;

			int idx0 = (ipty * srcWidth + iptx) * 3;
			pSrcBm = &srcBm[idx0];
			int r = *pSrcBm++;
			int g = *pSrcBm++;
			int b = *pSrcBm;

			float fr = x1_t * y1_t;
			int rr = (int)(r * fr);
			int gg = (int)(g * fr);
			int bb = (int)(b * fr);
			if (x1_t < 1 && iptx + 1 < srcWidth)
			{
				pSrcBm = &srcBm[idx0 + 3];
				r = *pSrcBm++;
				g = *pSrcBm++;
				b = *pSrcBm;
				fr = x2_t * y1_t;
				rr += (int)(r * fr);
				gg += (int)(g * fr);
				bb += (int)(b * fr);
			}
			if (y1_t < 1 && ipty + 1 < srcHeight)
			{
				pSrcBm = &srcBm[idx0 + srcWidth * 3];
				r = *pSrcBm++;
				g = *pSrcBm++;
				b = *pSrcBm;
				fr = x1_t * y2_t;
				rr += (int)(r * fr);
				gg += (int)(g * fr);
				bb += (int)(b * fr);
			}
			if (x1_t < 1 && y1_t < 1 && iptx + 1 < srcWidth && ipty + 1 < srcHeight)
			{
				pSrcBm = &srcBm[idx0 + srcWidth * 3 + 3];
				r = *pSrcBm++;
				g = *pSrcBm++;
				b = *pSrcBm;
				fr = x2_t * y2_t;
				rr += (int)(r * fr);
				gg += (int)(g * fr);
				bb += (int)(b * fr);
			}

			if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_RGB) {
				*pOutBm++ = (BYTE)rr;
				*pOutBm++ = (BYTE)gg;
				*pOutBm++ = (BYTE)bb;
			} else if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_BGR) {
				*pOutBm++ = (BYTE)bb;
				*pOutBm++ = (BYTE)gg;
				*pOutBm++ = (BYTE)rr;
			}
		}
		else if (ptx == -9999)
		{
			*pOutBm++ = border_r;
			*pOutBm++ = border_g;
			*pOutBm++ = border_b;
		} 
		else if (ptx == -9999-1)
		{
			*pOutBm++ = blank_r;
			*pOutBm++ = blank_g;
			*pOutBm++ = blank_b;
		}
		else
		{
			*pOutBm++ = 0;
			*pOutBm++ = 0;
			*pOutBm++ = 0;
		}
	}

    return TRUE;
}


BOOL __declspec(dllexport) __stdcall SrcRGBMark0(int tbl[], int outWidth, int outHeight, BYTE srcBm[], RGBColor clMarkColor1, RGBColor clMarkColor2, DWORD dwRenderOpt)
{
    if (tbl == NULL) return FALSE;

	BYTE mark1_r = RGB_R(clMarkColor1);
	BYTE mark1_g = RGB_G(clMarkColor1);
	BYTE mark1_b = RGB_B(clMarkColor1);
	BYTE mark2_r = RGB_R(clMarkColor2);
	BYTE mark2_g = RGB_G(clMarkColor2);
	BYTE mark2_b = RGB_B(clMarkColor2);
	BYTE rr, gg, bb;

	for (int i = 0; i < outHeight; i++)
	{
		for (int j = 0; j < outWidth; j++)
		{
			int k = tbl[i * outWidth + j];
			if (k >= 0)
			{
				BYTE *pSrcBm = &srcBm[k];
				if(i % 20 == 0 || i % 20 == 1){
					rr = mark1_r;
					gg = mark1_g;
					bb = mark1_b;
				} else {
					rr = mark2_r;
					gg = mark2_g;
					bb = mark2_b;
				}

				if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_RGB) {
					*pSrcBm++ = (BYTE)rr;
					*pSrcBm++ = (BYTE)gg;
					*pSrcBm = (BYTE)bb;
				} else if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_BGR) {
					*pSrcBm++ = (BYTE)bb;
					*pSrcBm++ = (BYTE)gg;
					*pSrcBm = (BYTE)rr;
				}
			}
		}
	}

    return TRUE;
}


BOOL __declspec(dllexport) __stdcall SrcRGBMarkF0(CoordF tbl[], int outWidth, int outHeight, BYTE srcBm[], int srcWidth, int srcHeight, RGBColor clMarkColor1, RGBColor clMarkColor2, DWORD dwRenderOpt)
{
    if (tbl == NULL) return FALSE;

	BYTE mark1_r = RGB_R(clMarkColor1);
	BYTE mark1_g = RGB_G(clMarkColor1);
	BYTE mark1_b = RGB_B(clMarkColor1);
	BYTE mark2_r = RGB_R(clMarkColor2);
	BYTE mark2_g = RGB_G(clMarkColor2);
	BYTE mark2_b = RGB_B(clMarkColor2);
	BYTE rr, gg, bb;

	for (int i = 0; i < outHeight; i++)
	{
		for (int j = 0; j < outWidth; j++)
		{
			float ptx = tbl[i].x;
			float pty = tbl[i].y;

			if (ptx > -9999)
			{
				int idx0 = ((int)ptx * srcWidth + (int)ptx) * 3;
				BYTE *pSrcBm = &srcBm[idx0];
				if(i % 20 == 0 || i % 20 == 1){
					rr = mark1_r;
					gg = mark1_g;
					bb = mark1_b;
				} else {
					rr = mark2_r;
					gg = mark2_g;
					bb = mark2_b;
				}

				if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_RGB) {
					*pSrcBm++ = (BYTE)rr;
					*pSrcBm++ = (BYTE)gg;
					*pSrcBm = (BYTE)bb;
				} else if ((dwRenderOpt & RENDER_OPT_MASK) == RENDER_OPT_BGR) {
					*pSrcBm++ = (BYTE)bb;
					*pSrcBm++ = (BYTE)gg;
					*pSrcBm = (BYTE)rr;
				}
			}
		}
	}

    return TRUE;
}

