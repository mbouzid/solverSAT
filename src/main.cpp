#include "solver.h"

int main (int argc, char * argv [])
{
	if ( argc > 2 )
	{
		
		Solver * SAT = Solver::importFromDimacs (argv[1]);
		
		std::string mode (argv[2]);

		if ( mode != "--rr" and mode != "--dll" and mode != "--help" )
			std::cerr << "option invalide!" << std::endl;

		if ( mode == "--help" )
		{
			std::cout << "options: --rr (Résolvante de Robinson) ou --dll (Méthode Davis Logemann Loveland) --help" << std::endl;
			std::cout << "Utilisation: ./build/solver [.dimacs] [option]" << std::endl;

			exit (EXIT_SUCCESS);
		}

		
		Clauses c (SAT->getClauses());
		//std::cout << c << std::endl;
	

		if ( mode == "--rr" )
		{
			std::cout << Solver::Robinson (c) << std::endl;
		}
		else
			if ( mode == "--dll" )
			{
				std::cout << Solver::DLL (c) << std::endl;
			}

		delete SAT;

		return EXIT_SUCCESS;
	}
	else
	{
		std::cerr << "options: --rr (Résolvante de Robinson) ou --dll (Méthode Davis Logemann Loveland)" << std::endl;
		std::cerr << "Utilisation: ./build/solver [.dimacs] [option]" << std::endl;
		exit (EXIT_FAILURE); 
	}
}
