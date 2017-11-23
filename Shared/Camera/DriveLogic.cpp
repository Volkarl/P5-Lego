#include "DriveLogic.h"
#include <cmath>
#include <algorithm>
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



void SortNxtCamData() // TODO: How much is going to be class-wide?
{
	const int dataLen = 16;

	Vector2 leftPoints[8];
	Vector2 rightPoints[8];

	int maxIndexY = 0;
	int leftCount = 0;
	int rightCount = 0;

	for (int i = 0; i < dataLen; i++) {
		const Vector2& point = nxtCamData[i];

		if (point.x <= 0)
		{
			leftPoints[leftCount] = point;
			leftCount++;
		}
		else
		{
			rightPoints[rightCount] = point;
			rightCount++;
		}

		if (point.y > nxtCamData[maxIndexY].y)
		{
			maxIndexY = i;
		}
	}

	// TODO: fix: default comparison (operator <):
	std::sort(leftPoints.begin(), leftPoints.end());
	std::sort(rightPoints.begin(), rightPoints.end());

	// TODO: Convert the rest
}

void CombineData(Vector2* data, Vector2* pData, Vector2* pDataOld) // TODO: FIX I DONT GET IT
{
	//Vector2 data[] = new Vector2[pData.Length + pDataOld.Length];
	Array.Copy(pDataOld, data, 8);
	Array.Copy(pData, 0, data, 8, 8);

	//Array.Sort(data);
}

void RotateAndDisplaceData(Vector2* dataL, Vector2* dataR, Vector2* lastTwoPoints)
{
	int leftLen = 8;  // TODO: Research length
	int rightLen = 8; // TODO: Research length

	const Vector2& last2Point1 = lastTwoPoints[0];
	const Vector2& last2Point2 = lastTwoPoints[1];

	int rotationDirection = last2Point1.x < last2Point2.x ? 1 : -1;
	double rotationSumInDegrees = (atan(abs((last2Point1.x - last2Point2.x)) / abs(last2Point1.y - last2Point2.y))) * rotationDirection;


	for (int i = 0; i <= leftLen - 1; i++)
	{
		double tempX = dataL[i].x;
		double tempY = dataL[i].y;

		dataL[i].x = tempX * cos(rotationSumInDegrees) - tempY * sin(rotationSumInDegrees); // rotation
		dataL[i].y = tempX * sin(rotationSumInDegrees) + tempY * cos(rotationSumInDegrees);
	}

	for (int i = 0; i <= rightLen - 1; i++)
	{
		double tempX = dataR[i].x;
		double tempY = dataR[i].y;

		dataR[i].x = tempX * cos(rotationSumInDegrees) - tempY * sin(rotationSumInDegrees); // rotation
		dataR[i].y = tempX * sin(rotationSumInDegrees) + tempY * cos(rotationSumInDegrees);
	}

	double tempX = last2Point1.x;
	double tempY = last2Point1.y;

	last2Point2.X = tempX * Math.Cos(rotationSumInDegrees) - tempY * Math.Sin(rotationSumInDegrees); // rotation
	last2Point2.Y = tempX * Math.Sin(rotationSumInDegrees) + tempY * Math.Cos(rotationSumInDegrees);

	double displacementX = _busPoint.x - last2Point2.x;
	double displacementY = _busPoint.y - last2Point2.y;

	for (int i = 0; leftLen - 1 >= i; i++)
	{
		if (dataL[i] != nullptr) // TODO: the fuck
		{
			dataL[i].x = dataL[i].x + displacementX;
			dataL[i].y = dataL[i].y + displacementY;
		}
	}
	for (int i = 0; leftLen - 1 >= i; i++)
	{
		if (dataR[i] != nullptr)
		{
			dataR[i].x = dataR[i].x + displacementX;
			dataR[i].y = dataR[i].y + displacementY;
		}
	}

}


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

