#include <array>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "Teleso.h"
class Case {
	public:
    const int dim = 2;
    std::vector<Teleso> Telesa;

    // static void Add(double x, double y, double px, double py, double ms)//Teleso t
	void Add(Teleso t)
        {
            // Teleso t(x, y, px, py, ms);
            Telesa.push_back(t);
        }

	// void SolverWrite(std::ofstream file)
	// // zapisuje do solverResult.txt ve formátu r1;r2 \n...pro kazde teleso... dp \n\n
	// 	{
			
	// 		for (int i = 0; i < Telesa.size(); i++)
	// 		{
	// 			for (int j = 0; j < dim; j++)
	// 			{
	// 				file << Telesa[i].nr[j];
	// 				if (j==dim-1)
	// 					{
	// 						file << "\n";
	// 					}
	// 					else
	// 					{
	// 						file << ";";
	// 					}
	// 				}
	// 			// pro pripad, ze bychom chteli vypisovat i p
	// 		// 	for (int j = 0; j < dim; j++)
	// 		// 	{
	// 		// 		MyFile << Telesa[i].np[j];
	// 		// 		if (j==dim-1)
	// 		// 			{
	// 		// 				file << "\n\n";
	// 		// 			}
	// 		// 			else
	// 		// 			{
	// 		// 				file << ";";
	// 		// 			}
	// 		// 		}
	// 		// file << Telesa[i].dp;
	// 		}
	// 		file << "\n\n";
	// 	}
	// double Hamiltonian()//vypocet hamiltonianu
	// {
	// 	double hamiltonian;
	// 	double grav = 0;
	// 	for (int i = 0; i < dim; i++)
	// 	{
	// 		for (int j = 0; j < dim;j++)
	// 		{
	// 			if (i < j)
	// 			{
	// 				grav = 6.6743e-11 * Telesa[i].m * Telesa[j].m / Distance(i, j);
	// 			}
	// 			else
	// 			{
	// 				grav = 0;
	// 			}
	// 			hamiltonian += (pow(2.0, Telesa[i].p[0]) + pow(2.0, Telesa[i].p[1])) / (2 * Telesa[i].m) + grav;
	// 		}
	// 	}
	// 	return hamiltonian;
	// };
	
	double Distance(int i, int j)
	{
		double v = sqrt(pow(Telesa[i].r[0]-Telesa[j].r[0],2.0) + pow(Telesa[i].r[1]-Telesa[j].r[1],2.0));
		return v;
	}
	void Dr()
	{
		// dr;
		// for (int d = 0; d < dim;d++)
		// {
		// 	dr[d] = 0.0;
		// }
		for (int d = 0; d < dim;d++) // pro každou souřadnici
		{
			for (int i = 0;i < Telesa.size();i++) // pro každé těleso
			{

				Telesa[i].dr[d] = Telesa[i].p[d] / Telesa[i].m;
				// if (d==1 && i==1)
				// {
				// std::cout <<Telesa[i].p[d]<< std::endl;
				// std::cout <<Telesa[i].m<< std::endl;
				// std::cout <<Telesa[i].dr[d]<< std::endl;
				// std::cout <<"\n"<< std::endl;
				// }
				


			}
		}
	}
	void Dp()
	{
		// for (int d = 0; d < dim;d++)
		// {
		// 	dp[d] = 0.0;
		// }
		for (int d = 0; d < dim;d++)
		{
			for (int i = 0;i < dim;i++)
			{
				Telesa[i].dp[d] = 0;
				for (int j = 0;j < dim;j++)
				{
					if (i != j)
					{
						Telesa[i].dp[d] = Telesa[i].dp[d] + (-6.6743e-11 * Telesa[i].m * Telesa[j].m / pow(Distance(i, j), 3)) * (Telesa[i].x - Telesa[j].x);
					}
					if (d==0)
					{
					std::cout <<Telesa[i].p[d]<< std::endl;
					std::cout <<Telesa[i].m<< std::endl;
					std::cout <<Telesa[i].dr[d]<< std::endl;
					std::cout <<"\n"<< std::endl;
					}
				}
			}
		}
	}
	double Dv(int telI,int coor){
		double dvi=0;
		double G=-6.6743e-11;
		// double G=-6.6743e2;
		for (int j = 0; j < Telesa.size(); j++)
		{
			if (j!=telI)
			{
				dvi+=(G*Telesa[telI].m*Telesa[j].m*(Telesa[j].r[coor]-Telesa[telI].r[coor]))/(pow(Telesa[j].r[coor]-Telesa[telI].r[coor],3)*Telesa[telI].m);
			}
		}
		return dvi;
		
	}
	void SolveEuSymp(double timeStep, double end, int saveInterval)
	{
		std::ofstream file("solverResult.txt");
		// print
											for (int i = 0; i < Telesa.size(); i++)
											{
												for (int j = 0; j < dim; j++)
												{
													file << Telesa[i].x[j];
													if (j==dim-1)
														{
															file << "\n";
														}
														else
														{
															file << ";";
														}
													}

											}
											file << "\n\n";
		int nSteps = ceil(end / timeStep); //vim bude zaokrohlovat
		std::cout <<"Create time"<< std::endl;
		for (int t = 0; t < nSteps; t++)  //mirne nechapu prevod z shift space do souradnic
			{
				// Telesa[i].Dp();
				// Telesa[i].Dr();  // update hodnot v každý čas
				for (int i = 0; i < Telesa.size(); i++) // pro každé těleso
					{
						Dp();
						Dr();  // update hodnot v každý čas
						Telesa[i].PolTransReversible(true);
						for (int j = 0; j < dim; j++) // pro každou souřadnici
						{
							Telesa[i].np[j] = Telesa[i].p[j] + timeStep * Telesa[i].dp[j]; // spočte nové p a nové r
							Telesa[i].nr[j] = Telesa[i].r[j] + timeStep * Telesa[i].dr[j];
							if (t%saveInterval==0)
							{
								// print
										{
			
											for (int i = 0; i < Telesa.size(); i++)
											{
												for (int j = 0; j < dim; j++)
												{
													file << Telesa[i].x[j];
													if (j==dim-1)
														{
															file << "\n";
														}
														else
														{
															file << ";";
														}
													}

											}
											file << "\n\n";
												// pro pripad, ze bychom chteli vypisovat i p
											// 	for (int j = 0; j < dim; j++)
											// 	{
											// 		MyFile << Telesa[i].np[j];
											// 		if (j==dim-1)
											// 			{
											// 				file << "\n\n";
											// 			}
											// 			else
											// 			{
											// 				file << ";";
											// 			}
											// 		}
											// file << Telesa[i].dp;
											// }
											// file << "\n\n";
										} // zapíše do souboru
															} 
														}
													}
			}
		for (int i = 0; i < Telesa.size(); i++)//for (Teleso t : Telesa) //-> nahrat nove do stare vrstvy, asi chybi.
		{
			for (int j = 0; j < dim; j++) // pro každou souřadnici
						{
			Telesa[i].p[j] = Telesa[i].np[j];
			Telesa[i].r[j] = Telesa[i].nr[j];
			}
			//t.p = t.np;
		}

	//pokud budete chtit dodelat solvery, tak mohme tady -> mluvil o normalnim eulerovy
};
	void SolveEuSymp2(double timeStep, double end, int saveInterval)
	{	
		std::ofstream file("solverResult.txt");
		int Nsteps=end/timeStep;
		for (int t = 0; t < Nsteps; t++)	//casova iterace
		{
			// std::cout <<"puvodni"<< std::endl;
			// std::cout <<Telesa[1].p[0]<< std::endl;
			// std::cout <<Telesa[1].p[1]<< std::endl;
			// std::cout <<"\n dv"<< std::endl;
			// std::cout <<Dv(1,0)<< std::endl;
			// std::cout <<Dv(1,1)<< std::endl;
			// std::cout <<"\n novy"<< std::endl;
			for (int telI = 0; telI < Telesa.size(); telI++)	//telesova iterace
			{
				for (int coor = 0; coor < dim; coor++)	//souradincova iterace rychlost
				{
					Telesa[telI].np[coor]=Telesa[telI].p[coor]+Dv(telI,coor)*timeStep;	// bacha, np a p jsou ted rychlosti aby se
					Telesa[telI].p[coor]=Telesa[telI].np[coor]; //rovnou prepise		// ty rovnice trochu zjednodusily,
				}																		// eventualne prejmenuju
				
				for (int coor = 0; coor < dim; coor++)	//souradincova iterace poloha
				{
					Telesa[telI].PolTransReversible(true);
					if (t%saveInterval==0)
					{
						file << Telesa[telI].x[coor];
						if (coor<dim-1)
						{
							file << ",";
						}
					}
					Telesa[telI].nr[coor]=Telesa[telI].r[coor]+Telesa[telI].np[coor]*timeStep;
					Telesa[telI].r[coor]=Telesa[telI].nr[coor];
				}
				if (telI!=Telesa.size()-1)
				{
					if (t%saveInterval==0){
						file << ";";
					}
				}
			}
			// std::cout <<Telesa[1].p[0]<< std::endl;
			// std::cout <<Telesa[1].p[1]<< std::endl;
			// std::cout <<"\n\n"<< std::endl;
			if (t%saveInterval==0){
				file << "\n";
			}
		}
	}
};