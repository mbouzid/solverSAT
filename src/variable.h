#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream>

class Variable
{
	private:
		
		unsigned long int m_id;
	
	public:
		
		Variable (unsigned long int id):
			m_id (id)
		{}

		Variable (const Variable & v):
			m_id (v.m_id)
		{}

		unsigned long int getID () const
		{
			return m_id;
		}
		
		bool operator == (const Variable &) const;
		bool operator != (const Variable &) const;
		bool operator < (const Variable &) const;

		~Variable()
		{}
};

std::ostream & operator << (std::ostream &, const Variable &);

#endif
