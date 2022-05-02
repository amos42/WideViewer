using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DistorDrawTest
{
    public partial class Form1 : Form
    {
        private int clientWidth, clientHeight;
        private int orgX, orgY;

        public Form1()
        {
            InitializeComponent();
        }

        private void drawBox(Graphics g, int x, int y, int width, int height, Color cl)
        {
            Pen pen = new Pen(cl);
            g.DrawRectangle(pen, orgX + x, orgY + y, width, height);
        }

        private void drawArc(Graphics g, int x, int y, int radius, int startAngle, int sweepAngle, Color cl)
        {
            Pen pen = new Pen(cl);
            g.DrawArc(pen, orgX + x - radius, orgY + y - radius, radius * 2, radius * 2, startAngle, sweepAngle);
        }

        private void drawCircle(Graphics g, int x, int y, int radius, Color cl)
        {
            drawArc(g, x, y, radius, 0, 360, cl);
        }

        private void drawCross(Graphics g, int x, int y, int dist, Color cl)
        {
            Pen pen = new Pen(cl);
            g.DrawLine(pen, orgX + x - dist / 2, orgY + y - dist / 2, orgX + x + dist / 2, orgY + y + dist / 2);
            g.DrawLine(pen, orgX + x - dist / 2, orgY + y + dist / 2, orgX + x + dist / 2, orgY + y - dist / 2);
        }

        private void drawAxis(Graphics g, int x, int y, Color cl)
        {
            Pen pen = new Pen(cl);
            g.DrawLine(pen, 0, orgY + y, clientWidth, orgY + y);
            g.DrawLine(pen, orgX + x, 0, orgX + x, clientHeight);
        }

        private void drawClient(Graphics g)
        {
            drawCircle(g, 0, 0, 100, Color.LightGray);

            double r = 100;
            double startAngle = 45;
            double sweepAngle = 90;

            drawArc(g, 0, 0, (int)r, (int)startAngle, (int)sweepAngle, Color.Blue);

            double deg = startAngle + sweepAngle / 2;

            double cos_v = Math.Cos(deg * Math.PI / 180);
            double sin_v = Math.Sin(deg * Math.PI / 180);
            double tan_v = Math.Sin(deg * Math.PI / 180);

            double startRatio = 1.6;
            double r2 = r * startRatio;

            double centX2 = 0 - cos_v * (r2 - r);
            double centY2 = 0 - sin_v * (r2 - r);

            drawCross(g, (int)centX2, (int)centY2, 4, Color.Red);

            //deg = startAngle + sweepAngle / 2;

            //double sweepAngle2 = Math.Atan(
            //r2 = sweepAngle2 / 
            //double sweepAngle2 = Math.Asin(sin_v * r / r2) * 180 / Math.PI;
            //double startAngle2 = deg - sweepAngle2 / 2;
            //drawArc(g, (int)centX2, (int)centY2, (int)r2, (int)startAngle2, (int)sweepAngle2, Color.Red);
        }

        private void draw(Graphics g, int width, int height)
        {
            clientWidth = width;
            clientHeight = height;
            orgX = width / 2;
            orgY = height / 2;

            drawAxis(g, 0, 0, Color.LightGray);
            //drawCross(g, 0, 0, 6, Color.Red);

            drawClient(g);
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.DrawRectangle(new Pen(Color.Black), 0, 0, pictureBox1.Width - 1, pictureBox1.Height - 1);

            draw(e.Graphics, pictureBox1.Width - 1, pictureBox1.Height - 1);
        }
    }
}
