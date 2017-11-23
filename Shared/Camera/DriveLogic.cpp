#include "DriveLogic.h"
#include <cmath>
#include "BezierCurve.h"

DriveLogic::DriveLogic()
{
	this->bezierCurve = new BezierCurve();
}

DriveLogic::~DriveLogic()
{
	delete bezierCurve;
}

void DriveLogic::CalculateBezierCurvePoints(double* returnData, Vector2* pData, int pointsOnCurve, int& boundCount)
{
	double data[pointsOnCurve];
	boundCount = 0;

	this->bezierCurve.Bezier2D(pData, pointsOnCurve / 2, returnData);

	// TODO: gem kun 1-2 old points, og sort data efter Bezier til kun relevante points.


	for (int i = 0; i < pointsOnCurve; i += 2)
	{
		if (data[i] <= MaxRealX && data[i] >= MinRealX && data[i + 1] <= MaxRealY && data[i + 1] >= MinRealY)
		{
			returnData[boundCount] = data[i];
			returnData[boundCount + 1] = data[i + 1];
			boundCount += 2;
		}
	}
}

void DriveLogic::CalculatePathMidPoints(Vector2* midPoints, double pLeft[], double pRight[]) // TODO: get array sizes
{
	midPoints[0] = _busPoint; // Set busPoint as the first pathpoint.

	//int pCount = pLeft.Length <= pRight.Length ? pLeft.Length : pRight.Length;
	const int pCount = _boundCountLeft <= _boundCountRight ? _boundCountLeft : _boundCountRight;

	for (int i = 1, midPointCount = 1; i < pCount - 1; i += 2, midPointCount++)
	{
		const Vector2& midPoint = midPoints[midPointCount];

		midPoint.x = (pLeft[i + 1] + pRight[i + 1]) / 2;
		midPoint.y = (pLeft[i] + pRight[i]) / 2;
	}
}

void DriveLogic::CalculatePathData(PathPoint* pathPoints, Vector2* midPoints, size_t length)
{
	for (int i = 1; i < length; i++)
	{
		if (midPoints[i] == nullptr) // TODO: Uh?
			break;

		const PathPoint& pathPoint = nxtCamData[i - 1];

		pathPoint.x = atan((midPoints[i].X - midPoints[i - 1].x) / (midPoints[i].y - midPoints[i - 1].y)) * 180 / M_PI;
		pathPoint.y = sqrt(pow(midPoints[i].x - midPoints[i - 1].x, 2) + pow(midPoints[i].y - midPoints[i - 1].y, 2));
	}
}

/* TODO: Sort NXTCam, combine func */

void DriveLogic::ApproximationOfTheOtherSideOfTheRoad(Vector2* returnData, Vector2* lastPoint1, Vector2* lastPoint2, bool leftOrRightSide, double distance) // right side should give negative number left positive
{
	Vector2 vectorBetweenTheTwoLastPoints = new Vector2(abs(lastPoint1.x - lastPoint2.x), abs(lastPoint1.y - lastPoint2.y)); // calculation of the vector between the two points

	if (vectorBetweenTheTwoLastPoints.y != 0)
	{
		const double orthogonalY = - (vectorBetweenTheTwoLastPoints.x * (leftOrRightSide ? -1 : 1)) / vectorBetweenTheTwoLastPoints.y; // calculation of the ortogonal vector
		const double multiplier = distance / ( sqrt(pow(1, 2) + pow(orthogonalY, 2))); // calculation of the multiplyer required for the ortogonal vector to be distance long

		returnData.x = lastPoint2.x + ((leftOrRightSide ? -1 : 1) * multiplier);
		returnData.y = lastPoint2.y + (orthogonalY * multiplier);
	}
	else
	{
		returnData.x = lastPoint2.x;
		returnData.y = lastPoint2.y + (distance * (leftOrRightSide ? -1 : 1));
	}

	delete vectorBetweenTheTwoLastPoints;
}

