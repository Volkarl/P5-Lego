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
        private readonly Vector2 _busPoint = new Vector2(MaxNxtCamX / 2, MaxNxtCamY - MaxNxtCamY); // TODO: Lav til Const

        // Old Data
        private List<Vector2> _pDataLeftOld = new List<Vector2>();
        private List<Vector2> _pDataRightOld = new List<Vector2>();
        private readonly List<Vector2> _lastTwoMidPointsOld = new List<Vector2>();

        private bool _isFirstTimeRunning = true;

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
                double[] dataLeft = CombineData(tupleData.Item1, _pDataLeftOld);
                double[] dataRight = CombineData(tupleData.Item2, _pDataRightOld);
                _pDataLeftOld = tupleData.Item1;
                _pDataRightOld = tupleData.Item2;

                // Mid Points
                List<Vector2> midPoints = CalculatePathMidPoints(
                    CalculateBezierCurvePoints(dataLeft, PointsOnCurve*2+2), 
                    CalculateBezierCurvePoints(dataRight, PointsOnCurve*2+2));
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
                double[] dataLeft = CombineData(tupleData.Item1, _pDataLeftOld);
                double[] dataRight = CombineData(tupleData.Item2, _pDataRightOld);
                _pDataLeftOld = tupleData.Item1;
                _pDataRightOld = tupleData.Item2;

                // Mid Points
                List<Vector2> midPoints = CalculatePathMidPoints(
                    CalculateBezierCurvePoints(dataLeft, PointsOnCurve), 
                    CalculateBezierCurvePoints(dataRight, PointsOnCurve));
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
        private double[] CalculateBezierCurvePoints(double[] pData, int pointsOnCurve)
        {
            double[] data = new double[pointsOnCurve];

            BezierCurve bc = new BezierCurve();
            bc.Bezier2D(pData, pointsOnCurve / 2, data);


            // TODO: gem kun 1-2 old points, og sort data efter Bezier til kun relevante points.
            List<double> dataUpdated = new List<double>();

            for (int i = 0; i < data.Length; i +=2)
            {
                if (data[i] <= MaxNxtCamX && data[i] >= 0 && data[i + 1] <= MaxNxtCamY && data[i + 1] >= 0)
                {
                    dataUpdated.Add(data[i]);
                    dataUpdated.Add(data[i+1]);
                }
            }
            return dataUpdated.ToArray();
        }

        // DONE
        private List<Vector2> CalculatePathMidPoints(double[] pLeft, double[] pRight)
        {
            List<Vector2> midPoints = new List<Vector2>();
            midPoints.Add(_busPoint); // Set busPoint as the first pathpoint.

            int pCount = pLeft.Length <= pRight.Length ? pLeft.Length : pRight.Length;

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
            if (_dataCount == 0)
            {
                List<Vector2> data = new List<Vector2>();
                data.Add(new Vector2(12, 0));
                data.Add(new Vector2(144, 13));
                data.Add(new Vector2(132, 32));
                data.Add(new Vector2(12, 19));
                data.Add(new Vector2(12, 38));
                data.Add(new Vector2(132, 51));
                data.Add(new Vector2(12, 57));
                data.Add(new Vector2(132, 70));

                _dataCount++;
                return data;
            }
            else if (_dataCount == 1)
            {
                List<Vector2> data = new List<Vector2>();
                data.Add(new Vector2(12, 0));
                data.Add(new Vector2(144, 13));
                data.Add(new Vector2(132, 32));
                data.Add(new Vector2(12, 19));
                data.Add(new Vector2(12, 38));
                data.Add(new Vector2(132, 51));
                data.Add(new Vector2(12, 57));
                data.Add(new Vector2(132, 70));

                _dataCount++;
                return data;
            }
            else if (_dataCount == 2)
            {
                List<Vector2> data = new List<Vector2>();
                data.Add(new Vector2(12, 0));
                data.Add(new Vector2(144, 13));
                data.Add(new Vector2(132, 32));
                data.Add(new Vector2(12, 19));
                data.Add(new Vector2(12, 38));
                data.Add(new Vector2(132, 51));
                data.Add(new Vector2(12, 57));
                data.Add(new Vector2(132, 70));

                _dataCount++;
                return data;
            }

            return null;
        }

        // DONE TODO: Lav bedre sorting. maybe?
        public Tuple<List<Vector2>, List<Vector2>> SortNxtCamData(List<Vector2> nxtCamData)
        {
            List<Vector2> leftPoints = new List<Vector2>();
            List<Vector2> rightPoints = new List<Vector2>();

            foreach (var point in nxtCamData)
            {
                if (point.X <= MaxNxtCamX / 2)
                    leftPoints.Add(point);
                else
                    rightPoints.Add(point);
            }

            //for (int i = 0; i < nxtCamData.Capacity; i += 2)
            //{
            //    if (nxtCamData[i] <= MaxNxtCamX / 2)
            //    {
            //        leftPoints.Add(nxtCamData[i]);
            //        leftPoints.Add(nxtCamData[i + 1]);
            //    }
            //    else
            //    {
            //        rightPoints.Add(nxtCamData[i]);
            //        rightPoints.Add(nxtCamData[i + 1]);
            //    }
            //}
            return Tuple.Create(leftPoints, rightPoints);
        }

        // TODO: 100% lort.
        private double[] CombineData(List<Vector2> pData, List<Vector2> pDataOld)
        {
            //double[] data = new double[pData.Count + pDataOld.Count];

            //for (int i = 0; i < pDataOld.Count; i++)
            //{
            //    data[i] = pDataOld[i];
            //}
            //for (int i = 0; i < pData.Count; i++)
            //{
            //    data[i + pDataOld.Count] = pData[i];
            //}

            // Order By Y-Value
            //double[] orderedData = new double[data.Length];
            //int minYIndex = 0; // TODO: Can crash then using this value?
            //double minY = double.MaxValue;
            //int x = 0;

            //while (data.Length > 0)
            //{
            //    for (int i = 1; i < data.Length; i += 2)
            //    {
            //        if (data[i] < minY)
            //        {
            //            minY = data[i];
            //            minYIndex = i;
            //        }
            //    }

            //    orderedData[x] = data[minYIndex - 1];
            //    orderedData[x + 1] = data[minYIndex];
            //    x += 2;

            //    data = RemoveAt(data, minYIndex - 1);
            //    data = RemoveAt(data, minYIndex - 1);

            //    minY = double.MaxValue;
            //}



            ////////////////////////////7 NEW WAY
            List<Vector2> data = new List<Vector2>();
            data.AddRange(pDataOld);
            data.AddRange(pData);

            List<Vector2> SortedData = data.OrderBy(p => p.Y).ToList();

            double[] d = new double[SortedData.Capacity*2];
            int i = 0;

            foreach (var vector2 in SortedData)
            {
                d[i] = vector2.X;
                d[i+1] = vector2.Y;
                i +=2;
            }
            return d;
        }








        //DONE?!?
        private void RotateAndDisplaceData(List<Vector2> DataL, List<Vector2> DataR, List<Vector2> LastTwoPoints)
        {
            int RotationDirection;

            if (LastTwoPoints[0].X < LastTwoPoints[1].X) // we are turning clockwise, soo rotate counterclockwise -1 is cloclwise and 1 is counterclockwise
            {
                RotationDirection = 1;
            }
            else //we are turning counterclockwise so turn clockwise
            {
                RotationDirection = -1;
            }

            double RotationSumInDegrees = Math.Atan(Math.Abs((LastTwoPoints[0].X - LastTwoPoints[1].X)) / Math.Abs(LastTwoPoints[0].Y - LastTwoPoints[1].Y)); // math.abs is the absolute value e.g always positive
            RotationSumInDegrees = RotationSumInDegrees * RotationDirection;
            // for (int i = 0; i < 8; i+=2)
            int i = 0;

            while (DataL.Count > i + 1)
            {
                double tempXValue = DataL[i].X; // we will override x value, but still need original when rotating y
                double tempYValue = DataL[i].Y; // i dont think this is needed but it makes it pretty

                /*
                 * take care
                 * what way is it rotating?
                 * if the bus has rotated clockwise
                 * rotate the cordinats counterclockwise
                 */
                DataL[i].X = tempXValue * Math.Cos(RotationSumInDegrees) - tempYValue * Math.Sin(RotationSumInDegrees); // rotation
                DataL[i].Y = tempXValue * Math.Sin(RotationSumInDegrees) + tempYValue * Math.Cos(RotationSumInDegrees);
                i += 2;
            }
            i = 0;

            while (DataR.Count > i + 1)
            {
                double tempXValue = DataR[i].X; // we will override x value, but still need original when rotating y
                double tempYValue = DataR[i].Y; // i dont think this is needed but it makes it pretty

                /*
                 * take care
                 * what way is it rotating?
                 * if the bus has rotated clockwise
                 * rotate the cordinats counterclockwise
                 */
                DataR[i].X = tempXValue * Math.Cos(RotationSumInDegrees) - tempYValue * Math.Sin(RotationSumInDegrees); // rotation
                DataR[i].Y = tempXValue * Math.Sin(RotationSumInDegrees) + tempYValue * Math.Cos(RotationSumInDegrees);
                i += 2;
            }
            i = 0;
            
            double tempXValue2 = LastTwoPoints[0].X; // we will override x value, but still need original when rotating y
            double tempYValue2 = LastTwoPoints[0].Y; // i dont think this is needed but it makes it pretty

            LastTwoPoints[1].X = tempXValue2 * Math.Cos(RotationSumInDegrees) - tempYValue2 * Math.Sin(RotationSumInDegrees); // rotation
            LastTwoPoints[1].Y = tempXValue2 * Math.Sin(RotationSumInDegrees) + tempYValue2 * Math.Cos(RotationSumInDegrees);


            /*
             * Set end point to startpoint cordinats,
             * all start points must be at the same spot in the graph 
             */
            double DisplacementX = 88 - LastTwoPoints[1].X; //after endpoint has been rotated
            double DisplacementY = (-20) - LastTwoPoints[1].Y;

            /*
             * lastly we displace all of the cordinats
             */
            // for (int i = 0; i < 8; i+=2)
            while (DataL.Count > i + 1)
            {
                DataL[i].X = DataL[i].X + DisplacementX;
                DataL[i].Y = DataL[i].Y + DisplacementY;
                i += 2;
            }
            i = 0;
            // for (int i = 0; i < 8; i += 2)
            while (DataR.Count > i + 1)
            {
                DataR[i].X = DataR[i].X + DisplacementX;
                DataR[i].Y = DataR[i].Y + DisplacementY;
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
