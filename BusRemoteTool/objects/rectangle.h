#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "../../Shared/Connectivity.h"

class Vector2 {
public:
	float X;
	float Y;
	
	Vector2();
	Vector2(float x, float y);
	Vector2 operator+ (const Vector2& other) const;
};

class RectangleF {
public:
	float x;
	float y;
	float w;
	float h;

	float rotation;
	bool anchorInCenter;

public:
	RectangleF();
	RectangleF(const Rectangle_T& rawrect);
	~RectangleF();

	void Translate(const Vector2 &v);
	void Rotate(float fDegrees);

	Vector2 GetPos() const;
	Vector2 GetSize() const;

	void GetRotatedPosArray(Vector2* pArray) const; // requires array size of at least (typeof(Vector2)*4)

	bool Contains(const Vector2 &point);

	bool Intersects(const RectangleF &other);

	static bool PointInPolygon(const Vector2 &p, const Vector2* poly, int vertices);
};

#endif // RECTANGLE_H
