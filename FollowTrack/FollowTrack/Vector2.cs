using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    public class Vector2
    {
        public double X { get { return X; } set { X = value; } }
        public double Y { get { return Y; } set { Y = value; } }

        public Vector2(double x, double y)
        {
            X = x;
            Y = y;
        }

       
        public override string ToString()
        {
            return "X: " + X.ToString("0.00") + "\t" + "Y:" + Y.ToString("0.00");
        }
    }
}
