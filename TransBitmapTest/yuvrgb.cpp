
#include "yuvrgb.h"



#define clip(var) ((var>=255)?255:(var<=0)?0:var) 


#define CLIP(t) (((t)>255)?255:(((t)<0)?0:(t)))//값의 범위가  0 <= t <= 255 되도록 한다
 
//pRGB24의 크기는 nHeight*nWidth*3의 크기여야 한다
int YUV422_to_RGB24(const unsigned int nWidth, const unsigned int nHeight, const unsigned char *pYUV, unsigned char *pRGB24, int rgb_bgr)
{
    if(!pYUV || !pRGB24 || nWidth == 0 || nHeight == 0)
        return -1;

	unsigned int nYUY2Size = nHeight * nWidth / 2;//yuv422 크기
    unsigned int nRGB24Pos = 0;

	const unsigned char *py, *pu, *pv;
    unsigned char y1, y2, u, v;
    unsigned char r, g, b;
	int y_pos, uv_pos;

	y_pos = uv_pos = 0;

	py = pYUV;
	pu = &pYUV[nWidth * nHeight];
	pv = &pYUV[nWidth * nHeight + (nWidth/2 * nHeight)];

    //이미지의 한 행(row)의 길이가 nWidth*3이 아닌 경우 이 소스는 정상동작 하지 않을 수 있다
    int nStride = ((((nWidth * 24) + 31) & ~31) >> 3);

    for(int i = 0 ; i < nYUY2Size ; i++) {
        y1 = py[y_pos];
        u = pu[uv_pos];
        y2 = py[y_pos + 1];
        v = pv[uv_pos];

        //floating 연산이 제거된 공식
        //참고 : http://minnimok.egloos.com/5161623  

		if(y1 > 16){
			b = CLIP(( 76284*(y1-16) + 132252*(u-128) ) >> 16);
			g = CLIP(( 76284*(y1-16) -  53281*(v-128) -  25625*(u-128) ) >> 16);
			r = CLIP(( 76284*(y1-16) + 104595*(v-128) ) >> 16);
		} else {
			b = g = r = 0;
		}

		if(rgb_bgr == 0){
			pRGB24[nRGB24Pos] = r;
			pRGB24[nRGB24Pos + 1] = g;
			pRGB24[nRGB24Pos + 2] = b;
		} else {
			pRGB24[nRGB24Pos] = b;
			pRGB24[nRGB24Pos + 1] = g;
			pRGB24[nRGB24Pos + 2] = r;
		}

        nRGB24Pos += 3;

        //floating 연산이 제거된 공식
		if(y1 > 16){
			b = CLIP(( 76284*(y2-16) + 132252*(u-128) ) >> 16);
			g = CLIP(( 76284*(y2-16) -  53281*(v-128) -  25625*(u-128) ) >> 16);
			r = CLIP(( 76284*(y2-16) + 104595*(v-128) ) >> 16);
		} else {
			b = g = r = 0;
		}

		if(rgb_bgr == 0){
			pRGB24[nRGB24Pos] = r;
			pRGB24[nRGB24Pos + 1] = g;
			pRGB24[nRGB24Pos + 2] = b;
		} else {
			pRGB24[nRGB24Pos] = b;
			pRGB24[nRGB24Pos + 1] = g;
			pRGB24[nRGB24Pos + 2] = r;
		}

        nRGB24Pos += 3;

		y_pos += 2;
		uv_pos ++;
    }

    return 0;//성공
}


//pRGB24의 크기는 nHeight*nWidth*3의 크기여야 한다
int RGB24_to_YUV422(const unsigned int nWidth, const unsigned int nHeight, const unsigned char *pRGB24, unsigned char *pYUV)
{
    if(!pYUV || !pRGB24 || nWidth == 0 || nHeight == 0)
        return -1;

	unsigned int nYUY2Size = nHeight * nWidth / 2;//yuv422 크기
    unsigned int nRGB24Pos = 0;

	unsigned char *py, *pu, *pv;
    unsigned char y1, y2, u, v;
    unsigned char r1, g1, b1, r2, g2, b2;
	int y_pos, uv_pos;

	y_pos = uv_pos = 0;

	py = pYUV;
	pu = &pYUV[nWidth * nHeight];
	pv = &pYUV[nWidth * nHeight + (nWidth/2 * nHeight)];

    //이미지의 한 행(row)의 길이가 nWidth*3이 아닌 경우 이 소스는 정상동작 하지 않을 수 있다
    int nStride = ((((nWidth * 24) + 31) & ~31) >> 3);

    for(int i = 0 ; i < nYUY2Size ; i++) {
        r1 = pRGB24[nRGB24Pos];
        g1 = pRGB24[nRGB24Pos + 1];
        b1 = pRGB24[nRGB24Pos + 2];

        nRGB24Pos += 3;

        r2 = pRGB24[nRGB24Pos];
        g2 = pRGB24[nRGB24Pos + 1];
        b2 = pRGB24[nRGB24Pos + 2];

        nRGB24Pos += 3;

        //floating 연산이 제거된 공식
        //참고 : http://minnimok.egloos.com/5161623  

		y1 = ( (  66 * r1 + 129 * g1 +  25 * b1 + 128) >> 8) + 16;
		y2 = ( (  66 * r2 + 129 * g2 +  25 * b2 + 128) >> 8) + 16;
		u = ((( -38 * r1 -  74 * g1 + 112 * b1 + 128) >> 8) + (( -38 * r2 -  74 * g2 + 112 * b2 + 128) >> 8) + 256) >> 1; 
		v = ((( 112 * r1 -  94 * g1 - 18 * b1 + 128) >> 8) + (( 112 * r2 -  94 * g2 - 18 * b2 + 128) >> 8) + 256) >> 1; 

        py[y_pos] = y1;
        pu[uv_pos] = u;
        py[y_pos + 1] = y2;
        pv[uv_pos] = v;

		y_pos += 2;
		uv_pos ++;
    }

    return 0;//성공
}

//pRGB24의 크기는 nHeight*nWidth*3의 크기여야 한다
int YUV420_to_RGB24(const unsigned int nWidth, const unsigned int nHeight, const unsigned char *pYUV, unsigned char *pRGB24, int rgb_bgr)
{
    if(!pYUV || !pRGB24 || nWidth == 0 || nHeight == 0)
        return -1;

	unsigned int nYUY2Size = nHeight * nWidth;
    unsigned int nRGB24Pos1, nRGB24Pos2;

	const unsigned char *py1, *py2, *pu, *pv;
    unsigned char y1, y2, y3, y4, u, v;
    //unsigned char r, g, b;
	int r0, g0, b0;
	int y_pos, uv_pos;

	y_pos = uv_pos = 0;

	py1 = pYUV;
	py2 = &pYUV[nWidth];
	pu = &pYUV[nYUY2Size];
	pv = &pYUV[nYUY2Size + (nYUY2Size / 4)];

    //이미지의 한 행(row)의 길이가 nWidth*3이 아닌 경우 이 소스는 정상동작 하지 않을 수 있다
    int nStride = ((((nWidth * 24) + 31) & ~31) >> 3);

	nRGB24Pos1 = 0;
	nRGB24Pos2 = nStride;

    for(int i = 0 ; i < nHeight/2; i++){
		for(int j = 0 ; j < nWidth/2; j++){
			y1 = py1[y_pos];
			y2 = py1[y_pos + 1];
			y3 = py2[y_pos];
			y4 = py2[y_pos + 1];
			u = pu[uv_pos];
			v = pv[uv_pos];

			//floating 연산이 제거된 공식
			//참고 : http://minnimok.egloos.com/5161623  
			b0 = ( 76284*(y1-16) + 132252*(u-128) ) >> 16;
			g0 = ( 76284*(y1-16) -  53281*(v-128) -  25625*(u-128) ) >> 16;
			r0 = ( 76284*(y1-16) + 104595*(v-128) ) >> 16;
			if(rgb_bgr == 0){
				pRGB24[nRGB24Pos1] = CLIP(r0);
				pRGB24[nRGB24Pos1 + 1] = CLIP(g0);
				pRGB24[nRGB24Pos1 + 2] = CLIP(b0);
			} else {
				pRGB24[nRGB24Pos1] = CLIP(b0);
				pRGB24[nRGB24Pos1 + 1] = CLIP(g0);
				pRGB24[nRGB24Pos1 + 2] = CLIP(r0);
			}

			nRGB24Pos1 += 3;

			b0 = ( 76284*(y2-16) + 132252*(u-128) ) >> 16;
			g0 = ( 76284*(y2-16) -  53281*(v-128) -  25625*(u-128) ) >> 16;
			r0 = ( 76284*(y2-16) + 104595*(v-128) ) >> 16;
			if(rgb_bgr == 0){
				pRGB24[nRGB24Pos1] = CLIP(r0);
				pRGB24[nRGB24Pos1 + 1] = CLIP(g0);
				pRGB24[nRGB24Pos1 + 2] = CLIP(b0);
			} else {
				pRGB24[nRGB24Pos1] = CLIP(b0);
				pRGB24[nRGB24Pos1 + 1] = CLIP(g0);
				pRGB24[nRGB24Pos1 + 2] = CLIP(r0);
			}

			nRGB24Pos1 += 3;

			b0 = ( 76284*(y3-16) + 132252*(u-128) ) >> 16;
			g0 = ( 76284*(y3-16) -  53281*(v-128) -  25625*(u-128) ) >> 16;
			r0 = ( 76284*(y3-16) + 104595*(v-128) ) >> 16;
			if(rgb_bgr == 0){
				pRGB24[nRGB24Pos2] = CLIP(r0);
				pRGB24[nRGB24Pos2 + 1] = CLIP(g0);
				pRGB24[nRGB24Pos2 + 2] = CLIP(b0);
			} else {
				pRGB24[nRGB24Pos2] = CLIP(b0);
				pRGB24[nRGB24Pos2 + 1] = CLIP(g0);
				pRGB24[nRGB24Pos2 + 2] = CLIP(r0);
			}

			nRGB24Pos2 += 3;

			b0 = ( 76284*(y4-16) + 132252*(u-128) ) >> 16;
			g0 = ( 76284*(y4-16) -  53281*(v-128) -  25625*(u-128) ) >> 16;
			r0 = ( 76284*(y4-16) + 104595*(v-128) ) >> 16;
			if(rgb_bgr == 0){
				pRGB24[nRGB24Pos2] = CLIP(r0);
				pRGB24[nRGB24Pos2 + 1] = CLIP(g0);
				pRGB24[nRGB24Pos2 + 2] = CLIP(b0);
			} else {
				pRGB24[nRGB24Pos2] = CLIP(b0);
				pRGB24[nRGB24Pos2 + 1] = CLIP(g0);
				pRGB24[nRGB24Pos2 + 2] = CLIP(r0);
			}

			nRGB24Pos2 += 3;

			y_pos += 2;
			uv_pos ++;
		}

		y_pos += nWidth;
		nRGB24Pos1 += nStride;
		nRGB24Pos2 += nStride;
	}

    return 0;//성공
}


//pRGB24의 크기는 nHeight*nWidth*3의 크기여야 한다
int RGB24_to_YUV420(const unsigned int nWidth, const unsigned int nHeight, const unsigned char *pRGB24, unsigned char *pYUV)
{
    if(!pYUV || !pRGB24 || nWidth == 0 || nHeight == 0)
        return -1;

	unsigned int nYUY2Size = nHeight * nWidth / 2;//yuv422 크기
    unsigned int nRGB24Pos = 0;

	unsigned char *py, *pu, *pv;
    unsigned char y1, y2, u, v;
    unsigned char r1, g1, b1, r2, g2, b2;
	int y_pos, uv_pos;

	y_pos = uv_pos = 0;

	py = pYUV;
	pu = &pYUV[nWidth * nHeight];
	pv = &pYUV[nWidth * nHeight + (nWidth * nHeight / 4)];

    //이미지의 한 행(row)의 길이가 nWidth*3이 아닌 경우 이 소스는 정상동작 하지 않을 수 있다
    int nStride = ((((nWidth * 24) + 31) & ~31) >> 3);

    for(register unsigned int i = 0 ; i < nYUY2Size ; i++)
    {
        r1 = pRGB24[nRGB24Pos];
        g1 = pRGB24[nRGB24Pos + 1];
        b1 = pRGB24[nRGB24Pos + 2];

        nRGB24Pos += 3;

        r2 = pRGB24[nRGB24Pos];
        g2 = pRGB24[nRGB24Pos + 1];
        b2 = pRGB24[nRGB24Pos + 2];

        nRGB24Pos += 3;

        //floating 연산이 제거된 공식
        //참고 : http://minnimok.egloos.com/5161623  

		y1 = ( (  66 * r1 + 129 * g1 +  25 * b1 + 128) >> 8) + 16;
		y2 = ( (  66 * r2 + 129 * g2 +  25 * b2 + 128) >> 8) + 16;
		u = ((( -38 * r1 -  74 * g1 + 112 * b1 + 128) >> 8) + (( -38 * r2 -  74 * g2 + 112 * b2 + 128) >> 8) + 256) >> 1; 
		v = ((( 112 * r1 -  94 * g1 - 18 * b1 + 128) >> 8) + (( 112 * r2 -  94 * g2 - 18 * b2 + 128) >> 8) + 256) >> 1; 

        py[y_pos] = y1;
        pu[uv_pos] = u;
        py[y_pos + 1] = y2;
        pv[uv_pos] = v;

		y_pos += 2;
		uv_pos ++;
    }

    return 0;//성공
}
