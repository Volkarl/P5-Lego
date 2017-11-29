#include "rectangle.h"
#include "../../Shared/Connectivity.h"
#include <algorithm>

Vector2::Vector2() {}
Vector2::Vector2(float x, float y) : X(x), Y(y) {}

Vector2 Vector2::operator+ (const Vector2& other) const {
	return Vector2(X + other.X, Y + other.Y);
}

RectangleF::RectangleF() {
	x = y = w = h = 0;
	rotation = 0;
	anchorInCenter = false;
}

RectangleF::RectangleF(const Rectangle_T& rawrect) {
	x = (float)rawrect.upperLeftX;
	y = (float)rawrect.upperLeftY;
	w = (float)rawrect.width;
	h = (float)rawrect.height;
}

RectangleF::~RectangleF()
{
}

void RectangleF::Translate(const Vector2 &v)
{
	x += v.X;
	y += v.Y;
}

void RectangleF::Rotate(float fDegrees)
{
	rotation += fDegrees;
}

Vector2 RectangleF::GetPos() const
{
	return Vector2(x, y);
}

Vector2 RectangleF::GetSize() const
{
	return Vector2(w, h);
}

void RectangleF::GetRotatedPosArray(Vector2* pArray) const
{
	Vector2 &topLeft = pArray[0];
	Vector2 &topRight = pArray[1];
	Vector2 &botRight = pArray[2];
	Vector2 &botLeft = pArray[3];

	Vector2 pos = GetPos();

	topLeft = pos;
	topRight = pos + Vector2(w, 0.0f);
	botRight = pos + Vector2(w, h);
	botLeft = pos + Vector2(0.0f, h);
}

bool RectangleF::Contains(const Vector2 &point)
{
	Vector2 verts[4];
	GetRotatedPosArray(verts);
	return PointInPolygon(point, verts, 4);
}

bool RectangleF::Intersects(const RectangleF &other)
{
	Vector2 points[4]; GetRotatedPosArray(points);
	Vector2 otherPoints[4]; other.GetRotatedPosArray(otherPoints);

	if(PointInPolygon(points[0], otherPoints, 4)) { return true; }
	if(PointInPolygon(points[1], otherPoints, 4)) { return true; }
	if(PointInPolygon(points[2], otherPoints, 4)) { return true; }
	if(PointInPolygon(points[3], otherPoints, 4)) { return true; }

	if(PointInPolygon(otherPoints[0], points, 4)) { return true; }
	if(PointInPolygon(otherPoints[1], points, 4)) { return true; }
	if(PointInPolygon(otherPoints[2], points, 4)) { return true; }
	if(PointInPolygon(otherPoints[3], points, 4)) { return true; }

	return false;
}

bool pointInPolygonOnSegment(const Vector2 &p, const Vector2 &q, const Vector2 &r)
{
	return q.X <= std::max(p.X, r.X) && q.X >= std::min(p.X, r.X) &&
				 q.Y <= std::max(p.Y, r.Y) && q.Y >= std::min(p.Y, r.Y);
}

int pointInPolygonOrientation(const Vector2 &p, const Vector2 &q, const Vector2 &r)
{
	float val = (q.Y - p.Y) * (r.X - q.X) - (q.X - p.X) * (r.Y - q.Y);
	if(val == 0) {
		return 0;
	} else if(val > 0) {
		return 1;
	}
	return 2;
}

bool pointInPolygonDoIntersect(const Vector2 &p1, const Vector2 &q1, const Vector2 &p2, const Vector2 &q2)
{
	int o1 = pointInPolygonOrientation(p1, q1, p2);
	int o2 = pointInPolygonOrientation(p1, q1, q2);
	int o3 = pointInPolygonOrientation(p2, q2, p1);
	int o4 = pointInPolygonOrientation(p2, q2, q1);
	return
		(o1 != o2 && o3 != o4) ||
		(o1 == 0 && pointInPolygonOnSegment(p1, p2, q1)) ||
		(o2 == 0 && pointInPolygonOnSegment(p1, q2, q1)) ||
		(o3 == 0 && pointInPolygonOnSegment(p2, p1, q2)) ||
		(o4 == 0 && pointInPolygonOnSegment(p2, q1, q2));
}

/**
 * Return whether the given point `p` is in the given polygon `poly`.
 * Uses above pointInPolygon-prefixed functions as well.
 * Source: http://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
 * This appears to work a lot better than what we were using before: http://alienryderflex.com/polygon/
 */
bool RectangleF::PointInPolygon(const Vector2 &p, const Vector2* poly, int vertices)
{
	if(vertices < 3)
		return false;

	Vector2 extreme(10000.0f, p.Y);
	int count = 0;
	int i = 0;
	do {
		int next = (i + 1) % vertices;
		if(pointInPolygonDoIntersect(poly[i], poly[next], p, extreme)) {
			if(pointInPolygonOrientation(poly[i], p, poly[next]) == 0) {
				return pointInPolygonOnSegment(poly[i], p, poly[next]);
			}
			count++;
		}
		i = next;
	} while(i != 0);
	
	return count & 1;
}
