using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FollowTrack
{
    public class FieldOfViewCorrecter
    {
        private readonly int _cameraHeight;
        private readonly int _fieldOfViewXdeg;
        private readonly int _fieldOfViewYdeg;
        private readonly int _cameraAngle;
        private readonly int _cameraBlindSpotDeg;

        public FieldOfViewCorrecter(int cameraHeight, int fieldOfViewXdeg, int fieldOfViewYdeg, int cameraAngle)
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

        public Tuple<int, int> CalcFloorCoordinates(int pictureHorizontalCoordinate, int pictureVerticalCoordinate)
        {
            // todo rename horizontal and vertical to x and y soon. Just note that it's the reverse in the mathcad calculations. 
            int y = CalcFloorCoordinateY(pictureVerticalCoordinate);
            int x = CalcFloorCoordinateX(pictureHorizontalCoordinate, pictureVerticalCoordinate, y);
            return new Tuple<int, int>(x, y);
        }

        public int CalcFloorCoordinateY(int pictureY)
        {
            int deg = ConvertToDegrees(pictureY, _fieldOfViewYdeg);
            return TangensSolveOpposite(_cameraHeight, deg + _cameraBlindSpotDeg);
        }

        private int TangensSolveOpposite(int adjacent, int degreesA)
        {
            return Convert.ToInt32(adjacent * Math.Tan(degreesA));
        }

        private int ConvertToDegrees(int value, int fieldOfView)
        {
            return Convert.ToInt32(value / (100 / (double) fieldOfView));
        }

        public int CalcFloorCoordinateX(int pictureX, int pictureY, int floorCoordinateY)
        {
            int deg = ConvertToDegrees(pictureX, _fieldOfViewXdeg) - 20;
            int hypo = PythagorasSolveC(_cameraHeight, floorCoordinateY);
            return TangensSolveOpposite(hypo, deg);
        }

        private int PythagorasSolveC(int a, int b)
        {
            return Convert.ToInt32(Math.Sqrt((a * a) + (b * b)));
        }
    }
}
