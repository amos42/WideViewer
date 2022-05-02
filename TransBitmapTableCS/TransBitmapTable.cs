using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TransBitmapTableCS
{
    public class TransBitmapTable
    {
        public struct CoordF
        {
            public float x;
            public float y;
        }

        public static void MakeTransTable(int[] transTbl, int outWidth, int outHeight, 
                                            int centerX, int centerY, int innerRadius, int outerRadius, 
                                            int srcWidth, int srcHeight, 
                                            double startAngle, double sweepAngle)
        {
            for (int x = 0; x < outWidth; x++)
            {
                double deg = startAngle + (double)x * sweepAngle / outWidth;
                double cos_v = Math.Cos(deg * Math.PI / 180);
                double sin_v = Math.Sin(deg * Math.PI / 180);

                int addr = x;
                for (int y = 0; y < outHeight; y++, addr += outWidth)
                {
                    int pty = outerRadius - y * (outerRadius - innerRadius) / outHeight;
                    int x2 = centerX + (int)(cos_v * pty);
                    int y2 = centerY + (int)(sin_v * pty);

                    if (x2 >= 0 && x2 < srcWidth && y2 >= 0 && y2 < srcHeight)
                    {
                        transTbl[addr] = (y2 * srcWidth + x2) * 3;
                    }
                    else
                    {
                        transTbl[addr] = -1;
                    }
                }
            }
        }

        public static void MakeTransTableF(CoordF[] transTbl, int outWidth, int outHeight,
                                            int centerX, int centerY, int innerRadius, int outerRadius,
                                            int srcWidth, int srcHeight,
                                            double startAngle, double sweepAngle)
        {
            for (int x = 0; x < outWidth; x++)
            {
                double deg = startAngle + (double)x * sweepAngle / outWidth;
                double cos_v = Math.Cos(deg * Math.PI / 180);
                double sin_v = Math.Sin(deg * Math.PI / 180);

                int addr = x;
                for (int y = 0; y < outHeight; y++, addr += outWidth)
                {
                    float pty = outerRadius - (float)y * (outerRadius - innerRadius) / outHeight;
                    float x2 = (float)(centerX + (cos_v * pty));
                    float y2 = (float)(centerY + (sin_v * pty));

                    if (x2 >= 0 && x2 < srcWidth && y2 >= 0 && y2 < srcHeight)
                    {
                        transTbl[addr].x = x2;
                        transTbl[addr].y = y2;
                    }
                    else
                    {
                        transTbl[addr].x = -9999;
                        transTbl[addr].y = -9999;
                    }
                }
            }
        }


        bool makeBitmap(int[] tbl, byte[] outBm, int outWidth, int outHeight, byte[] srcBm, int srcWidth, int srcHeight)
        {
            if (tbl == null) return false;

            for (int i = 0, idx = 0; i < outWidth * outHeight; i++, idx += 3)
            {
                int j = tbl[i];
                if (j >= 0)
                {
                    outBm[idx] = srcBm[j];
                    outBm[idx + 1] = srcBm[j + 1];
                    outBm[idx + 2] = srcBm[j + 2];
                }
                else
                {
                    outBm[idx] = 0;
                    outBm[idx + 1] = 0;
                    outBm[idx + 2] = 0;
                }
	        }

            return true;
        }


        bool makeBitmapF(CoordF[] tbl, byte[] outBm, int outWidth, int outHeight, byte[] srcBm, int srcWidth, int srcHeight)
        {
            if (tbl == null) return false;

            for (int i = 0, idx = 0; i < outWidth * outHeight; i++, idx += 3)
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
                    int r = srcBm[idx0];
                    int g = srcBm[idx0 + 1];
                    int b = srcBm[idx0 + 2];

                    int rr = (int)(r * x1_t * y1_t);
                    int gg = (int)(g * x1_t * y1_t);
                    int bb = (int)(b * x1_t * y1_t);
                    if (x1_t < 1 && iptx + 1 < srcWidth)
                    {
                        r = srcBm[idx0 + 3];
                        g = srcBm[idx0 + 3 + 1];
                        b = srcBm[idx0 + 3 + 2];
                        rr += (int)(r * x2_t * y1_t);
                        gg += (int)(g * x2_t * y1_t);
                        bb += (int)(b * x2_t * y1_t);
                    }
                    if (y1_t < 1 && ipty + 1 < srcHeight)
                    {
                        r = srcBm[idx0 + srcWidth * 3];
                        g = srcBm[idx0 + srcWidth * 3 + 1];
                        b = srcBm[idx0 + srcWidth * 3 + 2];
                        rr += (int)(r * x1_t * y2_t);
                        gg += (int)(g * x1_t * y2_t);
                        bb += (int)(b * x1_t * y2_t);
                    }
                    if (x1_t < 1 && y1_t < 1 && iptx + 1 < srcWidth && ipty + 1 < srcHeight)
                    {
                        r = srcBm[idx0 + srcWidth * 3 + 3];
                        g = srcBm[idx0 + srcWidth * 3 + 3 + 1];
                        b = srcBm[idx0 + srcWidth * 3 + 3 + 2];
                        rr += (int)(r * x2_t * y2_t);
                        gg += (int)(g * x2_t * y2_t);
                        bb += (int)(b * x2_t * y2_t);
                    }

                    outBm[idx] = (byte)rr;
                    outBm[idx + 1] = (byte)gg;
                    outBm[idx + 2] = (byte)bb;
                }
                else
                {
                    outBm[idx] = 0;
                    outBm[idx + 1] = 0;
                    outBm[idx + 2] = 0;
                }
            }

            return true;
        }


        bool transBitmap(byte[] outBm, int outWidth, int outHeight, 
                                                  int centerX, int centerY, int innerRadius, int outerRadius, 
                                                  double startAngle, double sweepAngle,
                                                  byte[] srcBm, int srcWidth, int srcHeight,
										          int opt)
        {
	        if(opt == 0){
		        int[] tbl = new int[outWidth * outHeight];
		        MakeTransTable(tbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle);
		        makeBitmap(tbl, outBm, outWidth, outHeight, srcBm, srcWidth, srcHeight);
	        } else if(opt == 1){
		        CoordF[] tbl = new CoordF[outWidth * outHeight];
		        MakeTransTableF(tbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, sweepAngle);
		        makeBitmapF(tbl, outBm, outWidth, outHeight, srcBm, srcWidth, srcHeight);
	        }

	        return true;
        }

    }
}
