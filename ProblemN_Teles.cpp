//using coordinates = double[];  //mnenitelne typy
//using speeds = double[];
//using mass = double;
#include <array>
#include <vector>
#include <cmath>;
#include <iostream>
#include <fstream>
class Teleso
{
public:
	static const int dim=2;//dimenze, bude potřeba ještě předělat/ dát jinam
	double r[dim];//coordinates r;
	double p[dim];//momentum p;
	double m;//mass m;
	//double* hamiltonian; //pointer, asi hodit do metody Hamiltonian
	double nr[dim];//coordinates r; //nova vypoctova vrstva
	double np[dim];//momentum p;
	double nm;//mass m;

	//pripravene vysledne veliciny
	static std::vector<Teleso> Telesa;
	static double dr[dim];
	static double dp[dim];

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
	static void Dr()
	{
		dr;
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
	static void Dp()
	{
		dp;
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

	static void SolverWrite()
	// zapisuje do solverResult.txt ve formátu r1;r2 \n...pro kazde teleso... dp \n\n
		{
			std::ofstream MyFile("solverResult.txt");
			for (int i = 0; i < Telesa.size(); i++)
			{
				for (int j = 0; j < dim; j++)
				{
					MyFile << Telesa[i].nr[j];
					if (j==dim-1)
						{
							MyFile << "\n";
						}
						else
						{
							MyFile << ";";
						}
					}
				// pro pripad, ze bychom chteli vypisovat i p
				// for (int j = 0; j < dim; j++)
				// {
				// 	MyFile << Telesa[i].np[j];
				// 	if (j==dim-1)
				// 		{
				// 			MyFile << "\n\n";
				// 		}
				// 		else
				// 		{
				// 			MyFile << ";";
				// 		}
				// 	}
			}
			MyFile << dp;
			MyFile << "\n\n";
		}
	static void SolveEuSymp(double timeStep, double end, int saveInterval)
	{

		int nSteps = ceil(end / timeStep); //vim bude zaokrohlovat
		std::cout <<"Create time"<< std::endl;
		for (int t = 0; t < nSteps; t++)  //mirne nechapu prevod z shift space do souradnic
			{
				Dp();
				Dr();  // update hodnot v každý čas
				for (int i = 0; i < Telesa.size(); i++) // pro každé těleso
					{
						for (int j = 0; j < dim; j++) // pro každou souřadnici
						{
							Telesa[i].np[j] = Telesa[i].p[j] + timeStep * dp[j]; // spočte nové p a nové r
							Telesa[i].nr[j] = Telesa[i].r[j] + timeStep * dr[j];
							if (t%saveInterval==0)
							{
								SolverWrite(); // zapíše do souboru
							}
						}
					}
			}
	//pokud budete chtit dodelat solvery, tak mohme tady
};
};