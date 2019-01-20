// TransBitmap.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include "TransYUV422BitmapByTable.h"
#include "GenTransTable.h"
#include <stdlib.h>


#define BLANK_Y (0)
#define BLANK_U (128)
#define BLANK_V (128)

#define CLIP(t) (((t)>=255)?255:(((t)<=0)?0:(t)))

/*
static void func1_2(void *param, int desX, int desY, int desWidth, int desHeight, float srcX, float srcY, int srcWidth, int srcHeight, int srcWidthStride, int pixelBytes)
{
	int *tbl = (int *)param;
	int *transTblY = (int *)tbl[0];
	int *transTblU = (int *)tbl[1];
	//int *transTblV = (int *)tbl[2];
	int srcStrideY = (int)tbl[3];
	int srcStrideU = (int)tbl[4];
	//int srcStrideV = (int)tbl[5];

	int addr = desY * desWidth + desX;
    if (srcX >= 0 && srcX < srcWidth && srcY >= 0 && srcY < srcHeight)
    {
        transTblY[addr] = ((int)srcY * srcStrideY + (int)srcX);
        transTblU[addr] = ((int)srcY * srcStrideU + ((int)srcX >> 1));
		//transTblV[addr] = ((int)srcY * srcStrideV + ((int)srcX >> 1));
    }
    else
    {
        transTblY[addr] = -2;
        transTblU[addr] = -2;
        //transTblV[addr] = -2;
    }
}
*/

BOOL __declspec(dllexport) __stdcall MakeYUV422TransTable0(int transYTbl[], int transUTbl[], int transVTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, 
										  int srcStrideY, int srcStrideU, int srcStrideV,
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam)
{
	/*
	long tbl[6];
	tbl[0] = (long)transYTbl;
	tbl[1] = (long)transUTbl;
	tbl[2] = (long)transVTbl;
	tbl[3] = (long)srcStrideY;
	tbl[4] = (long)srcStrideU;
	tbl[5] = (long)srcStrideV;

	return MakeTransTable0(func1_2, (void *)tbl, outWidth, outHeight, 
                                          centerX, centerY, innerRadius, outerRadius, 
                                          srcWidth, srcHeight, srcWidth, 1,
                                          startAngle, sweepAngle,
										  sx, sy, width, height,
										  distParam);
	*/

	BOOL r = MakeTransTable(transYTbl, outWidth, outHeight, 
							centerX, centerY, innerRadius, outerRadius, 
							srcWidth, srcHeight, srcStrideY, 1, 1,
							startAngle, sweepAngle,
							sx, sy, width, height,
							distParam);
	if(!r) return FALSE;

	r = MakeTransTable(transUTbl, outWidth, outHeight, 
							centerX, centerY, innerRadius, outerRadius, 
							srcWidth, srcHeight, srcStrideU, 0.5, 1,
							startAngle, sweepAngle,
							sx, sy, width, height,
							distParam);
	if(!r) return FALSE;

	r = MakeTransTable(transVTbl, outWidth, outHeight, 
							centerX, centerY, innerRadius, outerRadius, 
							srcWidth, srcHeight, srcStrideV, 0.5, 1,
							startAngle, sweepAngle,
							sx, sy, width, height,
							distParam);

	return r;
}


BOOL __declspec(dllexport) __stdcall MakeYUV422Bitmap0(int tblY[], int tblU[], int tblV[], 
													   BYTE outBmY[], int outBmYStride,
													   BYTE outBmU[], int outBmUStride, 
													   BYTE outBmV[], int outBmVStride,
													   int outWidth, int outHeight, 
													   BYTE srcBmY[], BYTE srcBmU[], BYTE srcBmV[],
													   YUVColor clBorderColor, YUVColor clBlankColor)
{
    if (tblY == NULL || tblU == NULL || tblV == NULL) return FALSE;

	BYTE blank_y = YUV_Y(clBlankColor);
	BYTE blank_u = YUV_U(clBlankColor);
	BYTE blank_v = YUV_V(clBlankColor);
	BYTE border_y = YUV_Y(clBorderColor);
	BYTE border_u = YUV_U(clBorderColor);
	BYTE border_v = YUV_V(clBorderColor);

	//BYTE *pOutBm = outBm;
	int len = outWidth * outHeight;

	//int nYUY2Size = outWidth * outHeight / 2;

	unsigned char *py, *pu, *pv;
	const unsigned char *py_0, *pu_0, *pv_0;
	unsigned char y1, y2, u, v;

	py = outBmY;
	pu = outBmU;
	pv = outBmV;
	py_0 = srcBmY;
	pu_0 = srcBmU;
	pv_0 = srcBmV;

	int y_idx1 = 0;
	int uv_idx = 0;

	int t_y_idx = 0;
	int t_u_idx = 0;
	int t_v_idx = 0;

	for(int i = 0; i < outHeight; i++) {
		for(int j = 0; j < outWidth/2; j++) {
			int idx1 = tblY[y_idx1];
			int idx2 = tblY[y_idx1 + 1];
			int u_idx1 = tblU[y_idx1];
			int u_idx2 = tblU[y_idx1 + 1];
			int v_idx1 = tblV[y_idx1];
			int v_idx2 = tblV[y_idx1 + 1];

#if 0 /// original
			int u_sum = 0;
			int v_sum = 0;
			int div_cnt = 0;

			if(idx1 >= 0){
				y1 = py_0[idx1];
				u_sum += pu_0[u_idx1];
				v_sum += pv_0[v_idx1];
				div_cnt ++;
			} else {
				y1 = BLANK_Y;
			}
			if(idx2 >= 0){
				y2 = py_0[idx2];
				u_sum += pu_0[u_idx2];
				v_sum += pv_0[v_idx2];
				div_cnt ++;
			} else {
				y2 = BLANK_Y;
			}

			py[t_y_idx] = y1;
			py[t_y_idx + 1] = y2;

			if(div_cnt > 0){
				int vv = u_sum / div_cnt;
				pu[t_u_idx] = CLIP(vv);
				vv = v_sum / div_cnt;
				pv[t_v_idx] = CLIP(vv);
			} else {
				pu[t_u_idx] = BLANK_U;
				pv[t_v_idx] = BLANK_V;
			}
#else
			// 최적화를 위해 나눗셈 없앰.
			if(idx1 >= 0 && idx2 >= 0){
				y1 = py_0[idx1];
				y2 = py_0[idx2];
				u = (pu_0[u_idx1] >> 1) + (pu_0[u_idx2] >> 1);
				v = (pv_0[v_idx1] >> 1) + (pv_0[v_idx2] >> 1);
			} else if(idx1 >= 0){
				y1 = py_0[idx1];
				y2 = BLANK_Y;
				u = pu_0[u_idx1];
				v = pv_0[v_idx1];
			} else if(idx2 >= 0){
				y1 = BLANK_Y;
				y2 = py_0[idx2];
				u = pu_0[u_idx2];
				v = pv_0[v_idx2];
			} else {
				y1 = BLANK_Y;
				y2 = BLANK_Y;
				u = BLANK_U;
				v = BLANK_V;
			}

			py[t_y_idx] = y1;
			py[t_y_idx + 1] = y2;
			pu[t_u_idx] = u;
			pv[t_v_idx] = v;
#endif

			y_idx1 += 2;
			uv_idx ++;

			t_y_idx += 2;
			t_u_idx ++;
			t_v_idx ++;
		}

		//t_y_idx ++;
		//t_u_idx ++;
		//t_v_idx ++;
	}

    return TRUE;
}


BOOL __declspec(dllexport) __stdcall MakeYUV422BitmapF0(CoordF tbl[], BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int outWidth, int outHeight, BYTE srcBmY[], BYTE srcBmU[], BYTE srcBmV[], int srcWidth, int srcHeight, YUVColor clBorderColor, YUVColor clBlankColor)
{
    if (tbl == NULL) return FALSE;

	BYTE blank_y = YUV_Y(clBlankColor);
	BYTE blank_u = YUV_U(clBlankColor);
	BYTE blank_v = YUV_V(clBlankColor);
	BYTE border_y = YUV_Y(clBorderColor);
	BYTE border_u = YUV_U(clBorderColor);
	BYTE border_v = YUV_V(clBorderColor);

	int len = outWidth * outHeight;
	BYTE *pOutBm = outBmY;
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
			pSrcBm = srcBmY;
			int r = *pSrcBm++;
			int g = *pSrcBm++;
			int b = *pSrcBm;

			float fr = x1_t * y1_t;
			int rr = (int)(r * fr);
			int gg = (int)(g * fr);
			int bb = (int)(b * fr);
			if (x1_t < 1 && iptx + 1 < srcWidth)
			{
				pSrcBm = srcBmY;
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
				pSrcBm = srcBmY;
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
				pSrcBm = srcBmY;
				r = *pSrcBm++;
				g = *pSrcBm++;
				b = *pSrcBm;
				fr = x2_t * y2_t;
				rr += (int)(r * fr);
				gg += (int)(g * fr);
				bb += (int)(b * fr);
			}

			*pOutBm++ = (BYTE)rr;
			*pOutBm++ = (BYTE)gg;
			*pOutBm++ = (BYTE)bb;
		}
		else if (ptx == -9999)
		{
			*pOutBm++ = border_y;
			*pOutBm++ = border_u;
			*pOutBm++ = border_v;
		} 
		else if (ptx == -9999-1)
		{
			*pOutBm++ = blank_y;
			*pOutBm++ = blank_u;
			*pOutBm++ = blank_v;
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


BOOL __declspec(dllexport) __stdcall SrcYUV422Mark0(int tblY[], int tblU[], int tblV[], int outWidth, int outHeight, BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], YUVColor clMarkColor1, YUVColor clMarkColor2)
{
    if (tblY == NULL || tblU == NULL || tblV == NULL) return FALSE;

	BYTE mark1_y = YUV_Y(clMarkColor1);
	BYTE mark1_u = YUV_U(clMarkColor1);
	BYTE mark1_v = YUV_V(clMarkColor1);
	BYTE mark2_y = YUV_Y(clMarkColor2);
	BYTE mark2_u = YUV_U(clMarkColor2);
	BYTE mark2_v = YUV_V(clMarkColor2);
	BYTE rr, gg, bb;

	for (int i = 0; i < outHeight; i++)
	{
		for (int j = 0; j < outWidth; j++)
		{
			int k = tblY[i * outWidth + j];
			if (k >= 0)
			{
				BYTE *pSrcBm = &outBmY[k];
				if(i % 20 == 0 || i % 20 == 1){
					rr = mark1_y;
					gg = mark1_u;
					bb = mark1_v;
				} else {
					rr = mark2_y;
					gg = mark2_u;
					bb = mark2_v;
				}

				*pSrcBm++ = (BYTE)rr;
				*pSrcBm++ = (BYTE)gg;
				*pSrcBm = (BYTE)bb;
			}
		}
	}

    return TRUE;
}


BOOL __declspec(dllexport) __stdcall SrcYUV422MarkF0(CoordF tbl[], int outWidth, int outHeight, BYTE outBmY[], BYTE outBmU[], BYTE outBmV[], int srcWidth, int srcHeight, YUVColor clMarkColor1, YUVColor clMarkColor2)
{
    if (tbl == NULL) return FALSE;

	BYTE mark1_y = YUV_Y(clMarkColor1);
	BYTE mark1_u = YUV_U(clMarkColor1);
	BYTE mark1_v = YUV_V(clMarkColor1);
	BYTE mark2_y = YUV_Y(clMarkColor2);
	BYTE mark2_u = YUV_U(clMarkColor2);
	BYTE mark2_v = YUV_V(clMarkColor2);
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
				BYTE *pSrcBm = &outBmY[idx0];
				if(i % 20 == 0 || i % 20 == 1){
					rr = mark1_y;
					gg = mark1_u;
					bb = mark1_v;
				} else {
					rr = mark2_y;
					gg = mark2_u;
					bb = mark2_v;
				}

				*pSrcBm++ = (BYTE)rr;
				*pSrcBm++ = (BYTE)gg;
				*pSrcBm = (BYTE)bb;
			}
		}
	}

    return TRUE;
}
