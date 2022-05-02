using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WideViewer
{
    class TransBitmap
    {
        public int centerX, centerY;
        public int innerRadius, outerRadius;
        public float startAngle;
        public float widthRatio;

        public int srcWidth, srcHeight;
        public byte[] srcBitData;

        public int outWidth, outHeight;
        public byte[] outBitData = null;

        public int[] transTbl = null;

        List<PolarRect> impAreaList = new List<PolarRect>();


        //void refreshTransTable()
        //{
        //    makeTransTable();
        //}

        public void setSourceSize(int width, int height)
        {
            srcWidth = width;
            srcHeight = height;

            srcBitData = new byte[srcWidth * srcHeight];
        }

        //public void setCenterPos(int X, int Y)
        //{
        //    centerX = X;
        //    centerY = Y;

        //    refreshTransTable();
        //}

        //public void makeTransTable()
        //{
        //    transTbl = new int[outWidth * outHeight];

        //    TransBitmapTable.MakeTransTable(transTbl, outWidth, outHeight, centerX, centerY, innerRadius, outerRadius, srcWidth, srcHeight, startAngle, 360, (IntPtr)null);
        //}

        public void clearImpArea()
        {
            impAreaList.Clear();
        }

        public void addImpArea(PolarRect item)
        {
            impAreaList.Add(item);
        }

        public void addImpArea(float angle1, float dist1, float angle2, float dist2)
        {
            addImpArea(new PolarRect(angle1, dist1, angle2, dist2));
        }

        public List<PolarRect> getImpAreaList()
        {
            return impAreaList;
        }
    }

    class Global {
        public static TransBitmap trans = new TransBitmap();
    }
    
}
