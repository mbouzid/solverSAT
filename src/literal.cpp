#include "literal.h"

bool Literal::operator == (const Literal & l) const
{
	return ( (m_var == l.m_var) and (m_negative == l.m_negative));
}

bool Literal::operator < (const Literal & l) const
{
	return ( (m_var < l.m_var) or (m_negative < l.m_negative));
}

bool Literal::isConjugate (const Literal & l) const
{
	return ( ( m_var == l.m_var ) and ( m_negative != l.m_negative ) );
}

bool Literal::isNegative () const
{
	return m_negative;
}

Literal Literal::operator *() const
{
	if ( isNegative() )
		return Literal (m_var);
	else
		return Literal (m_var, true);
}

std::ostream & operator << (std::ostream & os, const Literal & l)
{
	return os << ( l.isNegative() ? "-" : "" ) << l.getVar() ;
}
