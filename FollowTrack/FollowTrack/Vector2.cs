using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    public class Vector2 : IComparable
    {
        public double X { get; set; }
        public double Y { get; set; }

        public Vector2(double x, double y)
        {
            X = x;
            Y = y;
        }
  
        public override string ToString()
        {
            return "X: " + X.ToString("0.00") + "\t" + "Y:" + Y.ToString("0.00");
        }



        public int CompareTo(object obj)
        {
            if (obj == null) return 1;

            Vector2 otherTemperature = obj as Vector2;
            if (otherTemperature != null)
                return this.Y.CompareTo(otherTemperature.Y);
            else
                throw new ArgumentException("Object is not a Vector2");
        }
    }
}
