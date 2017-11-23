#ifndef CONNECTIVITY_FIELDOFVIEWCORRECTOR_H
#define CONNECTIVITY_FIELDOFVIEWCORRECTOR_H

// TODO: Most of this isn't gonna work at all
class FieldOfViewCorrector {
public:
	FieldOfViewCorrector(double cameraHeight, double fieldOfViewXdeg, double fieldOfViewYdeg, double cameraAngle, double maxPossibleX = 100, double maxPossibleY = 100);
	Vector2* CalcFloorCoordinates(Vector2 nxtCamData[]);
	Vector2* CalcFloorCoordinates(double pictureHorizontalCoordinate, double pictureVerticalCoordinate, bool displaceOriginToMid = true)

	double CalcFloorCoordinateY(double pictureY);

private:
	// Vars
	double _cameraHeight;
	double _fieldOfViewXdeg;
	double _fieldOfViewYdeg;
	double _cameraAngle;
	double _cameraBlindSpotDeg;
	double _maxPossibleX;
	double _maxPossibleY;

	double TanSolveOpposite(double adjacent, double degreesA);
	double ConvertToDegrees(double value, double maximumValue, double fieldOfView);
	double CalcFloorCoordinateX(double pictureX, double floorCoordinateY, bool displaceOriginToMid);
	double PythagorasSolveC(double a, double b);
	void DisplaceCoordinates(Vector2* coordinates, size_t count); // Breaks old stuff
};

#endif //CONNECTIVITY_FIELDOFVIEWCORRECTOR_H
