#ifndef CLAUSES_H
#define CLAUSES_H

#include "clause.h"
#include <functional>

class Clauses : public std::set < Clause >
{
	public:

		Clauses operator /(const Literal &) const;	
		Clauses & operator += (const Clauses & );
		Clauses operator +(const Clauses & ) const;
		Clauses operator * (const Clauses & clauses);
		iterator find_if (const std::function < bool (const Clause &) > &) const;
		bool exists (const iterator &) const;
		Clauses getMatches (const std::function < bool (const Clause &) > &) const;
		Clause::iterator getPureLiteral () const;
		bool hasPureLiteral () const;
		bool hasEmptyClause () const;
};

std::ostream & operator << (std::ostream &, const Clauses & );

#endif
