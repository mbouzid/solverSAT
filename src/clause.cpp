#include "clause.h"

bool Clause::isUnit (const Clause & c)
{
	return ( c.size () == 1 );
}
std::ostream & operator << (std::ostream & os, const Clause & c)
{
	if ( c.empty() )
		os << "[]" ;
	else
	{
		auto first = c.begin();
		auto second = std::next (first);
		auto last = c.end();
		os << "[" << *first ;
		for (auto i = second; i != last; ++i)
			os << ";" << *i;	
		os << "]";
	}

	return os;
}


bool Clause::contains (const Literal & l) const
{
	return ( std::find (begin(), end(), l) != end() );
}

const Literal & Clause::getFirst () const
{
	return *begin();
}

Clause & Clause::operator +=(const Literal & l)
{
	insert (l);
	return *this;
}

Clause Clause::operator +(const Clause & c)
{
	Clause res (*this);
	for (const Literal & l : c)
		res.insert (l);
	return res;
}

Clause Clause::operator *(const Clause & c)
{
	Clause res (*this);
	for (const Literal & l : c)
	{ 
		Literal _l (*l);
		if ( (c.contains (l) and res.contains (_l)))
		{
			res-=_l;
		}
		else 
			res+=l;
	}

	return res;
}

Clause & Clause::operator +=(const Clause & c)
{
	for (const Literal & l : c)
		insert (l);
	return *this;
}


Clause & Clause::operator -=(const Literal & l)
{	
	erase (l);
	return *this;
}


Clause Clause::operator /(const Literal & l) const
{
	Clause c (*this);
	c.erase (l);
	return c;
}
