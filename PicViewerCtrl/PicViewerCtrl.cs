using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PicViewerCtrl
{
    public partial class PicViewerCtrl : UserControl
    {
        private bool dragMode = true;

        private double ratio;
        private int imageWidth = 0;
        private int imageHeight = 0;
        private bool drag = false;
        private int moX, moY;

        public bool DragMode
        {
            get { return dragMode; }
            set {
                dragMode = value;
                pictureBox1.Enabled = !dragMode;
                panel1.Cursor = (dragMode)? Cursors.NoMove2D : Cursors.Default;
            }
        }

        public float Ratio
        {
            get { return (float)ratio; }
            set { ratio = (double)value; }
        }

        public Image Image
        {
            get { return pictureBox1.Image; }
            set {
                pictureBox1.Image = value;
                if (value != null)
                {
                    imageWidth = value.Width;
                    imageHeight = value.Height;
                    ratio = (double)pictureBox1.Width / imageWidth;
                }
                else
                {
                    imageWidth = 0;
                    imageHeight = 0;
                    ratio = (double)0;
                }
            }
        }

        public event PaintEventHandler PictureBoxPaint;
        public event MouseEventHandler PictureBoxMouseDown;
        public event MouseEventHandler PictureBoxMouseMove;
        public event MouseEventHandler PictureBoxMouseUp;
        public event EventHandler PictureBoxMouseEnter;
        public event EventHandler PictureBoxMouseLeave;

        public PicViewerCtrl()
        {
            InitializeComponent();
        }

        public PictureBox GetPictureBox()
        {
            return pictureBox1;
        }

        public void InvalidatePictureBox()
        {
            pictureBox1.Invalidate();
        }

        public void Fit()
        {
            ratio = (double)(panel1.Width) / imageWidth;
            if ((int)(imageHeight * ratio) > panel1.Height)
            {
                ratio = (double)(panel1.Height) / imageHeight;
            }
            btnOrigSize.Text = ((int)(ratio * 100)).ToString() + "%";
            pictureBox1.Width = (int)(imageWidth * ratio);
            pictureBox1.Height = (int)(imageHeight * ratio);

            DoLayout();
        }

        private void DoLayout()
        {
            if (pictureBox1.Width >= panel1.ClientSize.Width)
            {
                if (pictureBox1.Left > 0) pictureBox1.Left  = 0;
            }
            else
            {
                pictureBox1.Left = (panel1.ClientSize.Width - pictureBox1.Width) / 2;
            }

            if (pictureBox1.Height >= panel1.ClientSize.Height)
            {
                if (pictureBox1.Top > 0) pictureBox1.Top = 0;
            }
            else
            {
                pictureBox1.Top = (panel1.ClientSize.Height - pictureBox1.Height) / 2;
            }
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            if(PictureBoxPaint != null)
                PictureBoxPaint(sender, e);
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (PictureBoxMouseDown != null)
                PictureBoxMouseDown(sender, e);
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (PictureBoxMouseMove != null)
                PictureBoxMouseMove(sender, e);
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (PictureBoxMouseUp != null)
                PictureBoxMouseUp(sender, e);
        }

        private void pictureBox1_MouseEnter(object sender, EventArgs e)
        {
            if (PictureBoxMouseEnter != null)
                PictureBoxMouseEnter(sender, e);
        }

        private void pictureBox1_MouseLeave(object sender, EventArgs e)
        {
            if (PictureBoxMouseLeave != null)
                PictureBoxMouseLeave(sender, e);
        }

        private void PicViewerCtrl_Load(object sender, EventArgs e)
        {
        }

        private void PicViewerCtrl_Resize(object sender, EventArgs e)
        {
            panel1.Width = this.ClientSize.Width;
            panel1.Height = this.ClientSize.Height;
            DoLayout();
        }

        private void btnZoomIn_Click(object sender, EventArgs e)
        {
            ratio += ratio * 0.1;
            btnOrigSize.Text = ((int)(ratio * 100)).ToString() + "%";
            pictureBox1.Width = (int)(imageWidth * ratio);
            pictureBox1.Height = (int)(imageHeight * ratio);
            DoLayout();
        }

        private void btnZoomOut_Click(object sender, EventArgs e)
        {
            ratio -= ratio * 0.1;
            btnOrigSize.Text = ((int)(ratio * 100)).ToString() + "%";
            pictureBox1.Width = (int)(imageWidth * ratio);
            pictureBox1.Height = (int)(imageHeight * ratio);
            DoLayout();
        }

        private void btnAutofit_Click(object sender, EventArgs e)
        {
            Fit();
        }

        private void btnOrigSize_Click(object sender, EventArgs e)
        {
            ratio = 1;
            btnOrigSize.Text = ((int)(ratio * 100)).ToString() + "%";
            pictureBox1.Width = (int)(imageWidth * ratio);
            pictureBox1.Height = (int)(imageHeight * ratio);
            DoLayout();
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            if (dragMode)
            {
                drag = true;
                moX = e.X;
                moY = e.Y;
            }
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (drag)
            {
                if (panel1.HorizontalScroll.Enabled)
                {
                    int v = panel1.HorizontalScroll.Value;
                    v += moX - e.X;
                    if (v >= panel1.HorizontalScroll.Minimum && v <= panel1.HorizontalScroll.Maximum)
                    {
                        panel1.HorizontalScroll.Value = v;
                    }
                }
                moX = e.X;
                if (panel1.VerticalScroll.Enabled)
                {
                    int v = panel1.VerticalScroll.Value;
                    v += moY - e.Y;
                    if (v >= panel1.VerticalScroll.Minimum && v <= panel1.VerticalScroll.Maximum)
                    {
                        panel1.VerticalScroll.Value = v;
                    }
                }
                moY = e.Y;
            }
        }

        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {
            drag = false;
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            if (pictureBox1.Width >= panel1.Width && pictureBox1.Height >= panel1.Height) return;

            bool bidx0 = false;
            for (int i = 0; i < panel1.Height; i += 8)
            {
                bool bidx = bidx0;
                for (int j = 0; j < panel1.Width; j += 8)
                {
                    e.Graphics.FillRectangle((bidx)?Brushes.LightGray:Brushes.White, j, i, 8, 8);
                    bidx = !bidx;
                }
                bidx0 = !bidx0;
            }
        }
    }
}
