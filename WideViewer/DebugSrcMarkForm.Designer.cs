namespace WideViewer
{
    partial class DebugSrcMarkForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.picViewerCtrl1 = new PicViewerCtrl.PicViewerCtrl();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // picViewerCtrl1
            // 
            this.picViewerCtrl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.picViewerCtrl1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picViewerCtrl1.DragMode = true;
            this.picViewerCtrl1.Image = null;
            this.picViewerCtrl1.Location = new System.Drawing.Point(13, 13);
            this.picViewerCtrl1.Name = "picViewerCtrl1";
            this.picViewerCtrl1.Ratio = 0F;
            this.picViewerCtrl1.Size = new System.Drawing.Size(642, 482);
            this.picViewerCtrl1.TabIndex = 0;
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(539, 502);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(116, 35);
            this.button1.TabIndex = 1;
            this.button1.Text = "Close";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // DebugSrcMarkForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(668, 549);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.picViewerCtrl1);
            this.Name = "DebugSrcMarkForm";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.DebugSrcMarkForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private PicViewerCtrl.PicViewerCtrl picViewerCtrl1;
        private System.Windows.Forms.Button button1;
    }
}