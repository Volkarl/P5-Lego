using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    class Program
    {
        static void Main(string[] args)
        {
            Drive d = new Drive();

            // 1st Data set
            d.Run();
            Console.ReadKey();

            // 2nd Data set
            d.Run();
            Console.ReadKey();
        }
    }
}
