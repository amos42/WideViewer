using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;
using System.IO; 

namespace WideViewer
{
    public partial class MainForm : Form
    {
        IntPtr transBitmapHandle = IntPtr.Zero;

        byte[] srcBuf = null;
        int srcWidth = 0, srcHeight = 0;
        int srcFormat = 0;

        Bitmap srcBm = null;
        Bitmap outBm = null;
        //double q = 100;
        DebugSrcMarkForm form = null;

        private Bitmap makeBitmapFromRGB24Stream(byte[] buf, long len, int width, int height)
        {
            Bitmap bm = new Bitmap(width, height, PixelFormat.Format24bppRgb);

            System.Drawing.Imaging.BitmapData bmpData = bm.LockBits(new Rectangle(0, 0, width, height), System.Drawing.Imaging.ImageLockMode.WriteOnly, bm.PixelFormat);

            IntPtr ptr = bmpData.Scan0;

            int bytes = Math.Abs(bmpData.Stride) * height;
            //byte[] rgbValues = new byte[bytes];

            //for (int i = 0; i < bytes; i++) rgbValues[i] = 128;

            System.Runtime.InteropServices.Marshal.Copy(buf, 0, ptr, bytes);

            bm.UnlockBits(bmpData);

            return bm;
        }

        private Bitmap makeBitmapFromYUV420Stream(byte[] buf, long len, int width, int height)
        {
            Bitmap bm = new Bitmap(width, height, PixelFormat.Format24bppRgb);

            System.Drawing.Imaging.BitmapData bmpData = bm.LockBits(new Rectangle(0, 0, width, height), System.Drawing.Imaging.ImageLockMode.WriteOnly, bm.PixelFormat);

            IntPtr ptr = bmpData.Scan0;

            int bytes = Math.Abs(bmpData.Stride) * height;
            byte[] rgbValues = new byte[bytes];

            for (int i = 0; i < bytes; i++) rgbValues[i] = 128;

            YUVConvert.YUV420_to_BGR24(width, height, buf, rgbValues);

            System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

            bm.UnlockBits(bmpData);

            return bm;
        }

        private Bitmap makeBitmapFromYUV422Stream(byte[] buf, long len, int width, int height)
        {
            Bitmap bm = new Bitmap(width, height, PixelFormat.Format24bppRgb);

            System.Drawing.Imaging.BitmapData bmpData = bm.LockBits(new Rectangle(0, 0, width, height), System.Drawing.Imaging.ImageLockMode.WriteOnly, bm.PixelFormat);

            IntPtr ptr = bmpData.Scan0;

            int bytes = Math.Abs(bmpData.Stride) * height;
            byte[] rgbValues = new byte[bytes];

            for (int i = 0; i < bytes; i++) rgbValues[i] = 128;

            YUVConvert.YUV422_to_BGR24(width, height, buf, rgbValues);

            System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

            bm.UnlockBits(bmpData);

            return bm;
        }

        private void analysisImageFile(string filename)
        {
            srcFormat = -1;
            if (filename.Substring(filename.Length - 4).Equals(".yuv"))
            {
                int idx = filename.LastIndexOf('_');
                String fmtStr = filename.Substring(idx + 1, filename.Length - idx - 4 - 1);
                int idx2 = filename.LastIndexOf('x', idx);
                String heightStr = filename.Substring(idx2 + 1, idx - idx2 - 1);
                int idx3 = filename.LastIndexOf('_', idx2);
                String widthStr = filename.Substring(idx3 + 1, idx2 - idx3 - 1);

                srcWidth = Convert.ToInt32(widthStr);
                srcHeight = Convert.ToInt32(heightStr);

                FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
                long len = fs.Length;
                srcBuf = new byte[len];
                fs.Read(srcBuf, 0, (int)len);
                fs.Close();

                if(fmtStr.Equals("yuv420p"))
                {
                    srcFormat = 1;
                    srcBm = makeBitmapFromYUV420Stream(srcBuf, len, srcWidth, srcHeight);
                }
                else if (fmtStr.Equals("yuv422p"))
                {
                    srcFormat = 2;
                    srcBm = makeBitmapFromYUV422Stream(srcBuf, len, srcWidth, srcHeight);
                }
            }
            else
            {
                srcBm = new Bitmap(filename);

                srcWidth = srcBm.Width;
                srcHeight = srcBm.Height;

                Rectangle rect0 = new Rectangle(0, 0, srcWidth, srcHeight);
                System.Drawing.Imaging.BitmapData bmpData0 = srcBm.LockBits(rect0, System.Drawing.Imaging.ImageLockMode.ReadOnly, srcBm.PixelFormat);

                IntPtr ptr = bmpData0.Scan0;

                int bytes = Math.Abs(bmpData0.Stride) * srcBm.Height;
                srcBuf = new byte[bytes];

                System.Runtime.InteropServices.Marshal.Copy(ptr, srcBuf, 0, bytes);

                srcBm.UnlockBits(bmpData0);

                srcFormat = 0;
            }
        }

        private IntPtr makeHandle(int outWidth, int outHeight, 
                                          int centerX, int centerY, int innerRadius, int outerRadius, 
                                          float startAngle, float sweepAngle,
                                          int srcWidth, int srcHeight,
										  int opt, int opt2, int opt3)
        {
            UInt32 dwOpt = (opt == 0) ? TransBitmapAux.TRANS_OPT_SPEED : TransBitmapAux.TRANS_OPT_QUALITY;
            if (opt2 == 0)
            {
                dwOpt |= TransBitmapAux.TRANS_OPT_RGB24;
            }
            else if (opt2 == 1)
            {
                dwOpt |= TransBitmapAux.TRANS_OPT_YUV420;
            }
            else if (opt2 == 2)
            {
                dwOpt |= TransBitmapAux.TRANS_OPT_YUV422;
            }

            int sz = TransBitmapAux.GetRequireHandleSize(outWidth, outHeight, dwOpt);
            IntPtr handle = System.Runtime.InteropServices.Marshal.AllocHGlobal(sz);

            TransBitmapTable.Distortion xx;
            if (opt3 == 0)
            {
                unsafe
                {
                    xx.algorithm[0] = 0;
                }
            }
            else
            {
                unsafe
                {
                    xx.algorithm[0] = (byte)'A';
                    xx.algorithm[1] = (byte)'M';
                    xx.algorithm[2] = (byte)'S';
                    xx.algorithm[3] = (byte)'0';
                    xx.algorithm[4] = (byte)'1';
                    xx.algorithm[5] = (byte)'\0';
                    xx.param[0] = 0.8f;
                }
            }

            int[] srcStride = new int[3];
            srcStride[0] = srcWidth;
            int srcStrideCnt = 1;
            if (opt2 == 1 || opt2 == 2)
            {
                srcStride[1] = srcWidth / 2;
                srcStride[2] = srcWidth / 2;
                srcStrideCnt = 3;
            }
            TransBitmapAux.InitTransBitmapHandle(handle, outWidth, outHeight,
                                          centerX, centerY, innerRadius, outerRadius,
                                          startAngle, sweepAngle,
                                          srcWidth, srcHeight,
                                          srcStride, srcStrideCnt,
                                          1, 1, 0,
                                          dwOpt,
                                          ref xx);

            return handle;
        }

        private void removeHandle(IntPtr handle)
        {
            System.Runtime.InteropServices.Marshal.FreeHGlobal(handle);
        }

        private Bitmap makeBitmapByHandle(IntPtr transBitmapHandle, int outSize_width, int outSize_height, Bitmap srcBmX, UInt32 clBorderColor, UInt32 clBlankColor)
        {
            if (transBitmapHandle == IntPtr.Zero) return null;

            //TransBitmapTable.SizeT outSize = new TransBitmapTable.SizeT();
            //TransBitmapTable.getOutBitmapSize(transBitmapHandle, ref outSize);
            //Bitmap outBm = new Bitmap(outSize.width, outSize.height, srcBm.PixelFormat);
            Bitmap outBm = new Bitmap(outSize_width, outSize_height, srcBm.PixelFormat);

            //Rectangle rect0 = new Rectangle(0, 0, srcBm.Width, srcBm.Height);
            //System.Drawing.Imaging.BitmapData bmpData0 = srcBm.LockBits(rect0, System.Drawing.Imaging.ImageLockMode.ReadOnly, srcBm.PixelFormat);
            Rectangle rect = new Rectangle(0, 0, outBm.Width, outBm.Height);
            System.Drawing.Imaging.BitmapData bmpData = outBm.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly, outBm.PixelFormat);

            //IntPtr ptr0 = bmpData0.Scan0;
            //IntPtr ptr0 = srcBuf;
            IntPtr ptr = bmpData.Scan0;

            IntPtr[] rgbValuesx = new IntPtr[3];
            IntPtr[] rgbValues0x = new IntPtr[3];
            unsafe
            {
                fixed (byte* xx = srcBuf)
                {
                    if (srcFormat == 0)
                    {
                        rgbValuesx[0] = ptr;
                        rgbValues0x[0] = new IntPtr((void*)xx);
                        TransBitmapAux.TransBitmapByHandle0(transBitmapHandle, rgbValuesx, rgbValues0x, clBorderColor, clBlankColor, TransBitmapTable.RENDER_OPT_RGB);
                    }
                    else if (srcFormat == 1)
                    {
                        rgbValues0x[0] = new IntPtr((void*)xx);
                        rgbValues0x[1] = new IntPtr((void*)(xx + srcWidth * srcHeight));
                        rgbValues0x[2] = new IntPtr((void*)(xx + srcWidth * srcHeight + (srcWidth * srcHeight) / 4));
                        byte[] yy = new byte[outSize_width * outSize_height + outSize_width * outSize_height / 4 + outSize_width * outSize_height / 4];
                        fixed (byte* pyy = yy)
                        {
                            rgbValuesx[0] = new IntPtr((void*)pyy);
                            rgbValuesx[1] = new IntPtr((void*)(pyy + outSize_width * outSize_height));
                            rgbValuesx[2] = new IntPtr((void*)(pyy + outSize_width * outSize_height + (outSize_width * outSize_height) / 4));
                            TransBitmapAux.TransBitmapByHandle0(transBitmapHandle, rgbValuesx, rgbValues0x, clBorderColor, clBlankColor, 0);
                            int bytes = outSize_width * outSize_height * 3;
                            byte[] rgb = new byte[bytes];
                            YUVConvert.YUV420_to_BGR24(outSize_width, outSize_height, yy, rgb);
                            System.Runtime.InteropServices.Marshal.Copy(rgb, 0, ptr, bytes);
                        }
                    }
                    else if (srcFormat == 2)
                    {
                        rgbValues0x[0] = new IntPtr((void*)xx);
                        rgbValues0x[1] = new IntPtr((void*)(xx + srcWidth * srcHeight));
                        rgbValues0x[2] = new IntPtr((void*)(xx + srcWidth * srcHeight + (srcWidth * srcHeight) / 2));
                        byte[] yy = new byte[outSize_width * outSize_height + outSize_width * outSize_height / 2 + outSize_width * outSize_height / 2];
                        fixed (byte* pyy = yy)
                        {
                            rgbValuesx[0] = new IntPtr((void*)pyy);
                            rgbValuesx[1] = new IntPtr((void*)(pyy + outSize_width * outSize_height));
                            rgbValuesx[2] = new IntPtr((void*)(pyy + outSize_width * outSize_height + (outSize_width * outSize_height) / 2));
                            TransBitmapAux.TransBitmapByHandle0(transBitmapHandle, rgbValuesx, rgbValues0x, clBorderColor, clBlankColor, 0);
                            int bytes = outSize_width * outSize_height * 3;
                            byte[] rgb = new byte[bytes];
                            YUVConvert.YUV422_to_BGR24(outSize_width, outSize_height, yy, rgb);
                            System.Runtime.InteropServices.Marshal.Copy(rgb, 0, ptr, bytes);
                        }
                    }
                }
            }

            //srcBm.UnlockBits(bmpData0);
            outBm.UnlockBits(bmpData);

            return outBm;
        }


        private Bitmap makeBitmap(int[] tbl, int outWidth, int outHeight/*, Bitmap srcBmX*/, byte[] srcBuf, int srcFormat)
        {
            if (tbl == null) return null;

            Bitmap outBm = new Bitmap(outWidth, outHeight, PixelFormat.Format24bppRgb);

            //Rectangle rect0 = new Rectangle(0, 0, srcBm.Width, srcBm.Height);
            //System.Drawing.Imaging.BitmapData bmpData0 = srcBm.LockBits(rect0, System.Drawing.Imaging.ImageLockMode.ReadOnly, srcBm.PixelFormat);
            Rectangle rect = new Rectangle(0, 0, outBm.Width, outBm.Height);
            System.Drawing.Imaging.BitmapData bmpData = outBm.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly, outBm.PixelFormat);

            //IntPtr ptr0 = bmpData0.Scan0;
            IntPtr ptr = bmpData.Scan0;

            //int bytes0 = Math.Abs(bmpData0.Stride) * srcBm.Height;
            //byte[] rgbValues0 = new byte[bytes0];

            int bytes = Math.Abs(bmpData.Stride) * outBm.Height;
            byte[] rgbValues = new byte[bytes];

            //System.Runtime.InteropServices.Marshal.Copy(ptr0, rgbValues0, 0, bytes0);

            if (srcFormat == 0)
            {
                TransBitmapTable.MakeRGBBitmap0(tbl, rgbValues, outBm.Width, outBm.Height, srcBuf, 0, 0, TransBitmapTable.RENDER_OPT_RGB);
            }
            else if (srcFormat == 1)
            {
                //TransBitmapTable.MakeYUVBitmap0(tbl, rgbValues, outBm.Width, outBm.Height, srcBuf, 0, 0, TransBitmapTable.RENDER_OPT_YUV420);
            }
            else if (srcFormat == 2)
            {
                //TransBitmapTable.MakeYUVBitmap0(tbl, rgbValues, outBm.Width, outBm.Height, srcBuf, 0, 0, TransBitmapTable.RENDER_OPT_YUV422);
            }

            System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

            //srcBm.UnlockBits(bmpData0);
            outBm.UnlockBits(bmpData);

            return outBm;
        }

        private Bitmap makeBitmapF(TransBitmapTable.CoordF[] tbl, int outWidth, int outHeight/*, Bitmap srcBmX*/)
        {
            if (tbl == null) return null;

            Bitmap outBm = new Bitmap(outWidth, outHeight, PixelFormat.Format24bppRgb);

            //Rectangle rect0 = new Rectangle(0, 0, srcBm.Width, srcBm.Height);
            //System.Drawing.Imaging.BitmapData bmpData0 = srcBm.LockBits(rect0, System.Drawing.Imaging.ImageLockMode.ReadOnly, srcBm.PixelFormat);
            Rectangle rect = new Rectangle(0, 0, outBm.Width, outBm.Height);
            System.Drawing.Imaging.BitmapData bmpData = outBm.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly, outBm.PixelFormat);

            //IntPtr ptr0 = bmpData0.Scan0;
            IntPtr ptr = bmpData.Scan0;

            //int bytes0 = Math.Abs(bmpData0.Stride) * srcBm.Height;
            //byte[] rgbValues0 = new byte[bytes0];

            int bytes = Math.Abs(bmpData.Stride) * outBm.Height;
            byte[] rgbValues = new byte[bytes];

            //System.Runtime.InteropServices.Marshal.Copy(ptr0, rgbValues0, 0, bytes0);
            //System.Runtime.InteropServices.Marshal.Copy(ptr0, srcBuf, 0, bytes0);

            //TransBitmapAux.MakeBitmapF(tbl, rgbValues, outBm.Width, outBm.Height, srcBuf, srcWidth, srcHeight, 0);

            System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

            //srcBm.UnlockBits(bmpData0);
            outBm.UnlockBits(bmpData);

            return outBm;
        }

        private Bitmap makeBitmap0(int outWidth, int outHeight, 
                                   int centerX, int centerY, int innerRadius, int outerRadius, double startAngle, double sweepAngle, 
                                   Bitmap srcBmX, int opt)
        {
            Bitmap outBm = new Bitmap(outWidth, outHeight, PixelFormat.Format24bppRgb);

            //Rectangle rect0 = new Rectangle(0, 0, srcBm.Width, srcBm.Height);
            //System.Drawing.Imaging.BitmapData bmpData0 = srcBm.LockBits(rect0, System.Drawing.Imaging.ImageLockMode.ReadOnly, srcBm.PixelFormat);
            Rectangle rect = new Rectangle(0, 0, outBm.Width, outBm.Height);
            System.Drawing.Imaging.BitmapData bmpData = outBm.LockBits(rect, System.Drawing.Imaging.ImageLockMode.WriteOnly, outBm.PixelFormat);

            //IntPtr ptr0 = bmpData0.Scan0;
            IntPtr ptr = bmpData.Scan0;

            //int bytes0 = Math.Abs(bmpData0.Stride) * srcBm.Height;
            //byte[] rgbValues0 = new byte[bytes0];

            int bytes = Math.Abs(bmpData.Stride) * outBm.Height;
            byte[] rgbValues = new byte[bytes];

            //System.Runtime.InteropServices.Marshal.Copy(ptr0, rgbValues0, 0, bytes0);

            TransBitmapTable.Distortion xx;
            unsafe
            {
                xx.algorithm[0] = 0;
            }
            //TransBitmapAux.TransBitmap(rgbValues, outBm.Width, outBm.Height, 
            //    centerX, centerY, innerRadius, outerRadius, startAngle, sweepAngle,
            //    rgbValues0, srcBm.Width, srcBm.Height, (UInt32)opt, 
            //    ref xx);

            System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bytes);

            //srcBm.UnlockBits(bmpData0);
            outBm.UnlockBits(bmpData);

            return outBm;
        }

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (transBitmapHandle != IntPtr.Zero)
            {
                removeHandle(transBitmapHandle);
                transBitmapHandle = IntPtr.Zero;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //Global.trans.centerX = 1065;
            //Global.trans.centerY = 742;
            Global.trans.centerX = 1280;
            Global.trans.centerY = 880;
            Global.trans.innerRadius = 350;
            Global.trans.outerRadius = 980;
            Global.trans.startAngle = 220;
            //Global.trans.centerX = 1020;
            //Global.trans.centerY = 850;
            //Global.trans.innerRadius = 350;
            //Global.trans.outerRadius = 900;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Global.trans.setCenterPos(0, 0);

            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "모든 이미지 (*.jpg;*.bmp;*.png;*.yuv)|*.jpg;*.bmp;*.png;*.yuv|JPEG 이미지 (*.jpg)|*.jpg|BMP 이미지 (*.bmp)|*.bmp|PNG 이미지 (*.png)|*.png|YUV Raw 파일 (*.yuv)|*.yuv|모든 파일 (*.*)|*.*";
            if (dlg.ShowDialog() != DialogResult.OK) return;

            String filename = dlg.FileName;
            analysisImageFile(filename);

            Global.trans.srcWidth = srcWidth;
            Global.trans.srcHeight = srcHeight;
            listView1.Items[0].SubItems[1].Text = Global.trans.srcWidth.ToString() + "," + Global.trans.srcHeight.ToString();
            listView1.Items[1].SubItems[1].Text = srcBm.PixelFormat.ToString();

            //ratio = (double)pictureBox1.Width / Global.trans.srcWidth;
            //ratio = (double)panel1.ClientSize.Width / Global.trans.srcWidth;

            listView1.Items[2].SubItems[1].Text = Global.trans.centerX.ToString() + "," + Global.trans.centerY.ToString();
            listView1.Items[3].SubItems[1].Text = Global.trans.innerRadius.ToString();
            listView1.Items[4].SubItems[1].Text = Global.trans.outerRadius.ToString();

            //Global.trans.outWidth = (int)(2 * Math.PI * Global.trans.outerRadius);
            Global.trans.outWidth = (int)(2 * Math.PI * Global.trans.outerRadius * 0.63); // 약간의 이심률
            Global.trans.outWidth += 4 - Global.trans.outWidth % 4;
            Global.trans.outHeight = Global.trans.outerRadius - Global.trans.innerRadius;

            listView1.Items[5].SubItems[1].Text = Global.trans.outWidth.ToString() + "," + Global.trans.outHeight.ToString();

            outBm = new Bitmap(Global.trans.outWidth, Global.trans.outHeight, srcBm.PixelFormat);

            picViewerCtrl1.Image = srcBm;
            picViewerCtrl1.Fit();

            picViewerCtrl2.Image = outBm;
            picViewerCtrl2.Fit();

            button2_Click(button2, null);
            button3_Click(button3, null);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (transBitmapHandle != IntPtr.Zero)
            {
                removeHandle(transBitmapHandle);
                transBitmapHandle = IntPtr.Zero;
            }

            transBitmapHandle = makeHandle(Global.trans.outWidth, Global.trans.outHeight, 
                Global.trans.centerX, Global.trans.centerY, 
                Global.trans.innerRadius, Global.trans.outerRadius, 
                Global.trans.startAngle, (float)360, 
                Global.trans.srcWidth, Global.trans.srcHeight, 
                0, 
                srcFormat,
                0);
            //Global.trans.makeTransTable();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (transBitmapHandle == IntPtr.Zero) return;

            //outBm = makeBitmap(Global.trans.transTbl, outBm.Width, outBm.Height, srcBm);
            //outBm = makeBitmap0(outBm.Width, outBm.Height, Global.trans.centerX, Global.trans.centerY, Global.trans.innerRadius, Global.trans.outerRadius, 0, 360, srcBm, 1);
            outBm = makeBitmapByHandle(transBitmapHandle, outBm.Width, outBm.Height, srcBm, 0x000000, (0xFF << 16) | (0x00 << 8) | 0x00);

            picViewerCtrl2.Image = outBm;

            button9_Click(button9, null);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            ConfigParamForm form = new ConfigParamForm();
            form.setBitmap(srcBm);
            if (form.ShowDialog() != DialogResult.OK) return;

            listView1.Items[2].SubItems[1].Text = Global.trans.centerX.ToString() + "," + Global.trans.centerY.ToString();
            listView1.Items[3].SubItems[1].Text = Global.trans.innerRadius.ToString();
            listView1.Items[4].SubItems[1].Text = Global.trans.outerRadius.ToString();

            Global.trans.outWidth = (int)(2 * Math.PI * Global.trans.outerRadius * 0.63); // 약간의 이심률
            Global.trans.outWidth += 4 - Global.trans.outWidth % 4;
            Global.trans.outHeight = Global.trans.outerRadius - Global.trans.innerRadius;

            listView1.Items[5].SubItems[1].Text = Global.trans.outWidth.ToString() + "," + Global.trans.outHeight.ToString();

            outBm = new Bitmap(Global.trans.outWidth, Global.trans.outHeight, srcBm.PixelFormat);

            button2_Click(button2, null);
            button3_Click(button3, null);

            picViewerCtrl1.InvalidatePictureBox();
            picViewerCtrl2.Fit();
        }

        private void picViewerCtrl1_PictureBoxPaint(object sender, PaintEventArgs e)
        {
            float ratio = picViewerCtrl1.Ratio;

            int centerX = Global.trans.centerX;
            int centerY = Global.trans.centerY;
            int innerRadius = Global.trans.innerRadius;
            int outerRadius = Global.trans.outerRadius;
            float startAngle = Global.trans.startAngle;

            Pen pen = new Pen(Color.Red);
            Font font = new Font("Arial", 16);
            SolidBrush drawBrush = new SolidBrush(Color.White);

            e.Graphics.DrawLine(pen, new Point((int)(centerX * ratio), (int)(centerY * ratio) - 5), new Point((int)(centerX * ratio), (int)(centerY * ratio) + 5));
            e.Graphics.DrawLine(pen, new Point((int)(centerX * ratio) - 5, (int)(centerY * ratio)), new Point((int)(centerX * ratio) + 5, (int)(centerY * ratio)));

            e.Graphics.DrawEllipse(pen, (int)((centerX - innerRadius) * ratio), (int)((centerY - innerRadius) * ratio),
                                         (int)(innerRadius * 2 * ratio), (int)(innerRadius * 2 * ratio));
            e.Graphics.DrawEllipse(pen, (int)((centerX - outerRadius) * ratio), (int)((centerY - outerRadius) * ratio),
                                         (int)(outerRadius * 2 * ratio), (int)(outerRadius * 2 * ratio));

            XYCoord coord1, coord2;
            for (int i = 0; i < 4; i++)
            {
                float angle = startAngle + i * 90;
                coord1 = PolarUtil.TransPolarToXY(angle, innerRadius, centerX, centerY);
                coord2 = PolarUtil.TransPolarToXY(angle, outerRadius, centerX, centerY);
                e.Graphics.DrawLine(pen, (int)(coord1.x * ratio), (int)(coord1.y * ratio), (int)(coord2.x * ratio), (int)(coord2.y * ratio));

                angle += 45;
                int cent = innerRadius + (outerRadius - innerRadius) / 2;
                coord1 = PolarUtil.TransPolarToXY(angle, cent, centerX, centerY);
                e.Graphics.DrawString((i + 1).ToString(), font, drawBrush, (int)(coord1.x * ratio) - 8, (int)(coord1.y * ratio) - 16);
            }
        }

        private void button8_Click(object sender, EventArgs e)
        {
            panel1.Controls.Clear();
            int idx = 0;
            foreach (PolarRect item in Global.trans.getImpAreaList())
            {
                PicViewerCtrl.PicViewerCtrl pic = new PicViewerCtrl.PicViewerCtrl();
                pic.BorderStyle = BorderStyle.FixedSingle;

                float angle2 = item.end.angle;
                if (angle2 < item.start.angle)
                {
                    angle2 += 360;
                }

                int width = (int)(2 * Math.PI * item.end.dist * (angle2 - item.start.angle) / 360);
                width += 4 - width % 4;
                int height = (int)(item.end.dist - item.start.dist);

                Bitmap bm = makeBitmap0(width, height, Global.trans.centerX, Global.trans.centerY, (int)item.start.dist, (int)item.end.dist, item.start.angle, angle2 - item.start.angle, srcBm, 0);
                pic.Image = bm;

                pic.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right;
                pic.Width = panel1.ClientSize.Width - 8;
                pic.Height = 100;
                pic.Left = 4;
                pic.Top = idx * (100 + 4);

                panel1.Controls.Add(pic);
                pic.Fit();
                idx++;
            }
        }

        private void button9_Click(object sender, EventArgs e)
        {
            //q = int.Parse(textBox1.Text);
            //button2_Click(button2, null);
            //button3_Click(button3, null);

            int width = 160;
            int height = 120;
            int[] transTbl = new int[width * height];
            //TransBitmapTable.CoordF[] transTblF = new TransBitmapTable.CoordF[width * height];

            //int xmid = width / 2;
            //double q = height * 0.15;
            //double p = 0;
            //double a = q / (xmid * xmid);

            float v1 = (float)trackBar1.Value / 10;
            float v2 = (float)trackBar2.Value / 10;
            float v3 = (float)trackBar3.Value / 10;
            float v4 = (float)trackBar4.Value / 10;

            TransBitmapTable.Distortion xx;
            unsafe
            {
                xx.algorithm[0] = (byte)'A';
                xx.algorithm[1] = (byte)'M';
                xx.algorithm[2] = (byte)'S';
                xx.algorithm[3] = (byte)'0';
                xx.algorithm[4] = (byte)'1';
                xx.algorithm[5] = (byte)'\0';
                xx.param[0] = v1;
                xx.param[1] = v2;
                xx.param[2] = v3;
                xx.param[3] = v4;
            }

            {
                TransBitmapTable.MakeRGBTransTable0(transTbl, 160, 120, Global.trans.centerX, Global.trans.centerY, Global.trans.innerRadius, Global.trans.outerRadius,
                    Global.trans.srcWidth, Global.trans.srcHeight, Global.trans.startAngle, 90,
                            0, 0, 160, 120,
                            ref xx);
                Bitmap img = makeBitmap(transTbl, 160, 120, srcBuf, srcFormat);
                pictureBox3.Image = img;
            }

            {
                TransBitmapTable.MakeRGBTransTable0(transTbl, 160, 120, Global.trans.centerX, Global.trans.centerY, Global.trans.innerRadius, Global.trans.outerRadius,
                    Global.trans.srcWidth, Global.trans.srcHeight, Global.trans.startAngle + 90, 90,
                                0, 0, 160, 120,
                                ref xx);
                Bitmap img = makeBitmap(transTbl, 160, 120, srcBuf, srcFormat);
                pictureBox4.Image = img;
            }

            {
                TransBitmapTable.MakeRGBTransTable0(transTbl, 160, 120, Global.trans.centerX, Global.trans.centerY, Global.trans.innerRadius, Global.trans.outerRadius,
                    Global.trans.srcWidth, Global.trans.srcHeight, Global.trans.startAngle + 180, 90,
                                0, 0, 160, 120,
                                ref xx);
                Bitmap img = makeBitmap(transTbl, 160, 120, srcBuf, srcFormat);
                pictureBox5.Image = img;
            }

            {
                TransBitmapTable.MakeRGBTransTable0(transTbl, 160, 120, Global.trans.centerX, Global.trans.centerY, Global.trans.innerRadius, Global.trans.outerRadius,
                    Global.trans.srcWidth, Global.trans.srcHeight, Global.trans.startAngle + 270, 90,
                                0, 0, 160, 120,
                                ref xx);
                Bitmap img = makeBitmap(transTbl, 160, 120, srcBuf, srcFormat);
                pictureBox6.Image = img;
            }

        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            textBox1.Text = String.Format("{0:F1}", (float)trackBar1.Value / 10);
            button9_Click(button9, null);

            if (form != null && !form.IsDisposed)
            {
                float v1 = (float)trackBar1.Value / 10;
                float v2 = (float)trackBar2.Value / 10;
                float v3 = (float)trackBar3.Value / 10;
                float v4 = (float)trackBar4.Value / 10;
                form.setParam(v1, v2, v3, v4);
            }
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            textBox2.Text = String.Format("{0:F1}", (float)trackBar2.Value / 10);
            button9_Click(button9, null);

            if (form != null && !form.IsDisposed)
            {
                float v1 = (float)trackBar1.Value / 10;
                float v2 = (float)trackBar2.Value / 10;
                float v3 = (float)trackBar3.Value / 10;
                float v4 = (float)trackBar4.Value / 10;
                form.setParam(v1, v2, v3, v4);
            }
        }

        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            textBox3.Text = String.Format("{0:F1}", (float)trackBar3.Value / 10);
            button9_Click(button9, null);

            if (form != null && !form.IsDisposed)
            {
                float v1 = (float)trackBar1.Value / 10;
                float v2 = (float)trackBar2.Value / 10;
                float v3 = (float)trackBar3.Value / 10;
                float v4 = (float)trackBar4.Value / 10;
                form.setParam(v1, v2, v3, v4);
            }
        }

        private void pictureBox3_DoubleClick(object sender, EventArgs e)
        {
            float v1 = (float)trackBar1.Value / 10;
            float v2 = (float)trackBar2.Value / 10;
            float v3 = (float)trackBar3.Value / 10;
            float v4 = (float)trackBar4.Value / 10;

            if (form == null || form.IsDisposed)
            {
                form = new DebugSrcMarkForm();
                form.setBitmap(srcBm);
                form.Show();
            }
            form.setStartAngle(Global.trans.startAngle);
            form.setParam(v1, v2, v3, v4);
        }

        private void pictureBox4_DoubleClick(object sender, EventArgs e)
        {
            float v1 = (float)trackBar1.Value / 10;
            float v2 = (float)trackBar2.Value / 10;
            float v3 = (float)trackBar3.Value / 10;
            float v4 = (float)trackBar4.Value / 10;

            if (form == null || form.IsDisposed)
            {
                form = new DebugSrcMarkForm();
                form.setBitmap(srcBm);
                form.Show();
            }
            form.setStartAngle(Global.trans.startAngle + 90);
            form.setParam(v1, v2, v3, v4);
        }

        private void pictureBox5_DoubleClick(object sender, EventArgs e)
        {
            float v1 = (float)trackBar1.Value / 10;
            float v2 = (float)trackBar2.Value / 10;
            float v3 = (float)trackBar3.Value / 10;
            float v4 = (float)trackBar4.Value / 10;

            if (form == null || form.IsDisposed)
            {
                form = new DebugSrcMarkForm();
                form.setBitmap(srcBm);
                form.Show();
            }
            form.setStartAngle(Global.trans.startAngle + 180);
            form.setParam(v1, v2, v3, v4);
        }

        private void pictureBox6_DoubleClick(object sender, EventArgs e)
        {
            float v1 = (float)trackBar1.Value / 10;
            float v2 = (float)trackBar2.Value / 10;
            float v3 = (float)trackBar3.Value / 10;
            float v4 = (float)trackBar4.Value / 10;

            if (form == null || form.IsDisposed)
            {
                form = new DebugSrcMarkForm();
                form.setBitmap(srcBm);
                form.Show();
            }
            form.setStartAngle(Global.trans.startAngle + 270);
            form.setParam(v1, v2, v3, v4);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "JPEG 이미지 (*.jpg)|*.jpg|BMP 이미지 (*.bmp)|*.bmp|PNG 이미지 (*.png)|*.png";
            if (dlg.ShowDialog() != DialogResult.OK) return;

            ImageFormat fmt = null;
            String ext = Path.GetExtension(dlg.FileName).ToLower();
            if (ext == ".jpeg" || ext == ".jpg")
            {
                fmt = ImageFormat.Jpeg;
            }
            else if (ext == ".png")
            {
                fmt = ImageFormat.Png;
            }
            else if (ext == ".bmp")
            {
                fmt = ImageFormat.Bmp;
            }

            if (fmt != null)
            {
                picViewerCtrl2.Image.Save(dlg.FileName, fmt);
            }
        }

        private void trackBar4_Scroll(object sender, EventArgs e)
        {
            textBox4.Text = String.Format("{0:F1}", (float)trackBar4.Value / 10);
            button9_Click(button9, null);

            if (form != null && !form.IsDisposed)
            {
                float v1 = (float)trackBar1.Value / 10;
                float v2 = (float)trackBar2.Value / 10;
                float v3 = (float)trackBar3.Value / 10;
                float v4 = (float)trackBar4.Value / 10;
                form.setParam(v1, v2, v3, v4);
            }
        }


    }
}
