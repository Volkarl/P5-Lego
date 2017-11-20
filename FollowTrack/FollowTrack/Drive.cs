using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    public class Drive
    {
        // Public & Path
        public PathPoint[] Path = new PathPoint[PointsOnCurve]; // Size?
        public int CurrentAngle;

        // Const
        private const int MaxNxtCamX = 176;
        private const int MaxNxtCamY = 144;
        private const int PointsOnCurve = 9; //    Points = (value / 2) - 1

        private const int NxtCamHeight = 22;
        private const int FieldOfView = 40;
        private const int CameraAngle = 45;

        private readonly Vector2 _busPoint = new Vector2(MaxNxtCamX / 2, -(FieldOfView / 2)); // TODO: Lav til Const todo: I changed this from hardcoded -20, is it correct?
        private readonly FieldOfViewCorrecter _foV;

        // Old Data
        private Vector2[] _pDataLeftOld = new Vector2[8];
        private Vector2[] _pDataRightOld = new Vector2[8];
        private readonly Vector2[] _lastTwoMidPointsOld = new Vector2[2];

        private bool _isFirstTimeRunning = true;
        private int _boundCountLeft = 0;
        private int _boundCountRight = 0;

        private int _pathCounter = 0;
        private int _pathSize = 0;

        public Drive()
        {
            _foV = new FieldOfViewCorrecter(NxtCamHeight, FieldOfView, FieldOfView, CameraAngle, MaxNxtCamX, MaxNxtCamY);
        }

        // Main
        public void Run()
        {
            if (_pathSize > 0) // Following Path
            {
                Console.WriteLine("\n***************************************************");
                Drive.Turn(CurrentAngle + Path[_pathCounter].Slope);
                Drive.Length(Path[_pathCounter].Length); // Set how ofte Run needs to be called. //
                _pathCounter++;
                _pathSize--;
            }
            else if (!_isFirstTimeRunning) // Finding Path
            {
                // Get & Update New Data
                Vector2[] nxtCamData = new Vector2[8];
                nxtCamData = GetNewDataFromNxtCam(); // Need to balance data, and handle only left side data. //
                //nxtCamData = CorrectFieldOfView(nxtCamData);

                // Update Old Data
                RotateAndDisplaceData(_pDataLeftOld, _pDataRightOld, _lastTwoMidPointsOld);
                //Array.Clear(_lastTwoMidPointsOld,0,2); // TODO: Remove?

                // Sort Left/Right & Combine Old New Data 
                Tuple<Vector2[], Vector2[]> tupleData = SortNxtCamData(nxtCamData);
                Vector2[] dataLeft = new Vector2[8];
                dataLeft = CombineData(tupleData.Item1, _pDataLeftOld);
                Vector2[] dataRight = new Vector2[8];
                dataRight = CombineData(tupleData.Item2, _pDataRightOld);
                _pDataLeftOld = tupleData.Item1;
                _pDataRightOld = tupleData.Item2;

                // Mid Points
                Vector2[] midPoints = new Vector2[8];
                midPoints = CalculatePathMidPoints(
                    CalculateBezierCurvePoints(dataLeft, (PointsOnCurve*2), ref _boundCountLeft),
                    CalculateBezierCurvePoints(dataRight, (PointsOnCurve*2), ref _boundCountRight));

                _lastTwoMidPointsOld[0] = new Vector2(midPoints[0].X, midPoints[0].Y);
                _lastTwoMidPointsOld[1] = new Vector2(midPoints[1].X, midPoints[1].Y);

                // Path Points
                Path = CalculatePathData(midPoints);
                _pathCounter = 0;
                _pathSize = Path.Count(s => s != null);

                ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
                //long memory = GC.GetTotalMemory(true);
                //Console.WriteLine("MEMORY" + memory);
                foreach (var item in midPoints)
                {
                    if (item != null)
                        Console.WriteLine(item.ToString());
                }
                Console.WriteLine("\n\n");
                foreach (var item in Path)
                {
                    if (item != null)
                        Console.WriteLine(item.ToString());
                }
                ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
            }
            else // Finding Path -> First run.
            {
                // If it's first time, we don't combine multiple datasets
                // Get & Update New Data
                Vector2[] nxtCamData = new Vector2[8];
                nxtCamData = GetNewDataFromNxtCam();

                // Correct Data
                //nxtCamData = CorrectFieldOfView(nxtCamData);

                // Sort Left/Right
                Tuple<Vector2[], Vector2[]> tupleData = SortNxtCamData(nxtCamData);
                Vector2[] dataLeft = new Vector2[8];
                dataLeft = CombineData(tupleData.Item1, _pDataLeftOld);
                Vector2[] dataRight = new Vector2[8];
                dataRight = CombineData(tupleData.Item2, _pDataRightOld);
                _pDataLeftOld = tupleData.Item1;
                _pDataRightOld = tupleData.Item2;

                // Mid Points
                Vector2[] midPoints = new Vector2[8];
                midPoints = CalculatePathMidPoints(
                    CalculateBezierCurvePoints(dataLeft, (PointsOnCurve * 2), ref _boundCountLeft),
                    CalculateBezierCurvePoints(dataRight, (PointsOnCurve * 2), ref _boundCountRight));

                _lastTwoMidPointsOld[0] = new Vector2(midPoints[0].X, midPoints[0].Y);
                _lastTwoMidPointsOld[1] = new Vector2(midPoints[1].X, midPoints[1].Y);

                // Path Points
                Path = CalculatePathData(midPoints);
                _isFirstTimeRunning = false;
                _pathSize = Path.Count(s => s != null);


                ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
                foreach (var item in midPoints)
                {
                    if (item != null)
                        Console.WriteLine(item.ToString());
                }
                Console.WriteLine("\n\n");
                foreach (var item in Path)
                {
                    if(item != null)
                        Console.WriteLine(item.ToString());
                }
                ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
            }
        }

        private List<Vector2> CorrectFieldOfView(List<Vector2> nxtCamData)
        {
            return _foV.CalcFloorCoordinates(nxtCamData);
        }

        // TODO: Find relevant Points -> need test
        private double[] CalculateBezierCurvePoints(Vector2[] pData, int pointsOnCurve, ref int boundCount)
        {
            double[] data = new double[pointsOnCurve];

            BezierCurve bc = new BezierCurve();
            bc.Bezier2D(pData, pointsOnCurve / 2, data);


            // TODO: gem kun 1-2 old points, og sort data efter Bezier til kun relevante points.
            double[] dataUpdated = new double[data.Length];
            boundCount = 0;

            for (int i = 0; i < data.Length; i += 2)
            {
                if (data[i] <= MaxNxtCamX && data[i] >= 0 && data[i + 1] <= MaxNxtCamY && data[i + 1] >= 0)
                {
                    dataUpdated[boundCount] = data[i];
                    dataUpdated[boundCount + 1] = data[i + 1];
                    boundCount += 2;
                }
            }
            return dataUpdated;
        }

        // DONE
        private Vector2[] CalculatePathMidPoints(double[] pLeft, double[] pRight)
        {
            Vector2[] midPoints = new Vector2[PointsOnCurve];
            midPoints[0] = _busPoint; // Set busPoint as the first pathpoint.

            //int pCount = pLeft.Length <= pRight.Length ? pLeft.Length : pRight.Length;
            int pCount = _boundCountLeft <= _boundCountRight ? _boundCountLeft : _boundCountRight;
            int midPointCount = 1;

            for (int i = 1; i < pCount - 1; i += 2)
            {
                midPoints[midPointCount] = new Vector2((pLeft[i + 1] + pRight[i + 1]) / 2, (pLeft[i] + pRight[i]) / 2);
                midPointCount++;
            }
            return midPoints;
        }

        // DONE
        private PathPoint[] CalculatePathData(Vector2[] midPoints)
        {
           PathPoint[] pathPoints = new PathPoint[midPoints.Length];

            for (int i = 1; i < midPoints.Length; i++)
            {
                if (midPoints[i] == null)
                    break;

                pathPoints[i-1] = new PathPoint(
                    Math.Atan((midPoints[i].X - midPoints[i - 1].X) / (midPoints[i].Y - midPoints[i - 1].Y)) * 180 / Math.PI,
                    Math.Sqrt(Math.Pow(midPoints[i].X - midPoints[i - 1].X, 2) + Math.Pow(midPoints[i].Y - midPoints[i - 1].Y, 2)));
            }
            return pathPoints;
        }




        //TODO: tilpas til NxtCam
        private int _dataCount = 0;
        private Vector2[] GetNewDataFromNxtCam()
        {
            Vector2[] data = new Vector2[8];
            Array.Clear(data,0,8);

            if (_dataCount == 0)
            {
                data[0] = (new Vector2(12, 0));
                data[1] = (new Vector2(144, 13));
                data[2] = (new Vector2(132, 32));
                data[3] = (new Vector2(12, 19));
                data[4] = (new Vector2(12, 38));
                data[5] = (new Vector2(132, 51));
                data[6] = (new Vector2(12, 57));
                data[7] = (new Vector2(132, 70));

                _dataCount++;
            }
            else if (_dataCount == 1)
            {
                data[0] = (new Vector2(12, 0));
                data[1] = (new Vector2(144, 13));
                data[2] = (new Vector2(132, 32));
                data[3] = (new Vector2(12, 19));
                data[4] = (new Vector2(12, 38));
                data[5] = (new Vector2(132, 51));
                data[6] = (new Vector2(12, 57));
                data[7] = (new Vector2(132, 70));

                _dataCount++;
            }
            else if (_dataCount == 2)
            {
                data[0] = (new Vector2(12, 0));
                data[1] = (new Vector2(144, 13));
                data[2] = (new Vector2(132, 32));
                data[3] = (new Vector2(12, 19));
                data[4] = (new Vector2(12, 38));
                data[5] = (new Vector2(132, 51));
                data[6] = (new Vector2(12, 57));
                data[7] = (new Vector2(132, 70));

                _dataCount++;
            }


            // Handle empty data
            if (data[0] == null)
                data = GetNewDataFromNxtCam(); // vent til næste data fra cam er klar.

            return data;
        }

        // DONE TODO: Lav bedre sorting. maybe?
        public Tuple<Vector2[], Vector2[]> SortNxtCamData(Vector2[] nxtCamData)
        {
            Vector2[] leftPoints = new Vector2[8];
            Vector2[] rightPoints = new Vector2[8];

            int maxIndexY = 0;
            int leftCount = 0;
            int rightCount = 0;
            
            for (int i = 0; i < nxtCamData.Length; i++)
            {
                if (nxtCamData[i].X <= MaxNxtCamX / 2)
                {
                    leftPoints[leftCount] = nxtCamData[i];
                    leftCount++;
                }
                else
                {
                    rightPoints[rightCount] = nxtCamData[i];
                    rightCount++;
                }

                if (nxtCamData[i].Y > nxtCamData[maxIndexY].Y)
                {
                    maxIndexY = i;
                }
            }

            //Array.Sort(leftPoints, (x, y) => y.Y.CompareTo(x.Y));
            //Array.Sort(rightPoints, (x, y) => y.Y.CompareTo(x.Y));

            Array.Sort(leftPoints);
            Array.Sort(rightPoints);

            // Handle unbalanced data
            // Lav trekant;     90 grader, længden mellem Last[] Last[-1] og længden fra 
            if (nxtCamData[maxIndexY].X <= MaxNxtCamX / 2)
            {
                rightPoints[rightCount] = ApproximationOfTheOtherSideOfTheRoad(leftPoints[leftCount-1], leftPoints[leftCount - 2], true, 520);



                //Console.WriteLine("///////////////////////////////////////////////////////////////////////////");
                //Console.WriteLine("x: " + leftPoints[leftPoints.Length - 2].X + "  y: " + leftPoints[leftPoints.Length - 2].Y);
                //Console.WriteLine("x: " + leftPoints[leftPoints.Length - 1].X + "  y: " + leftPoints[leftPoints.Length - 1].Y);

                //Console.WriteLine("x:" + x + "y:" + y);
                //Console.WriteLine("///////////////////////////////////////////////////////////////////////////");
            }
            else
            {
                leftPoints[leftCount] = ApproximationOfTheOtherSideOfTheRoad(rightPoints[rightCount - 1], rightPoints[rightCount - 2], true, 520);

                //Console.WriteLine("///////////////////////////////////////////////////////////////////////////");
                //Console.WriteLine("x: " + rightPoints[rightPoints.Length - 2].X + "  y: " + rightPoints[rightPoints.Length - 2].Y);
                //Console.WriteLine("x: " + rightPoints[rightPoints.Length - 1].X + "  y: " + rightPoints[rightPoints.Length - 1].Y);

                //Console.WriteLine("x:" + x + "y:" + y);
                //Console.WriteLine("///////////////////////////////////////////////////////////////////////////");
            }

            return Tuple.Create(leftPoints, rightPoints);
        }

        // TODO: 100% lort.
        private Vector2[] CombineData(Vector2[] pData, Vector2[] pDataOld)
        {
            Vector2[] data = new Vector2[pData.Length + pDataOld.Length];
            Array.Copy(pDataOld, data, pDataOld.Length);
            Array.Copy(pData, 0, data, pDataOld.Length, pData.Length);

            return data;
        }

        //DONE
        private void RotateAndDisplaceData(Vector2[] dataL, Vector2[] dataR, Vector2[] lastTwoPoints)
        {
            int rotationDirection;

            if (lastTwoPoints[0].X < lastTwoPoints[1].X) // we are turning clockwise, soo rotate counterclockwise -1 is cloclwise and 1 is counterclockwise
            {
                rotationDirection = 1;
            }
            else //we are turning counterclockwise so turn clockwise
            {
                rotationDirection = -1;
            }

            double rotationSumInDegrees = Math.Atan(Math.Abs((lastTwoPoints[0].X - lastTwoPoints[1].X)) / Math.Abs(lastTwoPoints[0].Y - lastTwoPoints[1].Y)); // math.abs is the absolute value e.g always positive
            rotationSumInDegrees = rotationSumInDegrees * rotationDirection;
            // for (int i = 0; i < 8; i+=2)
            int i = 0;

            while (dataL.Length-1 >= i )
            {
                if (dataL[i] != null)
                {
                    double tempXValue = dataL[i].X; // we will override x value, but still need original when rotating y
                    double tempYValue = dataL[i].Y; // i dont think this is needed but it makes it pretty

                    /*
                     * take care
                     * what way is it rotating?
                     * if the bus has rotated clockwise
                     * rotate the cordinats counterclockwise
                     */
                    dataL[i].X = tempXValue * Math.Cos(rotationSumInDegrees) - tempYValue * Math.Sin(rotationSumInDegrees); // rotation
                    dataL[i].Y = tempXValue * Math.Sin(rotationSumInDegrees) + tempYValue * Math.Cos(rotationSumInDegrees);
                }
                i += 1;
            }
            i = 0;

            while (dataR.Length-1 >= i )
            {
                if (dataL[i] != null)
                {

                    double tempXValue = dataR[i].X; // we will override x value, but still need original when rotating y
                    double tempYValue = dataR[i].Y; // i dont think this is needed but it makes it pretty

                    /*
                     * take care
                     * what way is it rotating?
                     * if the bus has rotated clockwise
                     * rotate the cordinats counterclockwise
                     */
                    dataR[i].X = tempXValue * Math.Cos(rotationSumInDegrees) -
                                 tempYValue * Math.Sin(rotationSumInDegrees); // rotation
                    dataR[i].Y = tempXValue * Math.Sin(rotationSumInDegrees) +
                                 tempYValue * Math.Cos(rotationSumInDegrees);
                }
                i += 1;
            }
            i = 0;
            
            double tempXValue2 = lastTwoPoints[0].X; // we will override x value, but still need original when rotating y
            double tempYValue2 = lastTwoPoints[0].Y; // i dont think this is needed but it makes it pretty

            lastTwoPoints[1].X = tempXValue2 * Math.Cos(rotationSumInDegrees) - tempYValue2 * Math.Sin(rotationSumInDegrees); // rotation
            lastTwoPoints[1].Y = tempXValue2 * Math.Sin(rotationSumInDegrees) + tempYValue2 * Math.Cos(rotationSumInDegrees);


            /*
             * Set end point to startpoint cordinats,
             * all start points must be at the same spot in the graph 
             */
            double displacementX = _busPoint.X - lastTwoPoints[1].X; //after endpoint has been rotated
            double displacementY = _busPoint.Y - lastTwoPoints[1].Y;

            /*
             * lastly we displace all of the cordinats
             */
            // for (int i = 0; i < 8; i+=2)
            while (dataL.Length-1 >= i )
            {
                if (dataL[i] != null)
                {
                    dataL[i].X = dataL[i].X + displacementX;
                    dataL[i].Y = dataL[i].Y + displacementY;
                }
                i += 1;
            }
            i = 0;
            // for (int i = 0; i < 8; i += 2)
            while (dataR.Length-1 >= i )
            {
                if (dataL[i] != null)
                {
                    dataR[i].X = dataR[i].X + displacementX;
                    dataR[i].Y = dataR[i].Y + displacementY;
                }
                i += 1;
            }

            /*
             * we new have the new old cordinats 
             * override the old old cordinats, and its done
             */

        }

        /*
         * leftOrRightSide, true if last point if on the right side false if left side
         * distance is the distance to the other side of the road
         */

        private Vector2 ApproximationOfTheOtherSideOfTheRoad(Vector2 lastPoint1, Vector2 lastPoint2, bool leftOrRightSide, double distance) // right side should give negative number left positive
        {
            Vector2 vectorBetweenTheTwoLastPoints = new Vector2(Math.Abs(lastPoint1.X-lastPoint2.X),Math.Abs(lastPoint1.Y-lastPoint2.Y)); // calculation of the vector between the two points
            double orthogonalY = (vectorBetweenTheTwoLastPoints.X * (leftOrRightSide ? -1 : 1)) / vectorBetweenTheTwoLastPoints.Y; // calculation of the ortogonal vector
            double multiplyer = distance / Math.Sqrt(1 * 1 + Math.Pow(orthogonalY,2)); // calculation of the multiplyer required for the ortogonal vector to be distance long
            Vector2 k = new Vector2(lastPoint2.X+ (leftOrRightSide ? -1 : 1) * multiplyer, lastPoint2.X +orthogonalY * multiplyer); // addition of the ortogonalvector times the multiplyer, added to the last point

            return k;
        }







        #region Helper functions

        private static void Turn(double v)
        {

            Console.WriteLine("The Bus turned " + v + "degrees.");
        }

        private static void Length(double p)
        {

            Console.WriteLine("The Bus drove " + p + "km.");

        }

        #endregion


    }
}
