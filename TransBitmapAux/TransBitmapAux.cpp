// TransBitmap.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "TransBitmapTable.h"
#include "TransBitmapAux.h"
#include <stdlib.h>


#define MAX_TRANS_TABLE_SIZE (8)

typedef struct __tagTransTable {
	BYTE *ptr;
	int size;
} TransTable;

typedef struct __tagTransBitmapInfo {
	int tableIndex;
	int srcStride;
} TransBitmapInfo;

typedef struct __tagTransBitmapHandleHeader {
	int outWidth, outHeight;
    int centerX, centerY;
	int innerRadius, outerRadius;
    float startAngle, sweepAngle;
    int srcWidth, srcHeight;
    DWORD dwTransOpt;
	TransTable transTable[MAX_TRANS_TABLE_SIZE];
	int transTableCnt;
	TransBitmapInfo transBitmapInfo[MAX_TRANS_TABLE_SIZE];
	int transBitmapCnt;
} TransBitmapHandleHeader;


#if 0
static BOOL /*__declspec(dllexport) __stdcall*/ MakeTransTable(int transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, 
                                          double startAngle, double sweepAngle,
										  Distortion *distParam)
{
	return MakeRGBTransTable0(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle,
							0, 0, outWidth, outHeight, distParam);
}


static BOOL /*__declspec(dllexport) __stdcall*/ MakeTransTable2(int transTbl[], int transTbl2[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, 
                                          double startAngle, double sweepAngle,
										  Distortion *distParam, int yuv420_422)
{
	if(yuv420_422 == 0){
		return MakeYUV420TransTable0(transTbl, transTbl2, transTbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, 
							srcWidth, srcWidth/2, srcWidth/2,
							startAngle, sweepAngle,
							0, 0, outWidth, outHeight, distParam);
	} else {
		return MakeYUV422TransTable0(transTbl, transTbl2, transTbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, 
							srcWidth, srcWidth/2, srcWidth/2,
							startAngle, sweepAngle,
							0, 0, outWidth, outHeight, distParam);
	}
}


static BOOL /*__declspec(dllexport) __stdcall*/ MakeTransTableF(CoordF transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, 
                                          double startAngle, double sweepAngle,
										  Distortion *distParam)
{
	return MakeTransTableF0(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle,
							0, 0, outWidth, outHeight, distParam);
}


static BOOL /*__declspec(dllexport) __stdcall*/ MakeBitmap(int tbl[], BYTE outBm[], int outWidth, int outHeight, BYTE srcBm[], DWORD dwRenderOpt)
{
	return MakeRGBBitmap0(tbl, outBm, outWidth, outHeight, srcBm, MAKE_RGB(0x00,0x00,0x00), MAKE_RGB(0x00,0x00,0x00), dwRenderOpt);
}


static BOOL /*__declspec(dllexport) __stdcall*/ MakeBitmapF(CoordF tbl[], BYTE outBm[], int outWidth, int outHeight, BYTE srcBm[], int srcWidth, int srcHeight, DWORD dwRenderOpt)
{
	return MakeRGBBitmapF0(tbl, outBm, outWidth, outHeight, srcBm, srcWidth, srcHeight, MAKE_RGB(0x00,0x00,0x00), MAKE_RGB(0x00,0x00,0x00), dwRenderOpt);
}


static BOOL /*__declspec(dllexport) __stdcall*/ SrcRGBMark(int tbl[], int outWidth, int outHeight, BYTE srcBm[], DWORD dwRenderOpt)
{
	return SrcRGBMark0(tbl, outWidth, outHeight, srcBm, MAKE_RGB(0xff, 0xff, 0x00), MAKE_RGB(0xff, 0x00, 0xff), dwRenderOpt);
}


static BOOL /*__declspec(dllexport) __stdcall*/ SrcYUV420Mark(int tbl[], int outWidth, int outHeight, BYTE srcYBm[], BYTE srcUBm[], BYTE srcVBm[], DWORD dwRenderOpt)
{
	//return SrcYUVMark0(tbl, outWidth, outHeight, srcYBm, srcUBm, srcVBm, MAKE_RGB(0xff, 0xff, 0x00), MAKE_RGB(0xff, 0x00, 0xff), dwRenderOpt);
	return FALSE;
}


static BOOL /*__declspec(dllexport) __stdcall*/ SrcYUV422Mark(int tbl[], int outWidth, int outHeight, BYTE srcYBm[], BYTE srcUBm[], BYTE srcVBm[], DWORD dwRenderOpt)
{
	//return SrcYUVMark0(tbl, outWidth, outHeight, srcYBm, srcUBm, srcVBm, MAKE_RGB(0xff, 0xff, 0x00), MAKE_RGB(0xff, 0x00, 0xff), dwRenderOpt);
	return FALSE;
}


static BOOL /*__declspec(dllexport) __stdcall*/ SrcRGBMarkF(CoordF tbl[], int outWidth, int outHeight, BYTE srcBm[], int srcWidth, int srcHeight, DWORD dwRenderOpt)
{
	return SrcRGBMarkF0(tbl, outWidth, outHeight, srcBm, srcWidth, srcHeight, MAKE_RGB(0xff, 0xff, 0x00), MAKE_RGB(0xff, 0x00, 0xff), dwRenderOpt);
}
#endif


static BOOL /*__declspec(dllexport) __stdcall*/ MakeTransTableMultiRGB(int transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
                                          int srcWidth, int srcHeight,
										  int rowCount, int colCount, int splitWidth,
										  Distortion *distParam)
{
	if(rowCount <= 0 || colCount <= 0) return FALSE;

	if(splitWidth < 0) splitWidth = 0;

	int oneW = (outWidth - splitWidth * (colCount - 1)) / colCount;
	int oneH = (outHeight - splitWidth * (rowCount - 1)) / rowCount;
	int posX = oneW + splitWidth;
	int posY = oneH + splitWidth;

	if(rowCount == 1 && colCount == 1){
		MakeRGBTransTable0(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle, 0, 0, outWidth, outHeight, distParam);
	} else {
		for(int i = 0; i < outWidth * outHeight; i++) transTbl[i] = -1;

		float angle = (float)360 / (rowCount * colCount);
		float curangle = 0;
		for(int i = 0; i < rowCount; i++){
			for(int j = 0; j < colCount; j++){
				MakeRGBTransTable0(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle + curangle, angle,
								j * posX, i * posY,	oneW, oneH, 
								distParam);
				curangle += angle;
			}
		}
	}

	return TRUE;
}


static BOOL /*__declspec(dllexport) __stdcall*/ MakeTransTableMultiYUV422(int transTbl[], int transTbl2[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
                                          int srcWidth, int srcHeight,
										  int srcStrideY, int srcStrideUV,
										  int rowCount, int colCount, int splitWidth,
										  Distortion *distParam)
{
	if(rowCount <= 0 || colCount <= 0) return FALSE;

	if(splitWidth < 0) splitWidth = 0;

	int oneW = (outWidth - splitWidth * (colCount - 1)) / colCount;
	int oneH = (outHeight - splitWidth * (rowCount - 1)) / rowCount;
	int posX = oneW + splitWidth;
	int posY = oneH + splitWidth;

	if(rowCount == 1 && colCount == 1){
		//MakeTransTable2(transTbl, transTbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle, distParam);
		MakeYUV422TransTable0(transTbl, transTbl2, transTbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, 
						srcWidth, srcHeight, srcStrideY, srcStrideUV, srcStrideUV,
						startAngle, sweepAngle,
						0, 0, oneW, oneH, 
						distParam);
	} else {
		for(int i = 0; i < outWidth * outHeight; i++){
			transTbl[i] = transTbl2[i] = -1;
		}

		float angle = (float)360 / (rowCount * colCount);
		float curangle = 0;
		for(int i = 0; i < rowCount; i++){
			for(int j = 0; j < colCount; j++){
				MakeYUV422TransTable0(transTbl, transTbl2, transTbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, 
								srcWidth, srcHeight, srcStrideY, srcStrideUV, srcStrideUV,
								startAngle + curangle, angle,
								j * posX, i * posY,	oneW, oneH, 
								distParam);
				curangle += angle;
			}
		}
	}

	return TRUE;
}


static BOOL /*__declspec(dllexport) __stdcall*/ MakeTransTableMultiYUV420(int transTbl[], int transTbl2[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
                                          int srcWidth, int srcHeight,
										  int srcStrideY, int srcStrideUV,
										  int rowCount, int colCount, int splitWidth,
										  Distortion *distParam)
{
	if(rowCount <= 0 || colCount <= 0) return FALSE;

	if(splitWidth < 0) splitWidth = 0;

	int oneW = (outWidth - splitWidth * (colCount - 1)) / colCount;
	int oneH = (outHeight - splitWidth * (rowCount - 1)) / rowCount;
	int posX = oneW + splitWidth;
	int posY = oneH + splitWidth;

	if(rowCount == 1 && colCount == 1){
		//MakeTransTable(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle, distParam);
		MakeYUV420TransTable0(transTbl, transTbl2, transTbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, 
						srcWidth, srcHeight, srcStrideY, srcStrideUV, srcStrideUV,
						startAngle, sweepAngle,
						0, 0, oneW, oneH, 
						distParam);
	} else {
		for(int i = 0; i < outWidth * outHeight; i++){
			transTbl[i] = transTbl2[i] = -1;
		}

		float angle = (float)360 / (rowCount * colCount);
		float curangle = 0;
		for(int i = 0; i < rowCount; i++){
			for(int j = 0; j < colCount; j++){
				MakeYUV420TransTable0(transTbl, transTbl2, transTbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, 
								srcWidth, srcHeight, srcStrideY, srcStrideUV, srcStrideUV,
								startAngle + curangle, angle,
								j * posX, i * posY,	oneW, oneH, 
								distParam);
				curangle += angle;
			}
		}
	}

	return TRUE;
}


static BOOL /*__declspec(dllexport) __stdcall*/ MakeTransTableMultiF(CoordF transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
                                          int srcWidth, int srcHeight,
										  int rowCount, int colCount, int splitWidth,
										  Distortion *distParam)
{
	if(rowCount <= 0 || colCount <= 0) return FALSE;

	if(splitWidth < 0) splitWidth = 0;

	int oneW = (outWidth - splitWidth * (colCount - 1)) / colCount;
	int oneH = (outHeight - splitWidth * (rowCount - 1)) / rowCount;
	int posX = oneW + splitWidth;
	int posY = oneH + splitWidth;

	if(rowCount == 1 && colCount == 1){
		MakeTransTableF(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle, 0, 0, outWidth, outHeight, distParam);
	} else {
		for(int i = 0; i < outWidth * outHeight; i++) transTbl[i].x = transTbl[i].y = -9999;

		float angle = (float)360 / (rowCount * colCount);
		float curangle = 0;
		for(int i = 0; i < rowCount; i++){
			for(int j = 0; j < colCount; j++){
				MakeTransTableF(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle + curangle, angle,
								j * posX, i * posY,	oneW, oneH, 
								distParam);
				curangle += angle;
			}
		}
	}

	return TRUE;
}


BOOL __declspec(dllexport) __stdcall TransBitmap0(BYTE *outBm[], int outBmStride[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          double startAngle, double sweepAngle,
                                          BYTE *srcBm[], int srcWidth, int srcHeight,
										  int srcStride[],
										  DWORD dwTransRenderOpt, 
										  Distortion *distParam)
{
	if(dwTransRenderOpt & TRANS_OPT_RGB24){
		if(dwTransRenderOpt & TRANS_OPT_SPEED){
			int *tbl = new int[outWidth * outHeight];
			MakeRGBTransTable0(tbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle, 
								0, 0, outWidth, outHeight, distParam);
			MakeRGBBitmap0(tbl, outBm[0], outWidth, outHeight, srcBm[0], MAKE_RGB(0x00,0x00,0x00), MAKE_RGB(0x00,0x00,0x00), dwTransRenderOpt & RENDER_OPT_MASK);
			delete [] tbl;
		} else if(dwTransRenderOpt & TRANS_OPT_QUALITY){
			CoordF *tbl = new CoordF[outWidth * outHeight];
			MakeTransTableF(tbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle,
								0, 0, outWidth, outHeight, distParam);
			MakeRGBBitmapF0(tbl, outBm[0], outWidth, outHeight, srcBm[0], srcWidth, srcHeight, MAKE_RGB(0x00,0x00,0x00), MAKE_RGB(0x00,0x00,0x00), dwTransRenderOpt & RENDER_OPT_MASK);
			delete [] tbl;
		}
	} else if(dwTransRenderOpt & TRANS_OPT_YUV422){
		if(dwTransRenderOpt & TRANS_OPT_SPEED){
			int *tbl = new int[outWidth * outHeight];
			int *tbl2 = new int[outWidth/2 * outHeight];
			MakeYUV422TransTable0(tbl, tbl2, tbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, 
								srcStride[0], srcStride[1], srcStride[1],
								startAngle, sweepAngle, 
								0, 0, outWidth, outHeight,
								distParam);
			if(outBmStride != NULL){
				MakeYUV422Bitmap0(tbl, tbl2, tbl2, 
					outBm[0], outBmStride[0],
					outBm[1], outBmStride[1],
					outBm[2], outBmStride[2],
					outWidth, outHeight, srcBm[0], srcBm[1], srcBm[2], BLANK_YUV, BLANK_YUV);
			} else {
				MakeYUV422Bitmap0(tbl, tbl2, tbl2, 
					outBm[0], outWidth,
					outBm[1], outWidth / 2,
					outBm[2], outWidth / 2,
					outWidth, outHeight, srcBm[0], srcBm[1], srcBm[2], BLANK_YUV, BLANK_YUV);
			}
			delete [] tbl2;
			delete [] tbl;
		} else if(dwTransRenderOpt & TRANS_OPT_QUALITY){
			CoordF *tbl = new CoordF[outWidth * outHeight];
			MakeTransTableF(tbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle,
							0, 0, outWidth, outHeight, distParam);
			MakeYUV422BitmapF0(tbl, outBm[0], outBm[1], outBm[2], outWidth, outHeight, srcBm[0], srcBm[1], srcBm[2], srcWidth, srcHeight, BLANK_YUV, BLANK_YUV);
			delete [] tbl;
		}
	} else if(dwTransRenderOpt & TRANS_OPT_YUV420){
		if(dwTransRenderOpt & TRANS_OPT_SPEED){
			int *tbl = new int[outWidth * outHeight];
			int *tbl2 = new int[outWidth/2 * outHeight/2];
			MakeYUV420TransTable0(tbl, tbl2, tbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, 
								srcStride[0], srcStride[1], srcStride[1],
								startAngle, sweepAngle, 
								0, 0, outWidth, outHeight,
								distParam);
			if(outBmStride != NULL){
				MakeYUV420Bitmap0(tbl, tbl2, tbl2, 
					outBm[0], outBmStride[0],
					outBm[1], outBmStride[1],
					outBm[2], outBmStride[2],
					outWidth, outHeight, srcBm[0], srcBm[1], srcBm[2], BLANK_YUV, BLANK_YUV);
			} else {
				MakeYUV420Bitmap0(tbl, tbl2, tbl2, 
					outBm[0], outWidth,
					outBm[1], outWidth / 2,
					outBm[2], outWidth / 2,
					outWidth, outHeight, srcBm[0], srcBm[1], srcBm[2], BLANK_YUV, BLANK_YUV);
			}
			delete [] tbl2;
			delete [] tbl;
		} else if(dwTransRenderOpt & TRANS_OPT_QUALITY){
			CoordF *tbl = new CoordF[outWidth * outHeight];
			MakeTransTableF(tbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle, 0, 0, outWidth, outHeight, distParam);
			MakeYUV420BitmapF0(tbl, outBm[0], outBm[1], outBm[2], outWidth, outHeight, srcBm[0], srcBm[1], srcBm[2], srcWidth, srcHeight, BLANK_YUV, BLANK_YUV);
			delete [] tbl;
		}
	} else if(dwTransRenderOpt & TRANS_OPT_YUV444){
		if(dwTransRenderOpt & TRANS_OPT_SPEED){
			int *tbl = new int[outWidth * outHeight];
			//MakeYUV444TransTable0(tbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, 
			//					srcStride[0],
			//					startAngle, sweepAngle, 
			//					0, 0, outWidth, outHeight,
			//					distParam);
			//MakeYUV444Bitmap0(tbl, NULL, NULL, outBm[0], outBm[1], outBm[2], outWidth, outHeight, srcBm[0], srcBm[1], srcBm[2], BLANK_YUV, BLANK_YUV);
			delete [] tbl;
		} else if(dwTransRenderOpt & TRANS_OPT_QUALITY){
			CoordF *tbl = new CoordF[outWidth * outHeight];
			MakeTransTableF(tbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle, 0, 0, outWidth, outHeight, distParam);
			//MakeYUV444BitmapF0(tbl, outBm[0], outBm[1], outBm[2], outWidth, outHeight, srcBm[0], srcBm[1], srcBm[2], srcWidth, srcHeight, BLANK_YUV, BLANK_YUV);
			delete [] tbl;
		}
	}

	return TRUE;
}

BOOL __declspec(dllexport) __stdcall TransBitmap(BYTE outBm[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          double startAngle, double sweepAngle,
                                          BYTE srcBm[], int srcWidth, int srcHeight,
										  DWORD dwTransRenderOpt, 
										  Distortion *distParam)
{
	BYTE *outBm0[3];
	int outBmStride0[3];
	BYTE *srcBm0[3];
	int stride[3];

	outBm0[0] = outBm;
	outBmStride0[0] = outWidth;
	srcBm0[0] = srcBm;
	stride[0] = srcWidth;
	if(dwTransRenderOpt & TRANS_OPT_YUV422){
		outBm0[1] = outBm + outWidth * outHeight;
		outBmStride0[1] = outWidth / 2;
		outBm0[2] = outBm0[1] + outWidth * outHeight / 2;
		outBmStride0[2] = outWidth / 2;
		srcBm0[1] = srcBm + srcWidth * srcHeight;
		stride[1] = srcWidth / 2;
		srcBm0[2] = srcBm0[1] + srcWidth * srcHeight / 2;
		stride[2] = srcWidth / 2;
	} else if(dwTransRenderOpt & TRANS_OPT_YUV420){
		outBm0[1] = outBm + outWidth * outHeight;
		outBmStride0[1] = outWidth / 2;
		outBm0[2] = outBm0[1] + outWidth * outHeight / 4;
		outBmStride0[2] = outWidth / 2;
		srcBm0[1] = srcBm + srcWidth * srcHeight;
		stride[1] = srcWidth / 2;
		srcBm0[2] = srcBm0[1] + srcWidth * srcHeight / 4;
		stride[2] = srcWidth / 2;
	}

	return TransBitmap0(outBm0, outBmStride0, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle, 
						srcBm0, srcWidth, srcHeight, stride,
						dwTransRenderOpt, distParam);
}


int __declspec(dllexport) __stdcall GetRequireHandleSize(int outWidth, int outHeight, DWORD dwTransOpt)
{
	int sz = sizeof(TransBitmapHandleHeader);

	if(dwTransOpt & TRANS_OPT_SPEED){
		if(dwTransOpt & TRANS_OPT_RGB24){
			sz += outWidth * outHeight * sizeof(int);
		} else if(dwTransOpt & TRANS_OPT_YUV422){
			sz += outWidth * outHeight * sizeof(int) + outWidth * outHeight * sizeof(int);
		} else if(dwTransOpt & TRANS_OPT_YUV420){
			sz += outWidth * outHeight * sizeof(int) + outWidth * outHeight * sizeof(int);
		}
	} else if(dwTransOpt & TRANS_OPT_QUALITY){
		sz += outWidth * outHeight * sizeof(CoordF);
	}

	return sz;
}


BOOL __declspec(dllexport) __stdcall InitTransBitmapHandle(void *handlePtr, int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
                                          int srcWidth, int srcHeight, int srcStride[], int srcStrideCnt,
										  int rowCount, int colCount, int splitWidth,
										  DWORD dwTransOpt,
										  Distortion *distParam)
{
	TransBitmapHandleHeader *hdr = (TransBitmapHandleHeader *)handlePtr;

	hdr->outWidth  = outWidth;
	hdr->outHeight  = outHeight;
	hdr->centerX  = centerX;
	hdr->centerY  = centerY;
	hdr->innerRadius  = innerRadius;
	hdr->outerRadius  = outerRadius;
	hdr->startAngle  = startAngle;
	hdr->sweepAngle  = sweepAngle;
	hdr->srcWidth  = srcWidth;
	hdr->srcHeight  = srcHeight;
	hdr->dwTransOpt = dwTransOpt;

	if(rowCount <= 0 || colCount <= 0) return FALSE;

	if(splitWidth < 0) splitWidth = 0;

	if(dwTransOpt & TRANS_OPT_SPEED){
		if((dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_RGB24){
			hdr->transTableCnt = 1;
			hdr->transTable[0].ptr = (BYTE *)handlePtr + sizeof(TransBitmapHandleHeader);
			hdr->transTable[0].size = outWidth * outHeight * sizeof(int);
			hdr->transBitmapCnt = 1;
			hdr->transBitmapInfo[0].tableIndex = 0;
			if(srcStride != NULL){
				hdr->transBitmapInfo[0].srcStride = srcStride[0];
			} else {
				hdr->transBitmapInfo[0].srcStride = srcWidth;
			}
			MakeTransTableMultiRGB((int *)hdr->transTable[0].ptr, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle, srcWidth, srcHeight, 
								rowCount, colCount, splitWidth,
								distParam);
		} else if((dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV422){
			hdr->transTableCnt = 2;
			hdr->transTable[0].ptr = (BYTE *)handlePtr + sizeof(TransBitmapHandleHeader);
			hdr->transTable[0].size = outWidth * outHeight * sizeof(int);
			hdr->transTable[1].ptr = hdr->transTable[0].ptr + hdr->transTable[0].size;
			hdr->transTable[1].size = (outWidth / 2) * outHeight * sizeof(int);
			hdr->transBitmapCnt = 3;
			hdr->transBitmapInfo[0].tableIndex = 0;
			hdr->transBitmapInfo[1].tableIndex = 1;
			hdr->transBitmapInfo[2].tableIndex = 1;
			if(srcStride != NULL){
				hdr->transBitmapInfo[0].srcStride = srcStride[0];
				hdr->transBitmapInfo[1].srcStride = srcStride[1];
				hdr->transBitmapInfo[2].srcStride = srcStride[1];
			} else {
				hdr->transBitmapInfo[0].srcStride = srcWidth;
				hdr->transBitmapInfo[1].srcStride = srcWidth;
				hdr->transBitmapInfo[2].srcStride = srcWidth;
			}
			MakeTransTableMultiYUV422((int *)hdr->transTable[0].ptr, (int *)hdr->transTable[1].ptr, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle, 
								srcWidth, srcHeight, srcStride[0], srcStride[1],
								rowCount, colCount, splitWidth,
								distParam);
		} else if((dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV420){
			hdr->transTableCnt = 2;
			hdr->transTable[0].ptr = (BYTE *)handlePtr + sizeof(TransBitmapHandleHeader);
			hdr->transTable[0].size = outWidth * outHeight * sizeof(int);
			hdr->transTable[1].ptr = hdr->transTable[0].ptr + hdr->transTable[0].size;
			hdr->transTable[1].size = (outWidth / 2) * (outHeight / 2) * sizeof(int);
			hdr->transBitmapCnt = 3;
			hdr->transBitmapInfo[0].tableIndex = 0;
			hdr->transBitmapInfo[1].tableIndex = 1;
			hdr->transBitmapInfo[2].tableIndex = 1;
			if(srcStride != NULL){
				hdr->transBitmapInfo[0].srcStride = srcStride[0];
				hdr->transBitmapInfo[1].srcStride = srcStride[1];
				hdr->transBitmapInfo[2].srcStride = srcStride[1];
			} else {
				hdr->transBitmapInfo[0].srcStride = srcWidth;
				hdr->transBitmapInfo[1].srcStride = srcWidth;
				hdr->transBitmapInfo[2].srcStride = srcWidth;
			}
			MakeTransTableMultiYUV420((int *)hdr->transTable[0].ptr, (int *)hdr->transTable[1].ptr, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle, 
								srcWidth, srcHeight, srcStride[0], srcStride[1],
								rowCount, colCount, splitWidth,
								distParam);
		}
	} else if(dwTransOpt & TRANS_OPT_QUALITY){
		hdr->transTableCnt = 1;
		hdr->transTable[0].ptr = (BYTE *)handlePtr + sizeof(TransBitmapHandleHeader);
		hdr->transTable[0].size = outWidth * outHeight * sizeof(CoordF);
			hdr->transBitmapCnt = 3;
			hdr->transBitmapInfo[0].tableIndex = 0;
			hdr->transBitmapInfo[1].tableIndex = 1;
			hdr->transBitmapInfo[2].tableIndex = 1;
			if(srcStride != NULL){
				hdr->transBitmapInfo[0].srcStride = srcStride[0];
				hdr->transBitmapInfo[1].srcStride = srcStride[1];
				hdr->transBitmapInfo[2].srcStride = srcStride[1];
			} else {
				hdr->transBitmapInfo[0].srcStride = srcWidth;
				hdr->transBitmapInfo[1].srcStride = srcWidth;
				hdr->transBitmapInfo[2].srcStride = srcWidth;
			}
		MakeTransTableMultiF((CoordF *)hdr->transTable[0].ptr, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle, srcWidth, srcHeight, 
							rowCount, colCount, splitWidth,
							distParam);
	}

	return TRUE;
}


BOOL __declspec(dllexport) __stdcall GetSrcBitmapSize(void *handlePtr, SizeT *size)
{
	TransBitmapHandleHeader *hdr = (TransBitmapHandleHeader *)handlePtr;
	size->width = hdr->srcWidth;
	size->height = hdr->srcHeight;
	return TRUE;
}


BOOL __declspec(dllexport) __stdcall GetOutBitmapSize(void *handlePtr, SizeT *size)
{
	TransBitmapHandleHeader *hdr = (TransBitmapHandleHeader *)handlePtr;
	size->width = hdr->outWidth;
	size->height = hdr->outHeight;
	return TRUE;
}


BOOL __declspec(dllexport) __stdcall TransBitmapByHandle0(void *handlePtr, BYTE *outBm[], BYTE *srcBm[], RGBColor clBorderColor, RGBColor clBlankColor, DWORD dwRenderOpt)
{
	TransBitmapHandleHeader *hdr = (TransBitmapHandleHeader *)handlePtr;

	if(hdr->dwTransOpt & TRANS_OPT_SPEED){
		if((hdr->dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_RGB24){
			MakeRGBBitmap0((int *)hdr->transTable[0].ptr, outBm[0], hdr->outWidth, hdr->outHeight, srcBm[0], clBorderColor, clBlankColor, dwRenderOpt);
		} else if((hdr->dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV422){
			//int *tbl2 = tbl + hdr->outWidth * hdr->outHeight;
			//dwRenderOpt |= RENDER_OPT_YUV422;
			MakeYUV422Bitmap0((int *)hdr->transTable[0].ptr, (int *)hdr->transTable[1].ptr, (int *)hdr->transTable[1].ptr, 
								outBm[0], hdr->outWidth, 
								outBm[1], hdr->outWidth / 2, 
								outBm[2], hdr->outWidth / 2, 
								hdr->outWidth, hdr->outHeight, 
								srcBm[0], srcBm[1], srcBm[2], clBorderColor, clBlankColor);
		} else if((hdr->dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV420){
			//int *tbl2 = tbl + hdr->outWidth * hdr->outHeight;
			//dwRenderOpt |= RENDER_OPT_YUV420;
			MakeYUV420Bitmap0((int *)hdr->transTable[0].ptr, (int *)hdr->transTable[1].ptr, (int *)hdr->transTable[1].ptr, 
								outBm[0], hdr->outWidth, 
								outBm[1], hdr->outWidth / 2, 
								outBm[2], hdr->outWidth / 2, 
								hdr->outWidth, hdr->outHeight, 
								srcBm[0], srcBm[1], srcBm[2], clBorderColor, clBlankColor);
		}
	} else if(hdr->dwTransOpt & TRANS_OPT_QUALITY){
		//CoordF *tbl = (CoordF *)((BYTE *)handlePtr + sizeof(TransBitmapHandleHeader));
		MakeRGBBitmapF0((CoordF *)hdr->transTable[0].ptr, outBm[0], hdr->outWidth, hdr->outHeight, srcBm[0], hdr->srcWidth, hdr->srcHeight, clBorderColor, clBlankColor, dwRenderOpt);
	}

	return TRUE;
}


BOOL __declspec(dllexport) __stdcall TransBitmapByHandle(void *handlePtr, BYTE outBm[], BYTE srcBm[], DWORD dwRenderOpt)
{
	TransBitmapHandleHeader *hdr = (TransBitmapHandleHeader *)handlePtr;

	BYTE *outBm0[3];
	BYTE *srcBm0[3];
	outBm0[0] = outBm;
	srcBm0[0] = srcBm;
	if((hdr->dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV422){
		outBm0[1] = outBm + hdr->outWidth * hdr->outHeight;
		outBm0[2] = (BYTE *)outBm0[1] + hdr->outWidth * hdr->outHeight / 2;
		srcBm0[1] = srcBm + hdr->srcWidth * hdr->srcHeight;
		srcBm0[2] = (BYTE *)srcBm0[1] + hdr->srcWidth * hdr->srcHeight / 2;
	} else if((hdr->dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV420){
		outBm0[1] = outBm + hdr->outWidth * hdr->outHeight;
		outBm0[2] = (BYTE *)outBm0[1] + hdr->outWidth * hdr->outHeight / 4;
		srcBm0[1] = srcBm + hdr->srcWidth * hdr->srcHeight;
		srcBm0[2] = (BYTE *)srcBm0[1] + hdr->srcWidth * hdr->srcHeight / 4;
	}

	return TransBitmapByHandle0(handlePtr, outBm0, srcBm0, MAKE_RGB(0,0,0), MAKE_RGB(0,0,0), dwRenderOpt);
}


BOOL __declspec(dllexport) __stdcall SrcMarkBitmapByHandle0(void *handlePtr, BYTE *outBm[], BYTE *srcBm[], RGBColor clMarkColor1, RGBColor clMarkColor2, DWORD dwRenderOpt)
{
	TransBitmapHandleHeader *hdr = (TransBitmapHandleHeader *)handlePtr;

	if(hdr->dwTransOpt & TRANS_OPT_SPEED){
		//int *tbl = (int *)((BYTE *)handlePtr + sizeof(TransBitmapHandleHeader));
		if((hdr->dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_RGB24){
			SrcRGBMark0((int *)hdr->transTable[0].ptr, hdr->outWidth, hdr->outHeight, srcBm[0], clMarkColor1, clMarkColor2, dwRenderOpt);
		} else if((hdr->dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV422){
			//int *tbl2 = tbl + hdr->outWidth * hdr->outHeight;
			//dwRenderOpt |= RENDER_OPT_YUV422;
			//SrcYUVMark(tbl, hdr->outWidth, hdr->outHeight, srcBm[0], srcBm[1], srcBm[2], dwRenderOpt);
		} else if((hdr->dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV420){
			//int *tbl2 = tbl + hdr->outWidth * hdr->outHeight;
			//dwRenderOpt |= RENDER_OPT_YUV420;
			//SrcYUVMark(tbl, hdr->outWidth, hdr->outHeight, srcBm[0], srcBm[1], srcBm[2], dwRenderOpt);
		}
	} else if(hdr->dwTransOpt & TRANS_OPT_QUALITY){
		CoordF *tbl = (CoordF *)((BYTE *)handlePtr + sizeof(TransBitmapHandleHeader));
		SrcRGBMarkF0(tbl, hdr->outWidth, hdr->outHeight, srcBm[0], hdr->srcWidth, hdr->srcHeight, clMarkColor1, clMarkColor2, dwRenderOpt);
	}

	return TRUE;
}


BOOL __declspec(dllexport) __stdcall SrcMarkBitmapByHandle(void *handlePtr, BYTE outBm[], BYTE srcBm[], DWORD dwRenderOpt)
{
	BYTE *outBm0[3];
	outBm0[0] = outBm;
	BYTE *srcBm0[3];
	srcBm0[0] = srcBm;

	return SrcMarkBitmapByHandle0(handlePtr, outBm0, srcBm0, MAKE_RGB(0,0,0), MAKE_RGB(0,0,0), dwRenderOpt);
}


BOOL __declspec(dllexport) __stdcall MakeTransBitmap(BYTE *outBm[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
										  BYTE *srcBm[],
                                          int srcWidth, int srcHeight,
                                          int srcStride[], int srcStrideCnt,
										  DWORD dwTransOpt, DWORD dwRenderOpt,
										  Distortion *distParam)
{
	BOOL r = FALSE;

	if(dwTransOpt & TRANS_OPT_SPEED){
		if((dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_RGB24){
			int *transTbl = (int *)malloc(outWidth * outHeight * sizeof(int));
			MakeRGBTransTable0(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle,
							0, 0, outWidth, outHeight, distParam);
			r = MakeRGBBitmap0(transTbl, outBm[0], outWidth, outHeight, srcBm[0], 0, 0, dwRenderOpt);
			free(transTbl);
		} else if((dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV422){
			int size = GetRequireHandleSize(outWidth, outHeight, dwTransOpt);
			void *handle = malloc(size);
			InitTransBitmapHandle(handle, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle, srcWidth, srcHeight, srcStride, srcStrideCnt, 1, 1, 0, dwTransOpt, distParam);
			//r = TransBitmapByHandle(handle, outBm, srcBm, 0);
			free(handle);
		} else if((dwTransOpt & TRANS_OPT_MASK) == TRANS_OPT_YUV420){
			//int *transTbl = (int *)((BYTE *)handlePtr + sizeof(TransBitmapHandleHeader));
			//int *transTbl2 = transTbl + outWidth * outHeight;
			//MakeTransTableMultiYUV420(transTbl, transTbl2, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle, 
			//					srcWidth, srcHeight, srcStride[0], srcStride[1],
			//					rowCount, colCount, splitWidth,
			//					distParam);
		}
	} else if(dwTransOpt & TRANS_OPT_QUALITY){
		//CoordF *transTbl = (CoordF *)((BYTE *)handlePtr + sizeof(TransBitmapHandleHeader));
		//MakeTransTableMultiF(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle, srcWidth, srcHeight, 
		//					rowCount, colCount, splitWidth,
		//					distParam);
	}

	return r;
}
