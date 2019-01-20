using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WideViewer
{
    public class PolarCoord
    {
        public float angle;
        public float dist;

        public PolarCoord(float angle, float dist) 
        {
            this.angle = angle;
            this.dist = dist;
        }
    }

    public class XYCoord
    {
        public float x, y;

        public XYCoord(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    }

    public class PolarRect
    {
        public PolarCoord start;
        public PolarCoord end;

        public void set(PolarCoord start, PolarCoord end)
        {
            this.start = new PolarCoord(start.angle, start.dist);
            this.end = new PolarCoord(end.angle, end.dist);
        }

        public void set(float angle1, float dist1, float angle2, float dist2)
        {
            set(new PolarCoord(angle1, dist1), new PolarCoord(angle2, dist2));
        }

        public PolarRect(float angle1, float dist1, float angle2, float dist2)
        {
            set(angle1, dist1, angle2, dist2);
        }

        public PolarRect(PolarCoord start, PolarCoord end)
        {
            set(start.angle, start.dist, end.angle, end.dist);
        }

        public PolarRect(PolarRect src)
        {
            set(src.start, src.end);
        }
    }

    public class PolarUtil
    {
        public static XYCoord TransPolarToXY(float angle, float dist)
        {
            float x = (float)Math.Cos(angle * Math.PI / 180) * dist;
            float y = (float)Math.Sin(angle * Math.PI / 180) * dist;

            return new XYCoord(x, y);
        }

        public static XYCoord TransPolarToXY(PolarCoord coord)
        {
            return TransPolarToXY(coord.angle, coord.dist);
        }

        public static XYCoord TransPolarToXY(float angle, float dist, int centerX, int centerY)
        {
            XYCoord coord = TransPolarToXY(angle, dist);
            coord.x += centerX;
            coord.y += centerY;

            return coord;
        }

        public static PolarCoord TransXYToPolar(float x, float y)
        {
            float angle = (float)(Math.Atan2(y, x) * 180 / Math.PI);
            if (angle < 0) angle += 360;
            float dist = (float)Math.Sqrt((x * x) + (y * y));

            return new PolarCoord(angle, dist);
        }

        public static PolarCoord TransXYToPolar(XYCoord coord)
        {
            return TransXYToPolar(coord.x, coord.y);
        }
    }
}
