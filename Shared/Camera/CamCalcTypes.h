#ifndef CONNECTIVITY_CAMCALCTYPES_H
#define CONNECTIVITY_CAMCALCTYPES_H

struct PathPoint {
	double Slope;
	double Length;
};

struct Vector2 {
	double x;
	double y;

	bool operator==(const Vector2& a) const
	{
		return (a.Y == y);
	}

};

#endif //CONNECTIVITY_CAMCALCTYPES_H
