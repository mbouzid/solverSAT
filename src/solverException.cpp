#include "solverException.h"

const char * SolverException::what () const throw ()
{
	return m_what.c_str();
}
	
