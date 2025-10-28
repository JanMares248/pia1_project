
//using coordinates = double[];  //mnenitelne typy
//using speeds = double[];
//using mass = double;
#include <array>
#include <vector>
#include <cmath>;
class Teleso
{
public:
	double r[2];//coordinates r;
	double p[2];//momentum p;
	double m;//mass m;
	//double* hamiltonian; //pointer, asi hodit do metody Hamiltonian
	double nr[2];//coordinates r; //nova vypoctova vrstva
	double np[2];//momentum p;
	double nm;//mass m;

	//pripravene vysledne veliciny
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

	// metody
	static double Distance(int i, int j)
	{
		double v = sqrt(pow(2.0, Telesa[i].r[0]) + pow(2.0, Telesa[j].r[0]) + pow(2.0, Telesa[i].r[1]) + pow(2.0, Telesa[j].r[1]));
		return v;
	}
	static void Add(double x, double y, double px, double py, double ms)
	{
		Teleso newTel(x, y, px, py, ms);
		Telesa.push_back(newTel);
	}
	static double Hamiltonian()//vypocet hamiltonianu
	{
		double hamiltonian;
		double grav = 0;
		for (int i = 0; i < Telesa.size(); i++)
		{
			for (int j = 0; j < Telesa.size();j++)
			{
				if (i < j)
				{
					grav = 6.6743e-11 * Telesa[i].m * Telesa[j].m / Distance(i, j);
				}
				else
				{
					grav = 0;
				}
				hamiltonian += (pow(2.0, Telesa[i].p[0]) + pow(2.0, Telesa[i].p[1])) / (2 * Telesa[i].m) + grav;
			}
		}
		return hamiltonian;
	};
	void Dr()
	{
		this->dr;
		for (int d = 0; d < 2;d++)
		{
			dr[d] = 0.0;
		}
		for (int d = 0; d < 2;d++)
		{
			for (int i = 0;i < Telesa.size();i++)
			{
				dr[d] = Telesa[i].p[d] / Telesa[i].m;
			}
		}
	}
	void Dp()
	{
		this->dp;
		for (int d = 0; d < 2;d++)
		{
			dp[d] = 0.0;
		}
		for (int d = 0; d < 2;d++)
		{
			for (int i = 0;i < Telesa.size();i++)
			{
				for (int j = 0;j < Telesa.size();j++)
				{
					if (i != j)
					{
						dp[d] = (-6.6743e-11 * Telesa[i].m * Telesa[j].m / pow(Distance(i, j), 3)) * (Telesa[i].r - Telesa[j].r);
					}
				}
			}
		}
	}
	static void SolveEuSymp(double timeStep, double end)
	{
		int duration = ceil(end / timeStep); //vim bude zaokrohlovat
		for (int t = 0; t < duration; t++)  //mirne nechapu prevod z shift space do souradnic
		{

		}
	};
	//pokud budete chtit dodelat solvery, tak mohme tady
};