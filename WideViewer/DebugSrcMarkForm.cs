using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WideViewer
{
    public partial class DebugSrcMarkForm : Form
    {
        private Bitmap srcBm = null;
        private float startAngle = 0;

        private Bitmap makeMarkedBitmap(int[] tbl, int outWidth, int outHeight, Bitmap srcBm)
        {
            if (tbl == null) return null;

            Bitmap outBm = new Bitmap(srcBm.Width, srcBm.Height, srcBm.PixelFormat);

            Rectangle rect0 = new Rectangle(0, 0, srcBm.Width, srcBm.Height);
            System.Drawing.Imaging.BitmapData bmpData0 = srcBm.LockBits(rect0, System.Drawing.Imaging.ImageLockMode.ReadOnly, srcBm.PixelFormat);
            Rectangle rect = new Rectangle(0, 0, outBm.Width, outBm.Height);
            System.Drawing.Imaging.BitmapData bmpData = outBm.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly, outBm.PixelFormat);

            IntPtr ptr0 = bmpData0.Scan0;
            IntPtr ptr = bmpData.Scan0;

            int bytes0 = Math.Abs(bmpData0.Stride) * srcBm.Height;

            int bytes = Math.Abs(bmpData.Stride) * outBm.Height;
            byte[] rgbValues = new byte[bytes];

            System.Runtime.InteropServices.Marshal.Copy(ptr0, rgbValues, 0, bytes0);

            //TransBitmapAux.SrcMark(tbl, outWidth, outHeight, rgbValues, srcBm.Width, srcBm.Height, 0);

            System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

            srcBm.UnlockBits(bmpData0);
            outBm.UnlockBits(bmpData);

            return outBm;
        }
        public DebugSrcMarkForm()
        {
            InitializeComponent();
        }

        public void setBitmap(Bitmap srcBm)
        {
            this.srcBm = srcBm;
        }

        public void setStartAngle(float startAngle)
        {
            this.startAngle = startAngle;
        }

        public void setParam(float v1, float v2, float v3, float v4)
        {
            if (srcBm == null) return;

            TransBitmapTable.Distortion xx;
            unsafe
            {
                xx.algorithm[0] = (byte)'A';
                xx.algorithm[1] = (byte)'M';
                xx.algorithm[2] = (byte)'S';
                xx.algorithm[3] = (byte)'0';
                xx.algorithm[4] = (byte)'2';
                xx.algorithm[5] = (byte)'\0';
                xx.param[0] = (float)v1;
                xx.param[1] = (float)v2;
                xx.param[2] = (float)v3;
                xx.param[3] = (float)v4;
            }

            // 소스 영역 1번
            int width = srcBm.Width / 4; // 160;
            int height = srcBm.Height / 4; // 120;
            int[] transTbl = new int[width * height];

            TransBitmapTable.MakeRGBTransTable0(transTbl, width, height, Global.trans.centerX, Global.trans.centerY, Global.trans.innerRadius, Global.trans.outerRadius,
                Global.trans.srcWidth, Global.trans.srcHeight, startAngle, 90,
                        0, 0, width, height,
                        ref xx);
            Bitmap img = makeMarkedBitmap(transTbl, width, height, srcBm);

            picViewerCtrl1.Image = img;
            picViewerCtrl1.Fit();

            picViewerCtrl1.Refresh();
        }

        private void DebugSrcMarkForm_Load(object sender, EventArgs e)
        {
            //picViewerCtrl1.Fit();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
