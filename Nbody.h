#pragma once

#include <vector>
#include <cmath>

const double G = 6.6743e-11;

class Teleso
{
public:
	double r[2];
	double p[2];
	double m;
	double nr[2];
	double np[2];
	double nm;


	static std::vector<Teleso> Telesa;
	static double dr[2];
	static double dp[2];

	Teleso(double x, double y, double px, double py, double ms)
	{
		r[0] = x;
		r[1] = y;
		p[0] = px;
		p[1] = py;
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