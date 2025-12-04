#pragma once

#include <vector>
#include <cmath>

const double G = 6.6743e-11;

class Teleso
{
public:
	double x[2];
	double v[2];
	double m;
	double nr[2];
	double nv[2];
	double nm;


	static std::vector<Teleso> Bodies;
	static double dr[2];
	static double dp[2];

	Teleso(double x, double y, double px, double py, double ms)
	{
		x[0] = x;
		x[1] = y;
		v[0] = px;
		v[1] = py;
		m = ms;
	}


	static double Distance(int i, int j);
	static void Add(double x, double y, double px, double py, double ms);
	static double Hamiltonian();

	static void CalculateDr();
	static void CalculateDp();
	static void SolveEuSymp(double timeStep, double end);
};


namespace NBody
{
	void InitializeSimulationData();
}