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
        private const int MaxNxtCamX = 176;
        private const int MaxNxtCamY = 144;

        public int CurrentAngle;
        public List<PathPoint> Path = new List<PathPoint>();

        private const int PointsOnCurve = 18; //    Points = (value / 2) - 1
        private readonly Vector2 _busPoint = new Vector2(MaxNxtCamX / 2, MaxNxtCamY - MaxNxtCamY);

        private List<double> _pDataLeftOld = new List<double>();
        private List<double> _pDataRightOld = new List<double>();

        private readonly List<double> _lastTwoMidPointsOld = new List<double>();

        private bool _isFirstTimeRunning = true; 

        // Main
        public void Run()
        {
            if (Path.Count > 0)
            {
                Console.WriteLine("\n***************************************************");
                Drive.Turn(CurrentAngle + Path[0].Slope);
                Drive.Length(Path[0].Length); // Set how ofte Run needs to be called. //
                Path.RemoveAt(0);
            }
            else if (!_isFirstTimeRunning)
            {
                double[] nxtCamData = GetNewDataFromNxtCam(); // Need to balance data, and handle only left side data. //
                //ConvertDataFromFieldOfView(nxtCamData);

                //ConvertDataToWorldSpace(_pDataLeftOld, _pDataRightOld); // Use data from prev Path[] List. // ::NOTE: Convert new data also?
                RotateAndDisplaceData(_pDataLeftOld, _pDataRightOld, _lastTwoMidPointsOld);
                _lastTwoMidPointsOld.RemoveAt(0);
                _lastTwoMidPointsOld.RemoveAt(0);
                _lastTwoMidPointsOld.RemoveAt(0);
                _lastTwoMidPointsOld.RemoveAt(0);

                Tuple<List<double>, List<double>> tupleData = SortNxtCamData(nxtCamData);
                double[] dataLeft = CombineData(tupleData.Item1, _pDataLeftOld);
                double[] dataRight = CombineData(tupleData.Item2, _pDataRightOld);
                _pDataLeftOld = tupleData.Item1;
                _pDataRightOld = tupleData.Item2;

                List<Vector2> midPoints = CalculatePathMidPoints(CalculateBezierCurvePoints(dataLeft, PointsOnCurve*2+2), CalculateBezierCurvePoints(dataRight, PointsOnCurve*2+2));
                _lastTwoMidPointsOld.Add(midPoints[midPoints.Count - 2].X);
                _lastTwoMidPointsOld.Add(midPoints[midPoints.Count - 2].Y);
                _lastTwoMidPointsOld.Add(midPoints[midPoints.Count - 1].X);
                _lastTwoMidPointsOld.Add(midPoints[midPoints.Count - 1].Y);
                Path = CalculatePathData(midPoints);

                //long memory = GC.GetTotalMemory(true);
                //Console.WriteLine("MEMORY" + memory);
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
            else // First run.
            {
                double[] nxtCamData = GetNewDataFromNxtCam();
                //ConvertDataFromFieldOfView(nxtCamData);

                Tuple<List<double>, List<double>> tupleData = SortNxtCamData(nxtCamData);
                double[] dataLeft = CombineData(tupleData.Item1, _pDataLeftOld);
                double[] dataRight = CombineData(tupleData.Item2, _pDataRightOld);
                _pDataLeftOld = tupleData.Item1;
                _pDataRightOld = tupleData.Item2;

                List<Vector2> midPoints = CalculatePathMidPoints(CalculateBezierCurvePoints(dataLeft, PointsOnCurve), CalculateBezierCurvePoints(dataRight, PointsOnCurve));
                _lastTwoMidPointsOld.Add(midPoints[midPoints.Count - 2].X);
                _lastTwoMidPointsOld.Add(midPoints[midPoints.Count - 2].Y);
                _lastTwoMidPointsOld.Add(midPoints[midPoints.Count - 1].X);
                _lastTwoMidPointsOld.Add(midPoints[midPoints.Count - 1].Y);

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
            ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////
            //Console.WriteLine("TEST****************************");
            //Console.WriteLine("Left");
            //for (int i = 0; i < pLeft.Length; i += 2)
            //{
            //    Console.WriteLine("X: " + pLeft[i] + "\tY: " + pLeft[i+1]);
            //}
            //Console.WriteLine("Right");
            //for (int i = 0; i < pRight.Length; i += 2)
            //{
            //    Console.WriteLine("X: " + pRight[i] + "\tY: " + pRight[i + 1]);
            //}
            //Console.ReadKey();
            ////////////////////////////////////////////////////////////////////////TEST//////////////////////////////////////////



            List<Vector2> midPoints = new List<Vector2>();
            midPoints.Add(_busPoint); // set busPoint as the first pathpoint.

            double x;
            double y;

            int pCount = pLeft.Length <= pRight.Length ? pLeft.Length : pRight.Length;

            for (int i = 1; i != pCount - 1; i += 2) // PointsOnCurve er mindre eftersom vi lige har fjernet nogen fra old data.
            {
                x = (pLeft[i + 1] + pRight[i + 1]) / 2;
                y = (pLeft[i] + pRight[i]) / 2;

                Vector2 v = new Vector2(x, y);
                midPoints.Add(v);
            }

            return midPoints;
        }

        // DONE
        private List<PathPoint> CalculatePathData(List<Vector2> midPoints)
        {
            List<PathPoint> pathPoints = new List<PathPoint>();

            double distance;
            double slope;

            for (int i = 1; i < midPoints.Count; i++)
            {
                slope = Math.Atan((midPoints[i].X - midPoints[i - 1].X) / (midPoints[i].Y - midPoints[i - 1].Y)) * 180 / Math.PI;
                distance = Math.Sqrt(Math.Pow(midPoints[i].X - midPoints[i - 1].X, 2) + Math.Pow(midPoints[i].Y - midPoints[i - 1].Y, 2));

                PathPoint pathPoint = new PathPoint(slope, distance);
                pathPoints.Add(pathPoint);
            }

            return pathPoints;
        }




        //TODO: tilpas til NxtCam
        private int _dataCount = 0;
        private double[] GetNewDataFromNxtCam()
        {
            if (_dataCount == 0)
            {
                double[] data = { 12, 0, 144, 13, 132, 32, 12, 19, 12, 38, 132, 51, 12, 57, 132, 70 };
                _dataCount++;
                return data;
            }
            else if (_dataCount == 1)
            {
                double[] data = { 12, 0, 144, 13, 132, 32, 12, 19, 12, 38, 132, 51, 12, 57, 132, 70 };
                _dataCount++;
                return data;
            }
            else if (_dataCount == 2)
            {
                double[] data = { };
                _dataCount++;
                return data;
            }

            return null;
        }

        // DONE TODO: Lav bedre sorting. maybe?
        public Tuple<List<double>, List<double>> SortNxtCamData(double[] nxtCamData)
        {
            List<double> leftPoints = new List<double>();
            List<double> rightPoints = new List<double>();

            for (int i = 0; i < nxtCamData.Length; i += 2)
            {
                if (nxtCamData[i] <= MaxNxtCamX / 2)
                {
                    leftPoints.Add(nxtCamData[i]);
                    leftPoints.Add(nxtCamData[i + 1]);
                }
                else if (nxtCamData[i] > MaxNxtCamX / 2)
                {
                    rightPoints.Add(nxtCamData[i]);
                    rightPoints.Add(nxtCamData[i + 1]);
                }
                else
                {
                    throw new NotImplementedException();
                }
            }

            return Tuple.Create(leftPoints, rightPoints);
        }

        // TODO: 100% lort.
        private double[] CombineData(List<double> pData, List<double> pDataOld)
        {
            double[] data = new double[pData.Count + pDataOld.Count];

            for (int i = 0; i < pDataOld.Count; i++)
            {
                data[i] = pDataOld[i];
            }
            for (int i = 0; i < pData.Count; i++)
            {
                data[i + pDataOld.Count] = pData[i];
            }


            // Order By Y-Value
            double[] orderedData = new double[data.Length];
            int minYIndex = 0; // TODO: Can crash then using this value?
            double minY = double.MaxValue;
            int x = 0;

            while (data.Length > 0)
            {
                for (int i = 1; i < data.Length; i += 2)
                {
                    if (data[i] < minY)
                    {
                        minY = data[i];
                        minYIndex = i;
                    }
                }

                orderedData[x] = data[minYIndex - 1];
                orderedData[x + 1] = data[minYIndex];
                x += 2;

                data = RemoveAt(data, minYIndex - 1);
                data = RemoveAt(data, minYIndex - 1);

                minY = double.MaxValue;
            }
            return orderedData;
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
