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
        public List<PathPoint> Path = new List<PathPoint>();
        public int CurrentAngle;

        // Const
        private const int MaxNxtCamX = 176;
        private const int MaxNxtCamY = 144;
        private const int PointsOnCurve = 18; //    Points = (value / 2) - 1
        private readonly Vector2 _busPoint = new Vector2(MaxNxtCamX / 2, MaxNxtCamY-MaxNxtCamY); // TODO: Lav til Const

        // Old Data
        private List<Vector2> _pDataLeftOld = new List<Vector2>();
        private List<Vector2> _pDataRightOld = new List<Vector2>();
        private readonly List<Vector2> _lastTwoMidPointsOld = new List<Vector2>();

        private bool _isFirstTimeRunning = true;
        private int _boundCountLeft = 0;
        private int _boundCountRight = 0;


        // Main
        public void Run()
        {
            if (Path.Count > 0) // Following Path
            {
                Console.WriteLine("\n***************************************************");
                Drive.Turn(CurrentAngle + Path[0].Slope);
                Drive.Length(Path[0].Length); // Set how ofte Run needs to be called. //
                Path.RemoveAt(0);
            }
            else if (!_isFirstTimeRunning) // Finding Path
            {
                // Get & Update New Data
                List<Vector2> nxtCamData = GetNewDataFromNxtCam(); // Need to balance data, and handle only left side data. //
                //ConvertDataFromFieldOfView(nxtCamData);

                // Update Old Data
                RotateAndDisplaceData(_pDataLeftOld, _pDataRightOld, _lastTwoMidPointsOld);
                _lastTwoMidPointsOld.RemoveRange(0,2);

                // Sort Left/Right & Combine Old New Data 
                Tuple<List<Vector2>, List<Vector2>> tupleData = SortNxtCamData(nxtCamData);
                List<Vector2> dataLeft = CombineData(tupleData.Item1, _pDataLeftOld);
                List<Vector2> dataRight = CombineData(tupleData.Item2, _pDataRightOld);
                _pDataLeftOld = tupleData.Item1;
                _pDataRightOld = tupleData.Item2;

                // Mid Points
                List<Vector2> midPoints = CalculatePathMidPoints(
                    CalculateBezierCurvePoints(dataLeft, (PointsOnCurve*2), ref _boundCountLeft),
                    CalculateBezierCurvePoints(dataRight, (PointsOnCurve*2), ref _boundCountRight));
                _lastTwoMidPointsOld.Add(new Vector2(midPoints[midPoints.Count - 2].X, midPoints[midPoints.Count - 2].Y));
                _lastTwoMidPointsOld.Add(new Vector2(midPoints[midPoints.Count - 1].X, midPoints[midPoints.Count - 1].Y));

                // Path Points
                Path = CalculatePathData(midPoints);


                ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
                //long memory = GC.GetTotalMemory(true);
                //Console.WriteLine("MEMORY" + memory);
                foreach (var item in midPoints)
                {
                    Console.WriteLine(item.ToString());
                }
                Console.WriteLine("\n\n");
                foreach (var item in Path)
                {
                    Console.WriteLine(item.ToString());
                }
                //foreach (var item in _lastTwoMidPointsOld)
                //{
                //    Console.WriteLine(item);
                //}
                ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
            }
            else // Finding Path -> First run.
            {
                // Get & Update New Data
                List<Vector2> nxtCamData = GetNewDataFromNxtCam();
                //ConvertDataFromFieldOfView(nxtCamData);

                // Sort Left/Right
                Tuple<List<Vector2>, List<Vector2>> tupleData = SortNxtCamData(nxtCamData);
                List<Vector2> dataLeft = CombineData(tupleData.Item1, _pDataLeftOld);
                List<Vector2> dataRight = CombineData(tupleData.Item2, _pDataRightOld);
                _pDataLeftOld = tupleData.Item1;
                _pDataRightOld = tupleData.Item2;

                // Mid Points
                List<Vector2> midPoints = CalculatePathMidPoints(
                    CalculateBezierCurvePoints(dataLeft, PointsOnCurve, ref _boundCountLeft),
                    CalculateBezierCurvePoints(dataRight, PointsOnCurve, ref _boundCountRight));
                _lastTwoMidPointsOld.Add(new Vector2(midPoints[midPoints.Count - 2].X, midPoints[midPoints.Count - 2].Y));
                _lastTwoMidPointsOld.Add(new Vector2(midPoints[midPoints.Count - 1].X, midPoints[midPoints.Count - 1].Y));

                // Path Points
                Path = CalculatePathData(midPoints);
                _isFirstTimeRunning = false;


                ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
                foreach (var item in midPoints)
                {
                    Console.WriteLine(item.ToString());
                }
                Console.WriteLine("\n\n");
                foreach (var item in Path)
                {
                    Console.WriteLine(item.ToString());
                }
                //foreach (var item in _lastTwoMidPointsOld)
                //{
                //    Console.WriteLine(item);
                //}
                ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
            }
        }

        // TODO: Find relevant Points -> need test
        private double[] CalculateBezierCurvePoints(List<Vector2> pData, int pointsOnCurve, ref int boundCount)
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
        private List<Vector2> CalculatePathMidPoints(double[] pLeft, double[] pRight)
        {
            List<Vector2> midPoints = new List<Vector2>();
            midPoints.Add(_busPoint); // Set busPoint as the first pathpoint.

            //int pCount = pLeft.Length <= pRight.Length ? pLeft.Length : pRight.Length;
            int pCount = _boundCountLeft <= _boundCountRight ? _boundCountLeft : _boundCountRight;

            for (int i = 1; i != pCount - 1; i += 2)
            {
                midPoints.Add(new Vector2((pLeft[i + 1] + pRight[i + 1]) / 2, (pLeft[i] + pRight[i]) / 2));
            }
            return midPoints;
        }

        // DONE
        private List<PathPoint> CalculatePathData(List<Vector2> midPoints)
        {
            List<PathPoint> pathPoints = new List<PathPoint>();

            for (int i = 1; i < midPoints.Count; i++)
            {
                pathPoints.Add(new PathPoint(
                    Math.Atan((midPoints[i].X - midPoints[i - 1].X) / (midPoints[i].Y - midPoints[i - 1].Y)) * 180 / Math.PI,
                    Math.Sqrt(Math.Pow(midPoints[i].X - midPoints[i - 1].X, 2) + Math.Pow(midPoints[i].Y - midPoints[i - 1].Y, 2))));
            }
            return pathPoints;
        }




        //TODO: tilpas til NxtCam
        private int _dataCount = 0;
        private List<Vector2> GetNewDataFromNxtCam()
        {
            List<Vector2> data = new List<Vector2>();
            data.Clear();

            if (_dataCount == 0)
            {

                data.Add(new Vector2(12, 0));
                data.Add(new Vector2(144, 13));
                data.Add(new Vector2(132, 32));
                data.Add(new Vector2(12, 19));
                data.Add(new Vector2(12, 38));
                data.Add(new Vector2(132, 51));
                data.Add(new Vector2(12, 57));
                data.Add(new Vector2(132, 70));

                _dataCount++;
            }
            else if (_dataCount == 1)
            {
                //data.Add(new Vector2(MaxNxtCamX, MaxNxtCamY));
                //data.Add(new Vector2(MaxNxtCamX, MaxNxtCamY));
                //data.Add(new Vector2(MaxNxtCamX, MaxNxtCamY));
                //data.Add(new Vector2(MaxNxtCamX, MaxNxtCamY));
                data.Add(new Vector2(12, 0));
                data.Add(new Vector2(144, 13));
                data.Add(new Vector2(132, 32));
                data.Add(new Vector2(12, 19));
                data.Add(new Vector2(12, 38));
                data.Add(new Vector2(132, 51));
                data.Add(new Vector2(12, 57));
                data.Add(new Vector2(132, 70));

                _dataCount++;
            }
            else if (_dataCount == 2)
            {
                data.Add(new Vector2(12, 0));
                data.Add(new Vector2(144, 13));
                data.Add(new Vector2(132, 32));
                data.Add(new Vector2(12, 19));
                data.Add(new Vector2(12, 38));
                data.Add(new Vector2(132, 51));
                data.Add(new Vector2(12, 57));
                data.Add(new Vector2(132, 70));

                _dataCount++;
            }


            // Handle empty data
            if (data.Count == 0)
                data = GetNewDataFromNxtCam(); // vent til næste data fra cam er klar.

            return data;
        }

        // DONE TODO: Lav bedre sorting. maybe?
        public Tuple<List<Vector2>, List<Vector2>> SortNxtCamData(List<Vector2> nxtCamData)
        {
            List<Vector2> leftPoints = new List<Vector2>();
            List<Vector2> rightPoints = new List<Vector2>();

            int maxIndexY = 0;

            for (int i = 0; i < nxtCamData.Count; i++)
            {
                if (nxtCamData[i].X <= MaxNxtCamX / 2)
                    leftPoints.Add(nxtCamData[i]);
                else
                    rightPoints.Add(nxtCamData[i]);

                if (nxtCamData[i].Y > nxtCamData[maxIndexY].Y)
                {
                    maxIndexY = i;
                }
            }

            leftPoints = leftPoints.OrderBy(p => p.Y).ToList();
            rightPoints = rightPoints.OrderBy(p => p.Y).ToList();

            // Handle unbalanced data
            // Lav trekant;     90 grader, længden mellem Last[] Last[-1] og længden fra 
            if (nxtCamData[maxIndexY].X <= MaxNxtCamX / 2)
            {
                double AB = Math.Sqrt(
                    Math.Pow(leftPoints[leftPoints.Count - 1].Y - leftPoints[leftPoints.Count - 2].Y, 2) + 
                    Math.Pow(leftPoints[leftPoints.Count - 1].X - leftPoints[leftPoints.Count - 2].X, 2));
                double AC = 120; // Bredde af track lane
                double BC = Math.Sqrt(Math.Pow(AB, 2) + Math.Pow(AC, 2));


                double y = (Math.Pow(AB, 2) + Math.Pow(AC, 2) - Math.Pow(BC, 2)) / 2 * AB;
                double x = Math.Sqrt(Math.Pow(AC, 2) - Math.Pow(y, 2));

                rightPoints.Add(new Vector2(x, y));

                Console.WriteLine("///////////////////////////////////////////////////////////////////////////");
                Console.WriteLine("x: " + leftPoints[leftPoints.Count - 2].X + "  y: " + leftPoints[leftPoints.Count - 2].Y);
                Console.WriteLine("x: " + leftPoints[leftPoints.Count - 1].X + "  y: " + leftPoints[leftPoints.Count - 1].Y);

                Console.WriteLine("x:" + x + "y:" + y);
                Console.WriteLine("///////////////////////////////////////////////////////////////////////////");
            }
            else
            {
                double AB = Math.Sqrt(
                    Math.Pow(rightPoints[rightPoints.Count - 1].X - rightPoints[rightPoints.Count - 2].X, 2) +
                    Math.Pow(rightPoints[rightPoints.Count - 1].Y - rightPoints[rightPoints.Count - 2].Y, 2));

                double AC = 52; // Bredde af track lane
                double BC = Math.Sqrt(Math.Pow(AB, 2) + Math.Pow(AC, 2));




                //double y = (Math.Pow(AB, 2) + Math.Pow(AC, 2) - Math.Pow(BC, 2)) / 2 * AB;
                //double x = Math.Sqrt(Math.Pow(AC, 2) - Math.Pow(y, 2));

                double y = (Math.Pow(AB, 2) + Math.Pow(AC, 2) - Math.Pow(BC, 2)) / 2 * AB;
                double x = Math.Sqrt(Math.Pow(AC, 2) - Math.Pow(y, 2));




                leftPoints.Add(new Vector2(x,y));

                Console.WriteLine("///////////////////////////////////////////////////////////////////////////");
                Console.WriteLine("x: " + rightPoints[rightPoints.Count - 2].X + "  y: " + rightPoints[rightPoints.Count - 2].Y);
                Console.WriteLine("x: " + rightPoints[rightPoints.Count - 1].X + "  y: " + rightPoints[rightPoints.Count - 1].Y);

                Console.WriteLine("x:" + x + "y:" + y);
                Console.WriteLine("///////////////////////////////////////////////////////////////////////////");
            }


            //foreach (Vector2 point in nxtCamData)
            //{
            //    double maxValueY = double.MinValue;
            //    double maxValueX = 0;
            //    Vector2 maxPoint;

            //    if (point.X <= MaxNxtCamX / 2)
            //        leftPoints.Add(point);
            //    else
            //        rightPoints.Add(point);

            //    if (point.Y > maxValueY)
            //    {
            //        maxValueY = point.Y;
            //        maxValueX = point.X;
            //    }
            //}



            return Tuple.Create(leftPoints, rightPoints);
        }

        // TODO: 100% lort.
        private List<Vector2> CombineData(List<Vector2> pData, List<Vector2> pDataOld)
        {
            List<Vector2> data = new List<Vector2>();
            data.AddRange(pDataOld);
            data.AddRange(pData);

            //return data.OrderBy(p => p.Y).ToList();
            return data;
        }








        //DONE?!?
        private void RotateAndDisplaceData(List<Vector2> dataL, List<Vector2> dataR, List<Vector2> lastTwoPoints)
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

            while (dataL.Count > i + 1)
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
                i += 2;
            }
            i = 0;

            while (dataR.Count > i + 1)
            {
                double tempXValue = dataR[i].X; // we will override x value, but still need original when rotating y
                double tempYValue = dataR[i].Y; // i dont think this is needed but it makes it pretty

                /*
                 * take care
                 * what way is it rotating?
                 * if the bus has rotated clockwise
                 * rotate the cordinats counterclockwise
                 */
                dataR[i].X = tempXValue * Math.Cos(rotationSumInDegrees) - tempYValue * Math.Sin(rotationSumInDegrees); // rotation
                dataR[i].Y = tempXValue * Math.Sin(rotationSumInDegrees) + tempYValue * Math.Cos(rotationSumInDegrees);
                i += 2;
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
            double displacementX = 88 - lastTwoPoints[1].X; //after endpoint has been rotated
            double displacementY = (-20) - lastTwoPoints[1].Y;

            /*
             * lastly we displace all of the cordinats
             */
            // for (int i = 0; i < 8; i+=2)
            while (dataL.Count > i + 1)
            {
                dataL[i].X = dataL[i].X + displacementX;
                dataL[i].Y = dataL[i].Y + displacementY;
                i += 2;
            }
            i = 0;
            // for (int i = 0; i < 8; i += 2)
            while (dataR.Count > i + 1)
            {
                dataR[i].X = dataR[i].X + displacementX;
                dataR[i].Y = dataR[i].Y + displacementY;
                i += 2;
            }

            /*
             * we new have the new old cordinats 
             * override the old old cordinats, and its done
             */

        }

        private void ConvertDataFromFieldOfView(double[] data)
        {
            throw new NotImplementedException();
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

        // Done
        private static double[] RemoveAt(double[] source, int index)
        {
            double[] dest = new double[source.Length - 1];
            if (index > 0)
                Array.Copy(source, 0, dest, 0, index);

            if (index < source.Length - 1)
                Array.Copy(source, index + 1, dest, index, source.Length - index - 1);

            return dest;
        }


        #endregion












        // Remove ???
        //private double CalculateAvgDistanceBetweenPoints()
        //{
        //    double distance;
        //    double avgdistance = 0;

        //    for (int i = 1; i != PointsOnCurve - 1; i += 2)
        //    {
        //        distance = Math.Sqrt(Math.Pow(_pLeft[i + 1] - _pRight[i + 1], 2) + Math.Pow(_pLeft[i] - _pRight[i], 2));
        //        avgdistance += distance;

        //        //Test
        //        //Console.WriteLine("x:" + (int)_pLeft[i + 1] + "\t y:" + (int)_pLeft[i] + "\t Afstand:" + distance);
        //        //Console.WriteLine("x:" + (int)_pRight[i + 1] + "\t y:" + (int)_pRight[i]);
        //    }
        //    return avgdistance / ((PointsOnCurve / 2) - 1);
        //}




    }









}
