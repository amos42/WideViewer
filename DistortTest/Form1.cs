using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DistortTest
{
    public partial class Form1 : Form
    {
        private Bitmap srcBm;
        private Bitmap outBm;

        public Form1()
        {
            InitializeComponent();
        }

        public static void TransBitmap(int outWidth, int outHeight, byte[] outBitmap, int outVWidth,
                                       int srcWidth, int srcHeight, byte[] srcBitmap, int srcVWidth)
        {
            int xmid = srcWidth / 2;
            int ymid = srcHeight / 2;

            double a = (double)srcHeight * 0.2 / (xmid * xmid);

            for (int x = 0; x < outWidth; x++)
            {
                //double cos_v = Math.Cos(x * Math.PI / 180);
                //double sin_v = Math.Sin(y * Math.PI / 180);

                int addr = x * 3;
                for (int y = 0; y < outHeight; y++, addr += outVWidth)
                {
                    int xx = (int)Math.Abs(x - xmid);
                    double z = a * xx * xx + srcHeight * 0.8;
                    //z = 1 / z;

                    int x2 = x;
                    int y2 = ymid + (int)((y - ymid) * srcHeight / z);
                    y2 -= ymid + (int)((-ymid) * srcHeight / z);
                    //x2 = x;
                    //y2 = y;

                    y2 = (int)(y2 * 0.8);

                    if (x2 >= 0 && x2 < srcWidth && y2 >= 0 && y2 < srcHeight)
                    {
                        int addr0 = y2 * srcVWidth + x2 * 3;
                        outBitmap[addr] = srcBitmap[addr0];
                        outBitmap[addr + 1] = srcBitmap[addr0 + 1];
                        outBitmap[addr + 2] = srcBitmap[addr0 + 2];
                    }
                    else
                    {
                        outBitmap[addr] = 0;
                        outBitmap[addr + 1] = 0;
                        outBitmap[addr + 2] = 0;
                    }
                }
            }
        }

        private Bitmap makeBitmapByHandle(int outSize_width, int outSize_height, Bitmap srcBm)
        {
            Bitmap outBm = new Bitmap(outSize_width, outSize_height, srcBm.PixelFormat);

            Rectangle rect0 = new Rectangle(0, 0, srcBm.Width, srcBm.Height);
            System.Drawing.Imaging.BitmapData bmpData0 = srcBm.LockBits(rect0, System.Drawing.Imaging.ImageLockMode.ReadOnly, srcBm.PixelFormat);
            Rectangle rect = new Rectangle(0, 0, outBm.Width, outBm.Height);
            System.Drawing.Imaging.BitmapData bmpData = outBm.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly, outBm.PixelFormat);

            IntPtr ptr0 = bmpData0.Scan0;
            IntPtr ptr = bmpData.Scan0;

            int bytes0 = Math.Abs(bmpData0.Stride) * srcBm.Height;
            byte[] rgbValues0 = new byte[bytes0];

            int bytes = Math.Abs(bmpData.Stride) * outBm.Height;
            byte[] rgbValues = new byte[bytes];

            System.Runtime.InteropServices.Marshal.Copy(ptr0, rgbValues0, 0, bytes0);

            TransBitmap(outSize_width, outSize_height, rgbValues, bmpData.Stride,
                srcBm.Width, srcBm.Height, rgbValues0, bmpData0.Stride);

            System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

            srcBm.UnlockBits(bmpData0);
            outBm.UnlockBits(bmpData);

            return outBm;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //Global.trans.setCenterPos(0, 0);

            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "JPEG 이미지 (*.jpg)|*.jpg|모든 파일 (*.*)|*.*";
            if (dlg.ShowDialog() != DialogResult.OK) return;

            srcBm = new Bitmap(dlg.FileName);

            //Global.trans.srcWidth = srcBm.Width;
            //Global.trans.srcHeight = srcBm.Height;
            //listView1.Items[0].SubItems[1].Text = Global.trans.srcWidth.ToString() + "," + Global.trans.srcHeight.ToString();
            //listView1.Items[1].SubItems[1].Text = srcBm.PixelFormat.ToString();

            ////ratio = (double)pictureBox1.Width / Global.trans.srcWidth;
            ////ratio = (double)panel1.ClientSize.Width / Global.trans.srcWidth;

            //listView1.Items[2].SubItems[1].Text = Global.trans.centerX.ToString() + "," + Global.trans.centerY.ToString();
            //listView1.Items[3].SubItems[1].Text = Global.trans.innerRadius.ToString();
            //listView1.Items[4].SubItems[1].Text = Global.trans.outerRadius.ToString();

            ////Global.trans.outWidth = (int)(2 * Math.PI * Global.trans.outerRadius);
            //Global.trans.outWidth = (int)(2 * Math.PI * Global.trans.outerRadius * 0.63); // 약간의 이심률
            //Global.trans.outWidth += 4 - Global.trans.outWidth % 4;
            //Global.trans.outHeight = Global.trans.outerRadius - Global.trans.innerRadius;

            //listView1.Items[5].SubItems[1].Text = Global.trans.outWidth.ToString() + "," + Global.trans.outHeight.ToString();

            //outBm = new Bitmap(Global.trans.outWidth, Global.trans.outHeight, srcBm.PixelFormat);

            pictureBox1.Image = srcBm;

            //picViewerCtrl2.Image = outBm;
            //picViewerCtrl2.Fit();

            //button2_Click(button2, null);
            //button3_Click(button3, null);

        }

        private void button1_Click(object sender, EventArgs e)
        {
            outBm = makeBitmapByHandle(srcBm.Width, srcBm.Height, srcBm);

            pictureBox2.Image = outBm;
        }
    }
}
