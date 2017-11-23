#include "FieldOfViewCorrector.h"
#include "CamCalcTypes.h"
#include <cmath>
#include "../../Shared/Tools.h"

FieldOfViewCorrector::FieldOfViewCorrector(double cameraHeight, double fieldOfViewXdeg, double fieldOfViewYdeg,
										   double cameraAngle, double maxPossibleX, double maxPossibleY)
{
	if(cameraHeight < 0 || cameraHeight > 100
	   || fieldOfViewXdeg <= 0 || fieldOfViewXdeg >= 90
	   || fieldOfViewYdeg <= 0 || fieldOfViewYdeg >= 90
	   || cameraAngle < 0 || cameraAngle >= 90
	   || cameraAngle + fieldOfViewYdeg <= 0 || cameraAngle + fieldOfViewYdeg >= 90
	   || _maxPossibleX < 0
	   || _maxPossibleY < 0)
		// TODO: throw new ArgumentException();
	// The angle + field of view cannot reach 90 degrees, because otherwise no triangle calculation could work

	_cameraHeight = cameraHeight;
	_fieldOfViewXdeg = fieldOfViewXdeg;
	_fieldOfViewYdeg = fieldOfViewYdeg;
	_cameraAngle = cameraAngle;
	_maxPossibleX = maxPossibleX;
	_maxPossibleY = maxPossibleY;
	_cameraBlindSpotDeg = cameraAngle - _fieldOfViewYdeg / 2;
}

// TODO Not gonna work
Vector2* FieldOfViewCorrector::CalcFloorCoordinates(Vector2 nxtCamData[])
{
	// todo Nothing nxtcam specific should be in here! Move it later
	size_t count = 8; // TODO: HARDCODED

	nxtCamData = this->DisplaceCoordinates(nxtCamData, count);

	Vector2 floorCoordinates[count];

/*
	List<Tuple<double, double>> floorCoordinates = new List<Tuple<double, double>>();
 	foreach (Vector2 coordinate in nxtCamData)
	{
	floorCoordinates.Add(CalcFloorCoordinates(coordinate.X, coordinate.Y, false));
	}*/
	Vector2 correctedCoords[count];
	// TODO: correctedCoords = floorCoordinates.ConvertAll(fc => new Vector2(fc.Item1, fc.Item2)).ToArray();

	return correctedCoords;
}

// TODO: Not gonna work
Vector2* FieldOfViewCorrector::CalcFloorCoordinates(double pictureHorizontalCoordinate, double pictureVerticalCoordinate, bool displaceOriginToMid = true)
{
	Vector2 t;

	// Note that the horiontal (x) coordinate is the y-coordinate in the mathcad calculations, and vice versa
	t.y = this->CalcFloorCoordinateY(pictureVerticalCoordinate);
	t.x = this->CalcFloorCoordinateX(pictureHorizontalCoordinate, t.y, displaceOriginToMid);

	return t;
}

double FieldOfViewCorrector::CalcFloorCoordinateY(double pictureY)
{
	// TODO: if (pictureY > _maxPossibleY) throw new ArgumentException();

	double deg = ConvertToDegrees(pictureY, _maxPossibleY, _fieldOfViewYdeg);

	return this->TanSolveOpposite(_cameraHeight, deg + _cameraBlindSpotDeg);
}

double FieldOfViewCorrector::TanSolveOpposite(double adjacent, double degreesA)
{
	return adjacent * tan(DegreesToRadian(degreesA));
}

double FieldOfViewCorrector::ConvertToDegrees(double value, double maximumValue, double fieldOfView)
{
	return value / (maximumValue / fieldOfView);
}

double FieldOfViewCorrector::CalcFloorCoordinateX(double pictureX, double floorCoordinateY, bool displaceOriginToMid)
{
	// TODO: if (pictureX > _maxPossibleX) throw new ArgumentException();

	double deg = ConvertToDegrees(pictureX, _maxPossibleX, _fieldOfViewXdeg);
	if (displaceOriginToMid) deg = -((_fieldOfViewXdeg / 2) - deg);

	// Because the x axis has its origin in the middle of the FoV: 0 deg should become -20 and 40 deg should become 20
	double hypo = this->PythagorasSolveC(_cameraHeight, floorCoordinateY);

	return this->TanSolveOpposite(hypo, deg);
}

double FieldOfViewCorrector::PythagorasSolveC(double a, double b)
{
	return sqrt((a * a) + (b * b));
}


void FieldOfViewCorrector::DisplaceCoordinates(Vector2* coordinates, size_t count)
{
	// Moves the origin of the coordinate system. In stead of being in the very top left, it is now in the bottom-mid
	double displacementX = this->_maxPossibleX / 2, displacementY = this->_maxPossibleY;

	for (int i = 0; i < count; i++)
	{
		const Vector2& coord = coordinates[i];

		coord.x -= displacementX;
		coord.x = Math.Abs(coordinate.Y - displacementY);
		// High numbers become low numbers and the reverse as well
	}


	// I may still need to displace coordinates in the opposite direction:
	// However, i will need to use the new max and min points. Max is now for instance 47cm x -8(?)cm
	// So i'll need to minus this
}
