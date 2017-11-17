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

        public FieldOfViewCorrecter(double cameraHeight, double fieldOfViewXdeg, double fieldOfViewYdeg, double cameraAngle)
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
            _cameraBlindSpotDeg = cameraAngle - _fieldOfViewYdeg;
        }

        public Tuple<double, double> CalcFloorCoordinates(double pictureHorizontalCoordinate, double pictureVerticalCoordinate)
        {
            // todo rename horizontal and vertical to x and y soon. Just note that it's the reverse in the mathcad calculations. 
            double y = CalcFloorCoordinateY(pictureVerticalCoordinate);
            double x = CalcFloorCoordinateX(pictureHorizontalCoordinate, pictureVerticalCoordinate, y);
            return new Tuple<double, double>(x, y);
        }

        public double CalcFloorCoordinateY(double pictureY)
        {
            double deg = ConvertToDegrees(pictureY, _fieldOfViewYdeg);
            return TangensSolveOpposite(_cameraHeight, deg + _cameraBlindSpotDeg);
        }

        private double TangensSolveOpposite(double adjacent, double degreesA)
        {
            return adjacent * Math.Tan(degreesA);
        }

        private double ConvertToDegrees(double value, double fieldOfView)
        {
            return value / (100 / fieldOfView);
        }

        public double CalcFloorCoordinateX(double pictureX, double pictureY, double floorCoordinateY)
        {
            double deg = ConvertToDegrees(pictureX, _fieldOfViewXdeg) - 20;
            double hypo = PythagorasSolveC(_cameraHeight, floorCoordinateY);
            return TangensSolveOpposite(hypo, deg);
        }

        private double PythagorasSolveC(double a, double b)
        {
            return Math.Sqrt((a * a) + (b * b));
        }
    }
}
