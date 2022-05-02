#ifndef __YUVRGB_H_
#define __YUVRGB_H_


#ifdef __cplusplus
extern "C" {
#endif


int YUV422_to_RGB24(const unsigned int nWidth, const unsigned int nHeight, const unsigned char *pYUV, unsigned char *pRGB24, int rgb_bgr);
int RGB24_to_YUV422(const unsigned int nWidth, const unsigned int nHeight, const unsigned char *pRGB24, unsigned char *pYUV);
int YUV420_to_RGB24(const unsigned int nWidth, const unsigned int nHeight, const unsigned char *pYUV, unsigned char *pRGB24, int rgb_bgr);
int RGB24_to_YUV420(const unsigned int nWidth, const unsigned int nHeight, const unsigned char *pRGB24, unsigned char *pYUV);


#ifdef __cplusplus
}
#endif


#endif