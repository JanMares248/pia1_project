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

	void SolverWrite(std::ofstream file)
	// zapisuje do solverResult.txt ve formátu r1;r2 \n...pro kazde teleso... dp \n\n
		{
			
			for (int i = 0; i < Telesa.size(); i++)
			{
				for (int j = 0; j < dim; j++)
				{
					file << Telesa[i].nr[j];
					if (j==dim-1)
						{
							file << "\n";
						}
						else
						{
							file << ";";
						}
					}
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
			}
			file << "\n\n";
		}
	void SolveEuSymp(double timeStep, double end, int saveInterval)
	{
		std::ofstream file("solverResult.txt");
		int nSteps = ceil(end / timeStep); //vim bude zaokrohlovat
		std::cout <<"Create time"<< std::endl;
		for (int t = 0; t < nSteps; t++)  //mirne nechapu prevod z shift space do souradnic
			{
				std::cout <<t<< std::endl;
				// Telesa[i].Dp();
				// Telesa[i].Dr();  // update hodnot v každý čas
				for (int i = 0; i < Telesa.size(); i++) // pro každé těleso
					{
						Telesa[i].Dp();
						Telesa[i].Dr();  // update hodnot v každý čas
						for (int j = 0; j < dim; j++) // pro každou souřadnici
						{
							Telesa[i].np[j] = Telesa[i].p[j] + timeStep * Telesa[i].dp[j]; // spočte nové p a nové r
							Telesa[i].nr[j] = Telesa[i].r[j] + timeStep * Telesa[i].dr[j];
							if (t%saveInterval==0)
							{
										{
			
											for (int i = 0; i < Telesa.size(); i++)
											{
												for (int j = 0; j < dim; j++)
												{
													file << Telesa[i].nr[j];
													if (j==dim-1)
														{
															file << "\n";
														}
														else
														{
															file << ";";
														}
													}
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
											}
											file << "\n\n";
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
			}
			//t.p = t.np;
		}

	//pokud budete chtit dodelat solvery, tak mohme tady -> mluvil o normalnim eulerovy
};
};