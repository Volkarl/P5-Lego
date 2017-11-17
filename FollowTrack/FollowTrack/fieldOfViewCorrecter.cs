using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    public class FieldOfViewCorrecter
    {
        private readonly double _cameraHeight;
        private readonly double _fieldOfViewXdeg;
        private readonly double _fieldOfViewYdeg;
        private readonly double _cameraAngle;
        private readonly double _cameraBlindSpotDeg;
        private readonly double _maxPossibleX;
        private readonly double _maxPossibleY;

        public FieldOfViewCorrecter(double cameraHeight, double fieldOfViewXdeg, double fieldOfViewYdeg, double cameraAngle, double maxPossibleX, double maxPossibleY)
        {
            if(cameraHeight < 0 || cameraHeight > 100
                || fieldOfViewXdeg <= 0 || fieldOfViewXdeg >= 90
                || fieldOfViewYdeg <= 0 || fieldOfViewYdeg >= 90
                || cameraAngle < 0 || cameraAngle >= 90
                || cameraAngle + fieldOfViewYdeg <= 0 || cameraAngle + fieldOfViewYdeg >= 90)
                throw new ArgumentException();
            // The angle + field of view cannot reach 90 degrees, because otherwise no triangle calculation could work

            _cameraHeight = cameraHeight;
            _fieldOfViewXdeg = fieldOfViewXdeg;
            _fieldOfViewYdeg = fieldOfViewYdeg;
            _cameraAngle = cameraAngle;
            _maxPossibleX = maxPossibleX; //todo error check these
            _maxPossibleY = maxPossibleY;
            _cameraBlindSpotDeg = cameraAngle - _fieldOfViewYdeg / 2;
        }

        public List<Vector2> CalcFloorCoordinates(List<Vector2> nxtCamData)
        {
            // todo Nothing nxtcam specific should be in here! Move it later

            nxtCamData = DisplaceCoordinates(nxtCamData);

            List<Tuple<double, double>> floorCoordinates = new List<Tuple<double, double>>();
            foreach (Vector2 coordinate in nxtCamData)
            {
                floorCoordinates.Add(CalcFloorCoordinates(coordinate.X, coordinate.Y));
            }
            List<Vector2> correctedCoords = new List<Vector2>(floorCoordinates.ConvertAll(fc => new Vector2(fc.Item1, fc.Item2)));

            correctedCoords = DisplaceCoordinates(correctedCoords);
            return correctedCoords;
        }

        public Tuple<double, double> CalcFloorCoordinates(double pictureHorizontalCoordinate, double pictureVerticalCoordinate)
        {
            // todo rename horizontal and vertical to x and y soon. Just note that it's the reverse in the mathcad calculations. 
            double y = CalcFloorCoordinateY(pictureVerticalCoordinate);
            double x = CalcFloorCoordinateX(pictureHorizontalCoordinate, y);
            return new Tuple<double, double>(x, y);
        }

        public double CalcFloorCoordinateY(double pictureY)
        {
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

        private double CalcFloorCoordinateX(double pictureX, double floorCoordinateY)
        {
            double deg = ConvertToDegrees(pictureX, _maxPossibleX, _fieldOfViewXdeg);
            double hypo = PythagorasSolveC(_cameraHeight, floorCoordinateY);
            return TangensSolveOpposite(hypo, deg);
        }

        private double PythagorasSolveC(double a, double b)
        {
            return Math.Sqrt((a * a) + (b * b));
        }




        ///////////////////////////////

        private List<Vector2> DisplaceCoordinates(List<Vector2> coordinates)
        {
            // Moves the origin of the coordinate system. In stead of being in the very top left, it is now in the bottom-mid

            double displacementX = -(_maxPossibleX / 2), displacementY = _maxPossibleY;
            foreach (Vector2 coordinate in coordinates)
            {
                coordinate.X += displacementX;
                coordinate.Y += displacementY;
            }
            return coordinates;

            // I may still need to displace coordinates in the opposite direction:
            // However, i will need to use the new max and min points. Max is now for instance 47cm x -8(?)cm
            // So i'll need to minus this
        }
    }
}
