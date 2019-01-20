using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WideViewer
{
    public class YUVConvert
    {
        private static byte CLIP(int t)
        {
            return (byte)(((t) > 255) ? 255 : (((t) < 0) ? 0 : (t))); //값의 범위가  0 <= t <= 255 되도록 한다
        }

        public static int YUV420_to_BGR24(int nWidth, int nHeight, byte[] pYUV, byte[] pRGB24)
        {
            if (pYUV == null || pRGB24 == null || nWidth == 0 || nHeight == 0)
                return -1;

            int nYUY2Size = nHeight * nWidth;
            int nRGB24Pos1, nRGB24Pos2;

            unsafe
            {
                //byte* py1, py2, pu, pv;
                byte y1, y2, y3, y4, u, v;
                byte r, g, b;
                int y_pos, uv_pos;

                y_pos = uv_pos = 0;

                //이미지의 한 행(row)의 길이가 nWidth*3이 아닌 경우 이 소스는 정상동작 하지 않을 수 있다
                int nStride = ((((nWidth * 24) + 31) & ~31) >> 3);

                nRGB24Pos1 = 0;
                nRGB24Pos2 = nStride;

                fixed (byte* py1 = &pYUV[0],
                py2 = &pYUV[nWidth],
                pu = &pYUV[nYUY2Size],
                pv = &pYUV[nYUY2Size + (nYUY2Size / 4)])
                {

                    for (int i = 0; i < nHeight / 2; i++)
                    {
                        for (int j = 0; j < nWidth / 2; j++)
                        {
                            y1 = py1[y_pos];
                            y2 = py1[y_pos + 1];
                            y3 = py2[y_pos];
                            y4 = py2[y_pos + 1];
                            u = pu[uv_pos];
                            v = pv[uv_pos];

                            //floating 연산이 제거된 공식
                            //참고 : http://minnimok.egloos.com/5161623  
                            if (y1 > 16)
                            {
                                b = CLIP((76284 * (y1 - 16) + 132252 * (u - 128)) >> 16);
                                g = CLIP((76284 * (y1 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
                                r = CLIP((76284 * (y1 - 16) + 104595 * (v - 128)) >> 16);
                            }
                            else
                            {
                                b = g = r = 0;
                            }
                            pRGB24[nRGB24Pos1] = b;
                            pRGB24[nRGB24Pos1 + 1] = g;
                            pRGB24[nRGB24Pos1 + 2] = r;

                            nRGB24Pos1 += 3;

                            if (y2 > 16)
                            {
                                b = CLIP((76284 * (y2 - 16) + 132252 * (u - 128)) >> 16);
                                g = CLIP((76284 * (y2 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
                                r = CLIP((76284 * (y2 - 16) + 104595 * (v - 128)) >> 16);
                            }
                            else
                            {
                                b = g = r = 0;
                            }
                            pRGB24[nRGB24Pos1] = b;
                            pRGB24[nRGB24Pos1 + 1] = g;
                            pRGB24[nRGB24Pos1 + 2] = r;

                            nRGB24Pos1 += 3;

                            if (y3 > 16)
                            {
                                b = CLIP((76284 * (y3 - 16) + 132252 * (u - 128)) >> 16);
                                g = CLIP((76284 * (y3 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
                                r = CLIP((76284 * (y3 - 16) + 104595 * (v - 128)) >> 16);
                            }
                            else
                            {
                                b = g = r = 0;
                            }
                            pRGB24[nRGB24Pos2] = b;
                            pRGB24[nRGB24Pos2 + 1] = g;
                            pRGB24[nRGB24Pos2 + 2] = r;

                            nRGB24Pos2 += 3;

                            if (y4 > 16)
                            {
                                b = CLIP((76284 * (y4 - 16) + 132252 * (u - 128)) >> 16);
                                g = CLIP((76284 * (y4 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
                                r = CLIP((76284 * (y4 - 16) + 104595 * (v - 128)) >> 16);
                            }
                            else
                            {
                                b = g = r = 0;
                            }
                            pRGB24[nRGB24Pos2] = b;
                            pRGB24[nRGB24Pos2 + 1] = g;
                            pRGB24[nRGB24Pos2 + 2] = r;

                            nRGB24Pos2 += 3;

                            y_pos += 2;
                            uv_pos++;
                        }

                        y_pos += nWidth;
                        nRGB24Pos1 += nStride;
                        nRGB24Pos2 += nStride;
                    }
                }
            }

            return 0;//성공
        }

        public static int YUV422_to_BGR24(int nWidth, int nHeight, byte[] pYUV, byte[] pRGB24)
        {
            if (pYUV == null || pRGB24 == null || nWidth == 0 || nHeight == 0)
                return -1;

            int nYUY2Size = nHeight * nWidth;
            int nRGB24Pos;

            unsafe
            {
                //byte* py1, py2, pu, pv;
                byte y1, y2, u, v;
                byte r, g, b;
                int y_pos, uv_pos;

                y_pos = uv_pos = 0;

                //이미지의 한 행(row)의 길이가 nWidth*3이 아닌 경우 이 소스는 정상동작 하지 않을 수 있다
                int nStride = ((((nWidth * 24) + 31) & ~31) >> 3);

                nRGB24Pos = 0;

                fixed (byte* py = &pYUV[0],
                pu = &pYUV[nYUY2Size],
                pv = &pYUV[nYUY2Size + (nYUY2Size / 2)])
                {

                    for (int i = 0; i < nHeight; i++)
                    {
                        for (int j = 0; j < nWidth / 2; j++)
                        {
                            y1 = py[y_pos];
                            y2 = py[y_pos + 1];
                            u = pu[uv_pos];
                            v = pv[uv_pos];

                            //floating 연산이 제거된 공식
                            //참고 : http://minnimok.egloos.com/5161623  
                            if (y1 > 16)
                            {
                                b = CLIP((76284 * (y1 - 16) + 132252 * (u - 128)) >> 16);
                                g = CLIP((76284 * (y1 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
                                r = CLIP((76284 * (y1 - 16) + 104595 * (v - 128)) >> 16);
                            }
                            else
                            {
                                b = g = r = 0;
                            }
                            pRGB24[nRGB24Pos] = b;
                            pRGB24[nRGB24Pos + 1] = g;
                            pRGB24[nRGB24Pos + 2] = r;

                            nRGB24Pos += 3;

                            if (y2 > 16)
                            {
                                b = CLIP((76284 * (y2 - 16) + 132252 * (u - 128)) >> 16);
                                g = CLIP((76284 * (y2 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
                                r = CLIP((76284 * (y2 - 16) + 104595 * (v - 128)) >> 16);
                            }
                            else
                            {
                                b = g = r = 0;
                            }
                            pRGB24[nRGB24Pos] = b;
                            pRGB24[nRGB24Pos + 1] = g;
                            pRGB24[nRGB24Pos + 2] = r;

                            nRGB24Pos += 3;

                            y_pos += 2;
                            uv_pos++;
                        }

                        //y_pos += nWidth;
                        //nRGB24Pos += nStride;
                    }
                }
            }

            return 0;//성공
        }
    }
}
