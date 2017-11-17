using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using FollowTrack;

namespace FollowTrackTests
{
    /// <summary>
    /// A test class for ...
    /// </summary>
    [TestFixture]
    public class FieldOfViewCorrecterTests
    {

        private FieldOfViewCorrecter _foV = new FieldOfViewCorrecter(22, 40, 40, 45);

        //#region Setup and Tear down
        ///// <summary>
        ///// This runs only once at the beginning of all tests and is used for all tests in the 
        ///// class.
        ///// </summary>
        //[TestFixtureSetUp]
        //public void InitialSetup()
        //{

        //}

        ///// <summary>
        ///// This runs only once at the end of all tests and is used for all tests in the class.
        ///// </summary>
        //[TestFixtureTearDown]
        //public void FinalTearDown()
        //{

        //}

        ///// <summary>
        ///// This setup funcitons runs before each test method
        ///// </summary>
        //[SetUp]
        //public void SetupForEachTest()
        //{
        //}

        ///// <summary>
        ///// This setup funcitons runs after each test method
        ///// </summary>
        //[TearDown]
        //public void TearDownForEachTest()
        //{
        //}
        //#endregion

        [TestCase(0, 100, ExpectedResult = 10.26)]
        [TestCase(50, 100, ExpectedResult = 22)]
        [TestCase(100, 100, ExpectedResult = 47.18)]
        public double CalcFloorCoordinateY_Integer_IsCorrect(double y, double maxY)
        {
            double floorY = _foV.CalcFloorCoordinateY(y, maxY);
            return Math.Round(floorY, 2);
        }

        [TestCase(0, 100, 0, 100, ExpectedResult = -8.835)]  // To the very left
        [TestCase(50, 100, 0, 100, ExpectedResult = 0)]    // Middle
        [TestCase(100, 100, 0, 100, ExpectedResult = 8.835)]// To the very right
        public double CalcFloorCoordinateX_Integer_IsCorrect(double x, double maxX, double y, double maxY)
        {
            double floorY = _foV.CalcFloorCoordinates(x, y, maxX, maxY).Item2;
            return Math.Round(floorY, 3);
        }

        //todo Test med: Decimaltal, en anderledes maxX og maxY
    }
}
