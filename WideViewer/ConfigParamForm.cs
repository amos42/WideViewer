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
    public partial class ConfigParamForm : Form
    {
        int centerX, centerY;
        int innerRadius, outerRadius;
        float startAngle;
        float widthRatio;

        List<PolarRect> impAreaList = null;
        int selImpArea = -1;

        //double ratio;
        int curX, curY;
        float curAngle, curDist;
        int curButton = 0;
        PolarRect curArea = new PolarRect(0,0,0,0);

        public ConfigParamForm()
        {
            InitializeComponent();
        }

        public void setBitmap(Bitmap bm)
        {
            //pictureBox1.Image = bm;
            //ratio = (double)pictureBox1.ClientSize.Width / Global.trans.srcWidth;

            picViewerCtrl1.Image = bm;
            picViewerCtrl1.Fit();
        }

        public void setOutBitmap(Bitmap bm)
        {
            int width = 320;
            int height = 240;
            int xmid = width / 2;
            int[] transTbl = new int[width * height];
            double q = height * 0.15;
            //double p = 0;
            double a = q / (xmid * xmid);

            TransBitmapTable.Distortion xx;
            unsafe
            {
                xx.algorithm[0] = 0;
            }
            //TransBitmapAux.MakeTransTableMulti(transTbl, 160, 120, Global.trans.centerX, Global.trans.centerY, Global.trans.innerRadius, Global.trans.outerRadius,
            //    Global.trans.startAngle, 360, Global.trans.srcWidth, Global.trans.srcHeight, 
            //                2, 2, 12,
            //                ref xx);
            //Bitmap img = MakeBitmap(transTbl, 160, 120, srcBm);
            //picViewerCtrl2.Image = img;
        }

        private void refreshImpList()
        {
            listView1.Items.Clear();
            int idx = 0;
            foreach (PolarRect item in impAreaList)
            {
                String txt = String.Format("{0:0.0}, {1:0.0} - {2:0.0}, {3:0.0}", item.start.angle, item.start.dist, item.end.angle, item.end.dist);
                String[] itm = { (idx++).ToString(), txt };
                listView1.Items.Add(new ListViewItem(itm));
            }
        }

        private void drawPolarArea(Graphics g, Pen pen, PolarCoord start, PolarCoord end, double ratio)
        {
            XYCoord coord1 = PolarUtil.TransPolarToXY(start.angle, start.dist, centerX, centerY);
            XYCoord coord2 = PolarUtil.TransPolarToXY(start.angle, end.dist, centerX, centerY);
            g.DrawLine(pen, (int)(coord1.x * ratio), (int)(coord1.y * ratio), (int)(coord2.x * ratio), (int)(coord2.y * ratio));
            XYCoord coord3 = PolarUtil.TransPolarToXY(end.angle, start.dist, centerX, centerY);
            XYCoord coord4 = PolarUtil.TransPolarToXY(end.angle, end.dist, centerX, centerY);
            g.DrawLine(pen, (int)(coord3.x * ratio), (int)(coord3.y * ratio), (int)(coord4.x * ratio), (int)(coord4.y * ratio));

            double angle1 = start.angle;
            double angle2 = end.angle;
            if (angle2 < angle1)
            {
                angle2 += 360;
            }

            g.DrawArc(pen, (int)((centerX - start.dist) * ratio), (int)((centerY - start.dist) * ratio),
                (int)(start.dist * 2 * ratio), (int)(start.dist * 2 * ratio), (float)start.angle, (float)(angle2 - angle1));
            g.DrawArc(pen, (int)((centerX - end.dist) * ratio), (int)((centerY - end.dist) * ratio),
                (int)(end.dist * 2 * ratio), (int)(end.dist * 2 * ratio), (float)start.angle, (float)(angle2 - angle1));
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            centerX = Global.trans.centerX;
            centerY = Global.trans.centerY;
            innerRadius = Global.trans.innerRadius;
            outerRadius = Global.trans.outerRadius;
            startAngle = Global.trans.startAngle;
            widthRatio = Global.trans.widthRatio;
            impAreaList = new List<PolarRect>(Global.trans.getImpAreaList());

            numericUpDown4.Value = centerX;
            numericUpDown3.Value = centerY;
            numericUpDown1.Value = innerRadius;
            numericUpDown2.Value = outerRadius;
            numericUpDown6.Value = (int)startAngle;
            

            refreshImpList();

            comboBox1.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Global.trans.centerX = centerX;
            Global.trans.centerY = centerY;
            Global.trans.innerRadius = innerRadius;
            Global.trans.outerRadius = outerRadius;
            Global.trans.startAngle = startAngle;
            Global.trans.widthRatio = widthRatio;

            Global.trans.getImpAreaList().Clear();
            foreach(PolarRect item in impAreaList){
                Global.trans.addImpArea(item);
            }

            this.DialogResult = DialogResult.OK;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void numericUpDown4_ValueChanged(object sender, EventArgs e)
        {
            centerX = (int)numericUpDown4.Value;
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {
            centerY = (int)numericUpDown3.Value;
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            innerRadius = (int)numericUpDown1.Value;
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            outerRadius = (int)numericUpDown2.Value;
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void numericUpDown6_ValueChanged(object sender, EventArgs e)
        {
            startAngle = (float)numericUpDown6.Value;
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void listView1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Delete)
            {
                if (selImpArea >= 0 && selImpArea < impAreaList.Count)
                {
                    impAreaList.RemoveAt(selImpArea);
                    if (selImpArea >= impAreaList.Count)
                    {
                        selImpArea = impAreaList.Count - 1;
                    }
                }
                picViewerCtrl1.InvalidatePictureBox();
            }
        }

        private void listView1_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            selImpArea = e.ItemIndex;
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void picViewerCtrl1_PictureBoxMouseDown(object sender, MouseEventArgs e)
        {
            double ratio = picViewerCtrl1.Ratio;

            if (comboBox1.SelectedIndex == 1)
            {
                centerX = (int)(e.X / ratio);
                centerY = (int)(e.Y / ratio);
                //numericUpDown4.Value = centerX;
                //numericUpDown3.Value = centerY;
            }
            else if (comboBox1.SelectedIndex == 2)
            {
                innerRadius = (int)curDist;
                //numericUpDown6.Value = (int)startAngle;
            }
            else if (comboBox1.SelectedIndex == 2)
            {
                outerRadius = (int)curDist;
                //numericUpDown6.Value = (int)startAngle;
            }
            else if (comboBox1.SelectedIndex == 4)
            {
                startAngle = curAngle;
                //numericUpDown6.Value = (int)startAngle;
            }
            else if (comboBox1.SelectedIndex == 5)
            {
                curArea.start.angle = curArea.end.angle = curAngle;
                curArea.start.dist = curArea.end.dist = curDist;
            }
            curButton = 1;
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void picViewerCtrl1_PictureBoxMouseEnter(object sender, EventArgs e)
        {
            label1.Text = "Position : 0,0";
            label8.Text = "Polar : 0(deg),0";
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void picViewerCtrl1_PictureBoxMouseLeave(object sender, EventArgs e)
        {
            curX = -1;
            curAngle = -1;
            curDist = -1;
            label1.Text = "Position : ---,---";
            label8.Text = "Polar : ---(deg),---";
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void picViewerCtrl1_PictureBoxMouseMove(object sender, MouseEventArgs e)
        {
            float ratio = picViewerCtrl1.Ratio;

            curX = e.X;
            curY = e.Y;

            float x = (float)e.X / ratio;
            float y = (float)e.Y / ratio;
            PolarCoord polar = PolarUtil.TransXYToPolar(x - centerX, y - centerY);
            curAngle = polar.angle;
            curDist = polar.dist;

            if (curButton == 1)
            {
                if (comboBox1.SelectedIndex == 1)
                {
                    centerX = (int)(e.X / ratio);
                    centerY = (int)(e.Y / ratio);
                    //numericUpDown4.Value = centerX;
                    //numericUpDown3.Value = centerY;
                }
                else if (comboBox1.SelectedIndex == 2)
                {
                    innerRadius = (int)curDist;
                }
                else if (comboBox1.SelectedIndex == 3)
                {
                    outerRadius = (int)curDist;
                }
                else if (comboBox1.SelectedIndex == 4)
                {
                    startAngle = curAngle;
                    //numericUpDown6.Value = (int)startAngle;
                }
                else if (comboBox1.SelectedIndex == 5)
                {
                    curArea.end.angle = curAngle;
                    curArea.end.dist = curDist;
                }
            }

            label1.Text = String.Format("Position : {0:0},{1:0}", x, y);
            label8.Text = String.Format("Polar : {0:0.00}(deg),{1:0.00}", curAngle, curDist);
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void picViewerCtrl1_PictureBoxMouseUp(object sender, MouseEventArgs e)
        {
            if (curButton == 1)
            {
                if (comboBox1.SelectedIndex == 1)
                {
                    //centerX = (int)(e.X / ratio);
                    //centerY = (int)(e.Y / ratio);
                    numericUpDown4.Value = centerX;
                    numericUpDown3.Value = centerY;
                }
                else if (comboBox1.SelectedIndex == 2)
                {
                    numericUpDown1.Value = (int)innerRadius;
                }
                else if (comboBox1.SelectedIndex == 3)
                {
                    numericUpDown2.Value = (int)outerRadius;
                }
                else if (comboBox1.SelectedIndex == 4)
                {
                    //startAngle = curAngle;
                    numericUpDown6.Value = (int)startAngle;
                }
                else if (comboBox1.SelectedIndex == 5)
                {
                    if (Math.Abs(curArea.end.angle - curArea.start.angle) > 0 &&
                        Math.Abs(curArea.end.dist - curArea.start.dist) > 0)
                    {
                        if (curArea.start.dist > curArea.end.dist)
                        {
                            float t = curArea.start.dist;
                            curArea.start.dist = curArea.end.dist;
                            curArea.end.dist = t;
                        }
                        impAreaList.Add(new PolarRect(curArea));
                    }

                    refreshImpList();
                }
            }

            curButton = 0;
            picViewerCtrl1.InvalidatePictureBox();
        }

        private void picViewerCtrl1_PictureBoxPaint(object sender, PaintEventArgs e)
        {
            double ratio = picViewerCtrl1.Ratio;

            Pen pen = new Pen(Color.Red);
            Pen curPen = new Pen(Color.Blue);
            Pen curPen2 = new Pen(Color.Green);
            Pen curPen3 = new Pen(Color.White);
            Font font = new Font("Arial", 32);
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

            PolarRect selItem = null;
            if (selImpArea >= 0 && selImpArea < impAreaList.Count)
            {
                selItem = impAreaList.ElementAt(selImpArea);
            }
            foreach (PolarRect item in impAreaList)
            {
                drawPolarArea(e.Graphics, (item == selItem) ? curPen : pen, item.start, item.end, ratio);
            }

            if (curX >= 0)
            {
                e.Graphics.DrawLine(curPen, curX, 0, curX, picViewerCtrl1.GetPictureBox().Height);
                e.Graphics.DrawLine(curPen, 0, curY, picViewerCtrl1.GetPictureBox().Width, curY);
            }

            if (curAngle >= 0)
            {
                coord1 = PolarUtil.TransPolarToXY(curAngle, innerRadius, centerX, centerY);
                coord2 = PolarUtil.TransPolarToXY(curAngle, outerRadius, centerX, centerY);
                e.Graphics.DrawLine(curPen2, (int)(coord1.x * ratio), (int)(coord1.y * ratio), (int)(coord2.x * ratio), (int)(coord2.y * ratio));
            }

            if (curDist >= 0)
            {
                e.Graphics.DrawEllipse(curPen2, (int)((centerX - curDist) * ratio), (int)((centerY - curDist) * ratio),
                    (int)(curDist * 2 * ratio), (int)(curDist * 2 * ratio));
            }

            if (comboBox1.SelectedIndex == 5)
            {
                if (curButton == 1)
                {
                    drawPolarArea(e.Graphics, curPen3, curArea.start, curArea.end, ratio);

                    coord1 = PolarUtil.TransPolarToXY(curArea.start.angle, curArea.start.dist, centerX, centerY);
                    coord2 = PolarUtil.TransPolarToXY(curArea.start.angle, curArea.end.dist, centerX, centerY);
                    XYCoord coord3 = PolarUtil.TransPolarToXY(curArea.end.angle, curArea.start.dist, centerX, centerY);
                    XYCoord coord4 = PolarUtil.TransPolarToXY(curArea.end.angle, curArea.end.dist, centerX, centerY);

                    e.Graphics.DrawRectangle(curPen3, (int)(coord1.x * ratio) - 2, (int)(coord1.y * ratio) - 2, 5, 5);
                    e.Graphics.DrawRectangle(curPen3, (int)(coord2.x * ratio) - 2, (int)(coord2.y * ratio) - 2, 5, 5);
                    e.Graphics.DrawRectangle(curPen3, (int)(coord3.x * ratio) - 2, (int)(coord3.y * ratio) - 2, 5, 5);
                    e.Graphics.DrawRectangle(curPen3, (int)(coord4.x * ratio) - 2, (int)(coord4.y * ratio) - 2, 5, 5);
                }
            }

            curPen.Dispose();
            pen.Dispose();
            font.Dispose();
            drawBrush.Dispose();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex == 0)
            {
                picViewerCtrl1.DragMode = true;
            }
            else
            {
                picViewerCtrl1.DragMode = false;
            }
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(listView1.SelectedItems.Count > 0){
                impAreaList.RemoveAt(listView1.SelectedItems[0].Index);
                selImpArea = -1;
                refreshImpList();
                picViewerCtrl1.InvalidatePictureBox();
            }
        }

    }
}
