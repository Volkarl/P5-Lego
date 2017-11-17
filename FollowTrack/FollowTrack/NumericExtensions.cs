using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    public static class NumericExtensions
    {
        // Class name copied from https://stormconsultancy.co.uk/blog/development/code-snippets/convert-an-angle-in-degrees-to-radians-in-c/

        // Copied from http://www.vcskicks.com/csharp_net_angles.php
        public static double RadianToDegree(this double angle)
        {
            return angle * (180.0 / Math.PI);
        }

        // Copied from http://www.vcskicks.com/csharp_net_angles.php
        public static double DegreeToRadian(this double angle)
        {
            return Math.PI * angle / 180.0;
        }
    }
}
