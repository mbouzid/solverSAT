#include "variable.h"

bool Variable::operator == (const Variable & v) const
{
	return ( m_id == v.m_id );
}

bool Variable::operator != (const Variable & v) const
{
	return ( m_id != v.m_id );
}

bool Variable::operator < (const Variable & v) const
{
	return ( m_id < v.m_id );
}

std::ostream & operator << (std::ostream & os, const Variable & v)
{
	return os << v.getID() ;
}
