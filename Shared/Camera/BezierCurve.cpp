#include "BezierCurve.h"
#include "CamCalcTypes.h"
#include <cmath>

double BezierCurve::factorial(int n)
{
	// TODO: HANDLE EXCEPTIONS
	// if (n < 0) { throw new Exception("n is less than 0"); }
	// if (n > 32) { throw new Exception("n is greater than 32"); }

	return this->FactorialTable[n]; /* returns the value n! as a SUMORealing point number */
}

double BezierCurve::Ni(int n, int i)
{
	return this->factorial(n) / (this->factorial(i) * this->factorial(n - i));
}

double BezierCurve::Bernstein(int n, int i, double t)
{
	double basis;
	double ti; /* t^i */
	double tni; /* (1 - t)^i */

	/* Prevent problems with pow TODO: What problems with pow? */

	if (t == 0.0 && i == 0)
		ti = 1.0;
	else
		ti = pow(t, i);

	if (n == i && t == 1.0)
		tni = 1.0;
	else
		tni = pow((1 - t), (n - i));

	//Bernstein basis
	basis = this->Ni(n, i) * ti * tni;
	return basis;
}

void BezierCurve::Bezier2D(Vector2* b, size_t arrSize, int cpts, double* p)
{
	int npts = 0;
	int icount = 0;
	const double step = 1.0 / (cpts - 1);
	double t = 0.0;

	for (int k = 0; k < arrSize; k++) {
		if (b[k] == nullptr) // TODO: Is this even a nullptr?? Actually, do we need a pointer array..?
			continue;
		else
			npts++;
	}

	for (int i = 0; i != cpts; i++)
	{
		if ((1.0 - t) < 5e-6)
			t = 1.0;

		p[icount] = 0.0;
		p[icount + 1] = 0.0;
		for (int j = 0; j != npts; j++)
		{
			double basis = this->Bernstein(npts - 1, j, t);
			p[icount] += basis * b[j].x;
			p[icount + 1] += basis * b[j].y;
		}
		icount += 2;
		t += step;
	}
}
