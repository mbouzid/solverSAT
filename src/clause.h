#ifndef CLAUSE_H
#define CLAUSE_H

#include <set>
#include "literal.h"
#include <algorithm>
#include <iterator>

class Clause : public std::set <Literal>
{
	public:
		Clause operator /(const Literal &) const;
		Clause & operator +=(const Literal &);
		Clause & operator +=(const Clause & c);
		Clause operator +(const Clause &);
		Clause operator *(const Clause &);
		Clause & operator -=(const Literal &);
		const Literal & getFirst () const;
		static bool isUnit (const Clause &);
		bool contains (const Literal & l) const;
};

std::ostream & operator << (std::ostream &, const Clause &);
#endif
