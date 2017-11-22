using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using FollowTrack;

namespace FollowTrackTests
{
    [TestFixture]
    public class FieldOfViewCorrecterTests
    {
        // All expected results have been calculated with mathcad
        private FieldOfViewCorrecter CreateFovCorrecter(double cameraHeight = 22, double fieldOfViewXdeg = 40, double fieldOfViewYdeg = 40, double cameraAngle = 45, double maxPossibleX = 100, double maxPossibleY =100)
        {
            return new FieldOfViewCorrecter(cameraHeight, fieldOfViewXdeg, fieldOfViewYdeg, cameraAngle, maxPossibleX, maxPossibleY);
        }

        #region Integer
        [TestCase(0, ExpectedResult = 10.26)]
        [TestCase(50, ExpectedResult = 22)]
        [TestCase(100, ExpectedResult = 47.18)]
        public double CalcFloorCoordinateY_Integer_IsCorrect(double y)
        {
            double floorY = CreateFovCorrecter().CalcFloorCoordinateY(y);
            return Math.Round(floorY, 2);
        }

        [TestCase(0, 0, ExpectedResult = -8.835)]  // To the very left
        [TestCase(50, 0, ExpectedResult = 0)]    // Middle
        [TestCase(100, 0, ExpectedResult = 8.835)]// To the very right
        public double CalcFloorCoordinateX_Integer_IsCorrect(double x, double y)
        {
            double floorX = CreateFovCorrecter().CalcFloorCoordinates(x, y).Item1;
            return Math.Round(floorX, 3);
        }
        #endregion

        #region Decimal
        [TestCase(0.33, ExpectedResult = 10.321)]
        [TestCase(50.5, ExpectedResult = 22.154)]
        [TestCase(99.99, ExpectedResult = 47.171)]
        public double CalcFloorCoordinateY_Decimal_IsCorrect(double y)
        {
            double floorY = CreateFovCorrecter().CalcFloorCoordinateY(y);
            return Math.Round(floorY, 3);
        }

        [TestCase(0.33, 0, ExpectedResult = -8.772)]  // To the very left
        [TestCase(50.5, 0, ExpectedResult = 0.085)]    // Middle
        [TestCase(99.99, 0, ExpectedResult = 8.833)]// To the very right
        public double CalcFloorCoordinateX_Decimal_IsCorrect(double x, double y)
        {
            double floorX = CreateFovCorrecter().CalcFloorCoordinates(x, y).Item1;
            return Math.Round(floorX, 3);
        }
        #endregion

        #region DifferentMax
        [TestCase(0, 176, ExpectedResult = 10.259)]
        [TestCase(50, 177, ExpectedResult = 16.16)]
        [TestCase(100, 178, ExpectedResult = 23.985)]
        public double CalcFloorCoordinateY_DifferentMax_IsCorrect(double y, double maxY)
        {
            double floorY = CreateFovCorrecter(maxPossibleY:maxY).CalcFloorCoordinateY(y);
            return Math.Round(floorY, 3);
        }

        [TestCase(1, 176, 0, ExpectedResult = -8.726)]
        [TestCase(50, 177, 0, ExpectedResult = -3.715)]
        [TestCase(100, 178, 0, ExpectedResult = 1.048)]
        public double CalcFloorCoordinateX_DifferentMax_IsCorrect(double x, double maxX, double y)
        {
            double floorX = CreateFovCorrecter(maxPossibleX:maxX).CalcFloorCoordinates(x, y).Item1;
            return Math.Round(floorX, 3);
        }
        #endregion

        #region NxtCamData->Coordinate Test

        [Test]
        public void CorrectFieldOfView_TopLeft_IsCorrect()
        {
            Vector2[] topLeft = CreateFovCorrecter(maxPossibleX:176, maxPossibleY:144)
                .CalcFloorCoordinates(new[] {new Vector2(0, 0)});
            Assert.AreEqual(-18, (int) topLeft[0].X);
            Assert.AreEqual(47, (int) topLeft[0].Y);
        }

        [Test]
        public void CorrectFieldOfView_BottomRight_IsCorrect()
        {
            Vector2[] topLeft = CreateFovCorrecter(maxPossibleX: 176, maxPossibleY: 144)
                .CalcFloorCoordinates(new[] { new Vector2(176, 144) });
            Assert.AreEqual(8, (int) topLeft[0].X); // Minimum x value when the y value is lowest 
            Assert.AreEqual(10, (int) topLeft[0].Y); 
        }

        [Test]
        public void CorrectFieldOfView_BottomMid_IsCorrect()
        {
            Vector2[] topLeft = CreateFovCorrecter(maxPossibleX: 176, maxPossibleY: 144)
                .CalcFloorCoordinates(new[] { new Vector2(176 / 2, 144) });
            Assert.AreEqual(0, (int) topLeft[0].X);
            Assert.AreEqual(10, (int) topLeft[0].Y);
        }
        #endregion
    }
}
