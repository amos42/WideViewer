// TransBitmap.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include "GenTransTable.h"
#include <stdlib.h>


BOOL __declspec(dllexport) MakeTransTable0(CALLBACKFUNC *func, void *param, int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, int srcWidthStride, float srcPixelWSize, float srcPixelHSize,
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam)
{
	BOOL bDist = FALSE;
	if(distParam != NULL && distParam->algorithm[0] != '\0'){
		bDist = TRUE;
	}

	if(!bDist){
		for (int x = 0; x < width; x++)
		{
			double deg = startAngle + (double)x * sweepAngle / width;
			double cos_v = cos(deg * M_PI / 180);
			double sin_v = sin(deg * M_PI / 180);

			for (int y = 0; y < height; y++)
			{
				double pty = outerRadius - y * (outerRadius - innerRadius) / height;

				int x2 = centerX + (int)(cos_v * pty);
				int y2 = centerY + (int)(sin_v * pty);

				func(param, sx + x, sy + y, outWidth, outHeight, (float)x2, (float)y2, srcWidth, srcHeight, srcWidthStride, srcPixelWSize, srcPixelHSize);
			}
		}
	} else if(strcmp((const char *)distParam->algorithm, "AMS01") == 0){
		double round = distParam->param[0];
		double sAngRatio = distParam->param[1];
		double eAngRatio = distParam->param[2];

		if(sAngRatio <= 0) sAngRatio = 1;
		if(eAngRatio <= 0) eAngRatio = 1;

		double angle2 = startAngle + sweepAngle / 2 - 90;
		double cos_s = cos(angle2 * M_PI / 180);
		double sin_s = sin(angle2 * M_PI / 180);

		startAngle = 90 - sweepAngle / 2;
		double startAngle1 = startAngle * sAngRatio;
		double startAngle2 = startAngle * eAngRatio;

        double cos_v = cos(startAngle2 * M_PI / 180);
        double sin_v = sin(startAngle2 * M_PI / 180);

        double xe = outerRadius * cos_v;
        double ye = outerRadius * sin_v;

        cos_v = cos(startAngle1 * M_PI / 180);
        sin_v = sin(startAngle1 * M_PI / 180);
		double d = sin_v / cos_v; 

        double a0 = innerRadius * (1 + round);
        double b0 = innerRadius;
        double startAngle0 = atan(a0 * d / b0) * 180 / M_PI;

        double cos_v0 = cos(startAngle0 * M_PI / 180);
        double sin_v0 = sin(startAngle0 * M_PI / 180);

        double x0 = (a0 * cos_v0);
        double y0 = (b0 * sin_v0);

		for (int y = 0; y < height; y++)
		{
			double round2 = round - round * y / height;

            double xt = x0 + (xe - x0) * y / height;
            double yt = y0 + (ye - y0) * y / height;

			double startAngle3 = atan((1 + round2) * yt / xt) * 180 / M_PI;
            double cos0 = cos(startAngle3 * M_PI / 180);
            double sin0 = sin(startAngle3 * M_PI / 180);
            double a = xt / cos0;
            double b = yt / sin0;

			double sweepAngle2 = sweepAngle + (startAngle - startAngle3) * 2;

			for (int x = 0; x < width; x++)
			{
				double deg = startAngle3 + (double)x * sweepAngle2 / width;
				double cos_v = cos(deg * M_PI / 180);
				double sin_v = sin(deg * M_PI / 180);

				{
					float x2 = centerX + (float)(a * cos_v * cos_s - b * sin_v * sin_s);
					float y2 = centerY + (float)(a * cos_v * sin_s + b * sin_v * cos_s);

					func(param, sx + x, sy + height - 1 - y, outWidth, outHeight, x2, y2, srcWidth, srcHeight, srcWidthStride, srcPixelWSize, srcPixelHSize);
				}
			}
		}
	} else if(strcmp((const char *)distParam->algorithm, "AMS02") == 0){
		double sRound = distParam->param[0];
		double sAngRatio = distParam->param[1];
		double eAngRatio = distParam->param[2];
		double eRound = distParam->param[3];

		if(sAngRatio <= 0) sAngRatio = 1;
		if(eAngRatio <= 0) eAngRatio = 1;

		//startAngle = 90 - sweepAngle / 2;
		double startAngle1 = startAngle * sAngRatio;
		double startAngle2 = startAngle * eAngRatio;

        double cos_v = cos(startAngle2 * M_PI / 180);
        double sin_v = sin(startAngle2 * M_PI / 180);

        //double xe = outerRadius * cos_v;
        //double ye = outerRadius * sin_v;

        cos_v = cos(startAngle1 * M_PI / 180);
        sin_v = sin(startAngle1 * M_PI / 180);
		//double d = sin_v / cos_v; 

        //double x0 = innerRadius * cos_v;
        //double y0 = innerRadius * sin_v;

		for (int y = 0; y < height; y++)
		{
			double round2 = sRound + (eRound - sRound) * y / height;
			double r = innerRadius + (outerRadius - innerRadius) * y / height;

			double r2 = r * round2;

			cos_v = cos((startAngle + sweepAngle / 2) * M_PI / 180);
			sin_v = sin((startAngle + sweepAngle / 2) * M_PI / 180);

			double centerX2 = centerX - (r2 - r) * cos_v;
			double centerY2 = centerY - (r2 - r) * sin_v;

			sin_v = sin(sweepAngle / 2 * M_PI / 180);
			double sin_v2 = sin_v * r / r2;
			double sweepAngle2 = asin(sin_v2) * 180 / M_PI * 2;

			double startAngle3 = startAngle + sweepAngle / 2 - sweepAngle2 / 2;

			for (int x = 0; x < width; x++)
			{
				double deg = startAngle3 + (double)x * sweepAngle2 / width;
				cos_v = cos(deg * M_PI / 180);
				sin_v = sin(deg * M_PI / 180);

				{
					float x2 = (float)(centerX2 + r2 * cos_v - r2 * sin_v);
					float y2 = (float)(centerY2 + r2 * sin_v + r2 * cos_v);

					func(param, sx + x, sy + height - 1 - y, outWidth, outHeight, x2, y2, srcWidth, srcHeight, srcWidthStride, srcPixelWSize, srcPixelHSize);
				}
			}
		}
	}

	return TRUE;
}


static void func1(void *param, int desX, int desY, int desWidth, int desHeight, float srcX, float srcY, int srcWidth, int srcHeight, int srcWidthStride, float pixelWSize, float pixelHSize)
{
	int *transTbl = (int *)param;

	int addr = desY * desWidth + desX;
    if (srcX >= 0 && srcX < srcWidth && srcY >= 0 && srcY < srcHeight)
    {
        transTbl[addr] = (int)(srcY * pixelHSize) * srcWidthStride + (int)(srcX * pixelWSize);
    }
    else
    {
        transTbl[addr] = -2;
    }
}

BOOL __declspec(dllexport) MakeTransTable(int transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, int srcWidthStride, float srcPixelWSize, float srcPixelHSize,
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam)
{
	return MakeTransTable0(func1, transTbl, outWidth, outHeight, 
                                          centerX, centerY, innerRadius, outerRadius, 
                                          srcWidth, srcHeight, srcWidthStride, srcPixelWSize, srcPixelHSize,
                                          startAngle, sweepAngle,
										  sx, sy, width, height,
										  distParam);
}


static void func2(void *param, int desX, int desY, int desWidth, int desHeight, float srcX, float srcY, int srcWidth, int srcHeight, int srcWidthStride, float pixelWSize, float pixelHSize)
{
	CoordF *transTbl = (CoordF *)param;

	int addr = desY * desWidth + desX;
    if (srcX >= 0 && srcX < srcWidth && srcY >= 0 && srcY < srcHeight)
    {
        transTbl[addr].x = srcX;
		transTbl[addr].y = srcY;
    }
    else
    {
        transTbl[addr].x = transTbl[addr].y = -9999 - 1;
    }
}

BOOL __declspec(dllexport) __stdcall MakeTransTableF(CoordF transTbl[], int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          int srcWidth, int srcHeight, 
                                          double startAngle, double sweepAngle,
										  int sx, int sy, int width, int height,
										  Distortion *distParam)
{
	return MakeTransTable0(func2, transTbl, outWidth, outHeight, 
                                          centerX, centerY, innerRadius, outerRadius, 
                                          srcWidth, srcHeight, srcWidth, sizeof(CoordF), 1,
                                          startAngle, sweepAngle,
										  sx, sy, width, height,
										  distParam);
}

