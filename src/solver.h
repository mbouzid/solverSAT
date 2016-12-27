#ifndef SOLVER_H
#define SOLVER_H

#include <cstdlib>
#include "clauses.h"
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <iterator>
#include <unistd.h>
#include <map>

#include "solverException.h"

class Solver
{
	private:
		
		Clauses m_clauses;
		std::map < Variable, size_t > m_variables;

		Solver (const Clauses & clauses, const std::map <Variable, size_t> & variables);

	public:
		
		static Solver * importFromDimacs (const char * filename) throw (SolverException &);
		const Clauses & getClauses () const
		{
			return m_clauses;
		}
		static std::string Robinson (Clauses & S);	
		std::string DLL ();
		const std::map < Variable, size_t > & getVariables () const
		{
			return m_variables;
		}
};

std::vector< std::string > split (std::string const & _data, std::string const & _separator);
std::ostream & operator << (std::ostream & os, const Solver & s);
#endif
