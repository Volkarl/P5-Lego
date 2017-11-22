using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    /// <summary> 
    /// A photo is a representation of the 3D world onto 2 dimensions. The position of coordinates on a picture doesn't transfer directly to the position of  
    /// those coordinates in the real world. This class uses coordinates from the picture along with information about the camera angle and how much the camera 
    /// can see to calculate the actual coordinates of those points in the real world. It is assumed that any point in the real world has a z-value of 0. 
    /// </summary> 
    public class FieldOfViewCorrecter
    {
        private readonly double _cameraHeight;
        private readonly double _fieldOfViewXdeg;
        private readonly double _fieldOfViewYdeg;
        private readonly double _cameraAngle;
        private readonly double _cameraBlindSpotDeg;
        private readonly double _maxPossibleX;
        private readonly double _maxPossibleY;

        public FieldOfViewCorrecter(double cameraHeight, double fieldOfViewXdeg, double fieldOfViewYdeg, double cameraAngle, double maxPossibleX = 100, double maxPossibleY = 100)
        {
            if(cameraHeight < 0 || cameraHeight > 100
                || fieldOfViewXdeg <= 0 || fieldOfViewXdeg >= 90
                || fieldOfViewYdeg <= 0 || fieldOfViewYdeg >= 90
                || cameraAngle < 0 || cameraAngle >= 90
                || cameraAngle + fieldOfViewYdeg <= 0 || cameraAngle + fieldOfViewYdeg >= 90
                || _maxPossibleX < 0
                || _maxPossibleY < 0)
                throw new ArgumentException();
            // The angle + field of view cannot reach 90 degrees, because otherwise no triangle calculation could work

            _cameraHeight = cameraHeight;
            _fieldOfViewXdeg = fieldOfViewXdeg;
            _fieldOfViewYdeg = fieldOfViewYdeg;
            _cameraAngle = cameraAngle;
            _maxPossibleX = maxPossibleX; 
            _maxPossibleY = maxPossibleY;
            _cameraBlindSpotDeg = cameraAngle - _fieldOfViewYdeg / 2;
        }

        public Vector2[] CalcFloorCoordinates(Vector2[] nxtCamData)
        {
            // todo Nothing nxtcam specific should be in here! Move it later

            nxtCamData = DisplaceCoordinates(nxtCamData);

            List<Tuple<double, double>> floorCoordinates = new List<Tuple<double, double>>();
            foreach (Vector2 coordinate in nxtCamData)
            {
                floorCoordinates.Add(CalcFloorCoordinates(coordinate.X, coordinate.Y, false));
            }
            Vector2[] correctedCoords = new Vector2[nxtCamData.Length];
            correctedCoords = floorCoordinates.ConvertAll(fc => new Vector2(fc.Item1, fc.Item2)).ToArray();

            return correctedCoords;
        }

        /// <summary>
        /// Calculates floor coordinates (distance from the camera) using coordinates from a picture.
        /// </summary>
        /// <param name="pictureHorizontalCoordinate"></param>
        /// <param name="pictureVerticalCoordinate"></param>
        /// <param name="displaceOriginToMid">
        /// Usually the origin (0,0) of a coordinate system is in the bottom left, but if true, the function will return numbers as if the origin was
        /// half of the maximum x-value, which is where the camera is placed. This means the returned x-coordinates will be negative if they are left
        /// of the camera and positive if they are right of the camera.
        /// </param>
        /// <returns></returns>
        public Tuple<double, double> CalcFloorCoordinates(double pictureHorizontalCoordinate, double pictureVerticalCoordinate, bool displaceOriginToMid = true)
        {
            // Note that the horiontal (x) coordinate is the y-coordinate in the mathcad calculations, and vice versa
            double y = CalcFloorCoordinateY(pictureVerticalCoordinate);
            double x = CalcFloorCoordinateX(pictureHorizontalCoordinate, y, displaceOriginToMid);
            return new Tuple<double, double>(x, y);
        }

        public double CalcFloorCoordinateY(double pictureY)
        {
            if (pictureY > _maxPossibleY) throw new ArgumentException();

            double deg = ConvertToDegrees(pictureY, _maxPossibleY, _fieldOfViewYdeg);
            return TangensSolveOpposite(_cameraHeight, deg + _cameraBlindSpotDeg);
        }

        private double TangensSolveOpposite(double adjacent, double degreesA)
        {
            return adjacent * Math.Tan(degreesA.DegreeToRadian());
        }

        private double ConvertToDegrees(double value, double maximumValue, double fieldOfView)
        {
            return value / (maximumValue / fieldOfView);
        }

        private double CalcFloorCoordinateX(double pictureX, double floorCoordinateY, bool displaceOriginToMid)
        {
            if (pictureX > _maxPossibleX) throw new ArgumentException();

            double deg = ConvertToDegrees(pictureX, _maxPossibleX, _fieldOfViewXdeg);
            if (displaceOriginToMid) deg = -((_fieldOfViewXdeg / 2) - deg);
            // Because the x axis has its origin in the middle of the FoV: 0 deg should become -20 and 40 deg should become 20
            double hypo = PythagorasSolveC(_cameraHeight, floorCoordinateY);
            return TangensSolveOpposite(hypo, deg);
        }

        private double PythagorasSolveC(double a, double b)
        {
            return Math.Sqrt((a * a) + (b * b));
        }




        ///////////////////////////////

        private Vector2[] DisplaceCoordinates(Vector2[] coordinates)
        {
            // Moves the origin of the coordinate system. In stead of being in the very top left, it is now in the bottom-mid

            double displacementX = _maxPossibleX / 2, displacementY = _maxPossibleY;
            foreach (Vector2 coordinate in coordinates)
            {
                coordinate.X -= displacementX;
                coordinate.Y = Math.Abs(coordinate.Y - displacementY); 
                // High numbers become low numbers and the reverse as well
            }
            return coordinates;

            // I may still need to displace coordinates in the opposite direction:
            // However, i will need to use the new max and min points. Max is now for instance 47cm x -8(?)cm
            // So i'll need to minus this
        }
    }
}
