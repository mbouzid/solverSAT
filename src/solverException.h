#ifndef SOLVER_EXCEPTION_H
#define SOLVER_EXCEPTION_H

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

class SolverException : public std::exception
{
	private:
		
		std::string m_what;

	public:
		
		SolverException (const std::string & what):
			m_what (what)
		{}
		
		SolverException (const SolverException & e):
			m_what (e.m_what)
		{}

		const char * what () const throw ();
	
		~SolverException () throw ()
		{}
};

#endif
