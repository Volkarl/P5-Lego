#ifndef CONNECTIVITY_DRIVELOGIC_H
#define CONNECTIVITY_DRIVELOGIC_H

#include "FieldOfViewCorrector.h"
#include "CamCalcTypes.h"

class DriveLogic {

public:
	DriveLogic();
	~DriveLogic();

	PathPoint Path[PointsOnCurve];
	void CalculateBezierCurvePoints(double * returnData, Vector2* pData, int pointsOnCurve, int& boundCount);
	void CalculatePathMidPoints(Vector2* returnData, double pLeft[], double pRight[]) // TODO: get array sizes
	void CalculatePathData(PathPoint* pathPoints, Vector2* midPoints, size_t length);

private:
	const int MaxNxtCamX = 176;
	const int MaxNxtCamY = 144;
	const double MaxRealX = 18.95;
	const double MinRealX = -18.95;
	const double MaxRealY = 47.18;
	const double MinRealY = -10.5;

	const int PointsOnCurve = 8; //    Points = (value / 2) - 1
	const double LaneWidth = 28.9;

	const int NxtCamHeight = 22;
	const int FieldOfView = 40;
	const int CameraAngle = 45;

	Vector2 busPoint = {0, -10.5};
	FieldOfViewCorrector _fov(NxtCamHeight, FieldOfView, FieldOfView, CameraAngle, MaxNxtCamX, MaxNxtCamY);

	bool _isFirstTimeRunning = true;
	int _boundCountLeft = 0;
	int _boundCountRight = 0;

	int _pathCounter = 0;
	int _pathSize = 0;

	BezierCurve bezierCurve;
	void ApproximationOfTheOtherSideOfTheRoad(Vector2* returnData, Vector2* lastPoint1, Vector2* lastPoint2, bool leftOrRightSide, double distance);
};

#endif //CONNECTIVITY_DRIVELOGIC_H
