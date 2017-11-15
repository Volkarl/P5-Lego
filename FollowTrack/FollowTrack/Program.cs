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
            Console.WriteLine("Running path!!!");
            d.Run(); // 1
            d.Run(); // 2
            d.Run(); // 3
            d.Run(); // 4
            d.Run(); // 5
            d.Run(); // 6
            d.Run(); // 7
            d.Run(); // 8
            Console.ReadKey();
            Console.WriteLine("Running 2nd Data!!!");
            // 2nd Data set
            d.Run();
            Console.ReadKey();
            Console.WriteLine("Running path!!!");
            d.Run(); // 1
            d.Run(); // 2
            d.Run(); // 3
            d.Run(); // 4
            d.Run(); // 5
            d.Run(); // 6
            d.Run(); // 7
            d.Run(); // 8
            Console.ReadKey();
        }
    }
}
