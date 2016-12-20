#ifndef LITERAL_H
#define LITERAL_H

#include "variable.h"

class Literal
{
	private:
		
		Variable m_var;
		bool m_negative;
	
	public:
		
		Literal (const Variable & var, bool negative = false):
			m_var (var),
			m_negative (negative)
		{}
		

		Literal (const Literal & l):
			m_var (l.m_var),
			m_negative (l.m_negative)
		{}
	
		
		const Variable & getVar () const
		{
			return m_var;
		}

		bool operator == (const Literal &) const;
		bool operator < (const Literal &) const;
		bool isConjugate (const Literal &) const;
		bool isNegative () const;

		Literal operator *() const;
};

std::ostream & operator << (std::ostream &, const Literal &);
#endif
