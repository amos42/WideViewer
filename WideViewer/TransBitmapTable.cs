using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;     // DLL support

namespace WideViewer
{
    class TransBitmapTable
    {
        //public const UInt32 TRANS_OPT_MASK	=   0x7F000000;
        //public const UInt32 TRANS_OPT_SPEED	=	0x00000000;
        //public const UInt32 TRANS_OPT_QUALITY =	0x01000000;
        //public const UInt32 TRANS_OPT_DEBUG_SRCMARK = 0x80000000;

        public const UInt32 RENDER_OPT_MASK =	0x00FF0000;

        public const UInt32 RENDER_OPT_RGB =	0x00000000;
        public const UInt32 RENDER_OPT_BGR =	0x00010000;

        //public const UInt32 RENDER_OPT_YUV422 =	0x00040000;
        //public const UInt32 RENDER_OPT_YUV420 =	0x00020000;

        //#define MAKE_RGB(r, g, b)		(((DWORD)(r) << 16) | ((DWORD)(g) << 8) | ((DWORD)(b)))
        //#define RGB_R(color)			(((DWORD)(color) & 0x00FF0000) >> 16)
        //#define RGB_G(color)			(((DWORD)(color) & 0x0000FF00) >> 8)
        //#define RGB_B(color)			(((DWORD)(color) & 0x000000FF))

        //typedef DWORD RGBColor;

        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        public struct SizeT
        {
            public int width;
            public int height;
        }

        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        public struct CoordF
        {
            public float x;
            public float y;
        }

        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        public struct Distortion
        {
            public unsafe fixed byte algorithm[8];
            public unsafe fixed float param[8];
        }


        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeRGBTransTable0([Out] int[] transTbl, int outWidth, int outHeight,
                                          int centerX, int centerY, int innerRadius, int outerRadius,
                                          int srcWidth, int srcHeight,
                                          double startAngle, double sweepAngle,
                                          int sx, int sy, int width, int height,
                                          [In] ref Distortion distParam);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV422TransTable0([Out] int[] transTblY, [Out] int[] transTblU, [Out] int[] transTblV, int outWidth, int outHeight,
                                          int centerX, int centerY, int innerRadius, int outerRadius,
                                          int srcWidth, int srcHeight,
                                          double startAngle, double sweepAngle,
                                          int sx, int sy, int width, int height,
                                          [In] ref Distortion distParam);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV420TransTable0([Out] int[] transTblY, [Out] int[] transTblU, [Out] int[] transTblV, int outWidth, int outHeight,
                                          int centerX, int centerY, int innerRadius, int outerRadius,
                                          int srcWidth, int srcHeight,
                                          double startAngle, double sweepAngle,
                                          int sx, int sy, int width, int height,
                                          [In] ref Distortion distParam);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeTransTableF0([Out] CoordF[] transTbl, int outWidth, int outHeight,
                                          int centerX, int centerY, int innerRadius, int outerRadius,
                                          int srcWidth, int srcHeight,
                                          double startAngle, double sweepAngle,
                                          int sx, int sy, int width, int height,
                                          [In] ref Distortion distParam);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeRGBBitmap0([In] int[] tbl, [Out] byte[] outBm, int outWidth, int outHeight, [In] byte[] srcBm, UInt32 clBorderColor, UInt32 clBlankColor, UInt32 dwRenderOpt);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeRGBBitmapF0([In] CoordF[] tbl, [Out] byte[] outBm, int outWidth, int outHeight, [In] byte[] srcBm, int srcWidth, int srcHeight, UInt32 clBorderColor, UInt32 clBlankColor, UInt32 dwRenderOpt);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcRGBMark0([In] int[] tbl, int outWidth, int outHeight, [Out] byte[] srcBm, UInt32 clMarkColor1, UInt32 clMarkColor2, UInt32 dwRenderOpt);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcRGBMarkF0([In] CoordF[] tbl, int outWidth, int outHeight, [Out] byte[] srcBm, int srcWidth, int srcHeight, UInt32 clMarkColor1, UInt32 clMarkColor2, UInt32 dwRenderOpt);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV422Bitmap0([In] int[] tbl, [Out] byte[] outBmY, [Out] byte[] outBmU, [Out] byte[] outBmV, int outWidth, int outHeight, [In] byte[] srcBmY, [In] byte[] srcBmU, [In] byte[] srcBmV, UInt32 clBorderColor, UInt32 clBlankColor);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV422Bitmap1([In] int[] tbl, [Out] byte[] outBmYUV, int outWidth, int outHeight, [In] byte[] srcBmYUV, UInt32 clBorderColor, UInt32 clBlankColor);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV420Bitmap0([In] int[] tbl, [Out] byte[] outBmY, [Out] byte[] outBmU, [Out] byte[] outBmV, int outWidth, int outHeight, [In] byte[] srcBmY, [In] byte[] srcBmU, [In] byte[] srcBmV, UInt32 clBorderColor, UInt32 clBlankColor);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV420Bitmap1([In] int[] tbl, [Out] byte[] outBmYUV, int outWidth, int outHeight, [In] byte[] srcBmYUV, UInt32 clBorderColor, UInt32 clBlankColor);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV422BitmapF0([In] CoordF[] tbl, [Out] byte[] outBmY, [Out] byte[] outBmU, [Out] byte[] outBmV, int outWidth, int outHeight, [In] byte[] srcBmY, [In] byte[] srcBmU, [In] byte[] srcBmV, int srcWidth, int srcHeight, UInt32 clBorderColor, UInt32 clBlankColor);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV422BitmapF1([In] CoordF[] tbl, [Out] byte[] outBmYUV, int outWidth, int outHeight, [In] byte[] srcBmYUV, int srcWidth, int srcHeight, UInt32 clBorderColor, UInt32 clBlankColor);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV420BitmapF0([In] CoordF[] tbl, [Out] byte[] outBmY, [Out] byte[] outBmU, [Out] byte[] outBmV, int outWidth, int outHeight, [In] byte[] srcBmY, [In] byte[] srcBmU, [In] byte[] srcBmV, int srcWidth, int srcHeight, UInt32 clBorderColor, UInt32 clBlankColor);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool MakeYUV420BitmapF1([In] CoordF[] tbl, [Out] byte[] outBmYUV, int outWidth, int outHeight, [In] byte[] srcBmYUV, int srcWidth, int srcHeight, UInt32 clBorderColor, UInt32 clBlankColor);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcYUV422Mark0([In] int[] tbl, int outWidth, int outHeight, [Out] byte[] outBmY, [Out] byte[] outBmU, [Out] byte[] outBmV, UInt32 clMarkColor1, UInt32 clMarkColor2);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcYUV422Mark1([In] int[] tbl, int outWidth, int outHeight, [Out] byte[] outBmYUV, UInt32 clMarkColor1, UInt32 clMarkColor2);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcYUV422MarkF0([In] CoordF[] tbl, int outWidth, int outHeight, [Out] byte[] outBmY, [Out] byte[] outBmU, [Out] byte[] outBmV, int srcWidth, int srcHeight, UInt32 clMarkColor1, UInt32 clMarkColor2);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcYUV422MarkF1([In] CoordF[] tbl, int outWidth, int outHeight, [Out] byte[] outBmYUV, int srcWidth, int srcHeight, UInt32 clMarkColor1, UInt32 clMarkColor2);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcYUV420Mark0([In] int[] tbl, int outWidth, int outHeight, [Out] byte[] outBmY, [Out] byte[] outBmU, [Out] byte[] outBmV, UInt32 clMarkColor1, UInt32 clMarkColor2);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcYUV420Mark1([In] int[] tbl, int outWidth, int outHeight, [Out] byte[] outBmYUV, UInt32 clMarkColor1, UInt32 clMarkColor2);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcYUV420MarkF0([In] CoordF[] tbl, int outWidth, int outHeight, [Out] byte[] outBmY, [Out] byte[] outBmU, [Out] byte[] outBmV, int srcWidth, int srcHeight, UInt32 clMarkColor1, UInt32 clMarkColor2);

        [DllImport("TransBitmapTable.dll")]
        public static extern bool SrcYUV420MarkF1([In] CoordF[] tbl, int outWidth, int outHeight, [Out] byte[] outBmYUV, int srcWidth, int srcHeight, UInt32 clMarkColor1, UInt32 clMarkColor2);
    }

}
