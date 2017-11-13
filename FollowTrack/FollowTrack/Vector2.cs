using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    public class Vector2
    {
        public double X { get;}
        public double Y { get;}

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
