using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    public class PathPoint
    {
        public double Slope { get; }
        public double Length { get; }

        public PathPoint(double slope, double length)
        {
            Slope = slope;
            Length = length;
        }


        public override string ToString()
        {
            return "a:" + Slope.ToString("0.00") + "\t\tLength:" + Length.ToString("0.00"); 
        }
    }
}
