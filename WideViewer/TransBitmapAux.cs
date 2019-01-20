using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;     // DLL support

namespace WideViewer
{
    class TransBitmapAux : TransBitmapTable
    {
        public const UInt32 TRANS_OPT_MASK	=   0x7F000000;

        public const UInt32 TRANS_OPT_RGB24	=	0x01000000;
        public const UInt32 TRANS_OPT_YUV422 =	0x02000000;
        public const UInt32 TRANS_OPT_YUV420 = 0x04000000;
        public const UInt32 TRANS_OPT_YUV444 = 0x08000000;

        public const UInt32 TRANS_OPT_SPEED	=	0x80000000;
        public const UInt32 TRANS_OPT_QUALITY =	0x40000000;

        public const UInt32 TRANS_OPT_DEBUG_SRCMARK = 0x00800000;

        [DllImport("TransBitmapAux.dll")]
        public static extern int GetRequireHandleSize(int outWidth, int outHeight, UInt32 dwTransOpt);

        [DllImport("TransBitmapAux.dll")]
        public static extern bool InitTransBitmapHandle(IntPtr handlePtr, int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
                                          int srcWidth, int srcHeight,
                                          int[] srcStride, int scrStrideCnt,
                                          int rowCount, int colCount, int splitWidth,
                                          UInt32 dwTransOpt,
                                          [In] ref Distortion distParam);

        [DllImport("TransBitmapAux.dll")]
        public static extern bool GetSrcBitmapSize(IntPtr handlePtr, ref SizeT size);

        [DllImport("TransBitmapAux.dll")]
        public static extern bool GetOutBitmapSize(IntPtr handlePtr, ref SizeT size);

        [DllImport("TransBitmapAux.dll")]
        public static extern bool TransBitmapByHandle0(IntPtr handlePtr, [Out] IntPtr[] outBm, [In] IntPtr[] srcBm, UInt32 clBorderColor, UInt32 clBlankColor, UInt32 dwRenderOpt);

        [DllImport("TransBitmapAux.dll")]
        public static extern bool TransBitmapByHandle(IntPtr handlePtr, [Out] byte[] outBm, [In] byte[] srcBm, UInt32 dwRenderOpt);

        [DllImport("TransBitmapAux.dll")]
        public static extern bool MakeTransBitmap([Out] IntPtr[] outBm, int outWidth, int outHeight,
                                          int centerX, int centerY, int innerRadius, int outerRadius,
                                          double startAngle, double sweepAngle,
                                          [In] IntPtr[] srcBm, int srcWidth, int srcHeight,
                                          int[] srcStride, int srcStrideCnt,
                                          UInt32 dwTransOpt, UInt32 dwRenderOpt,
                                          [In] ref Distortion distParam);
    }

}
