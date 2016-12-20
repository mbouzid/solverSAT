#include "clauses.h"

std::ostream & operator << (std::ostream & os, const Clauses & c)
{
	if ( c.empty() )
		os << "{}" ;
	else
	{
		auto first = c.begin();
		auto second = std::next (first);
		auto last = c.end();
		os << "{" << *first ;
		for (auto i = second; i != last; ++i)
			os << "," << *i;	
		os << "}";
	}

	return os;
}

bool Clauses::exists (const iterator & i) const
{	
	return ( i != end() );
}

Clauses Clauses::operator /(const Literal & l) const
{
	Clauses C;
	for (const Clause & c : *this)
	{
		C.insert (c/l);	
	}
	return C;
}

Clause::iterator Clauses::getPureLiteral () const
{
	Clause::iterator foundLiteral;
	for (Clauses::iterator i = begin(); i != end(); ++i)
	{
		for (Clause::iterator j = (*i).begin(); j != (*i).end(); ++j)
		{
			Literal pivot (*j);
			
			auto hasPivotConjugate = [&pivot] (const Clause & c)
			{
				return ( c.contains (*pivot));	
			};

			auto found = std::find_if (begin(), end(), hasPivotConjugate);
			
			if ( found == end() )
			{
				foundLiteral = j;
				break;
			}
		}
		
	}
	
	return foundLiteral;
}

bool Clauses::hasEmptyClause () const
{
	auto emptyClause = [] (const Clause & c)
	{
		return (c.empty());
	};

	auto foundEmptyClause = find_if (emptyClause);
		
	return exists(foundEmptyClause);
}

bool Clauses::hasPureLiteral () const
{
	Clause::iterator foundLiteral;
	for (Clauses::iterator i = begin(); i != end(); ++i)
	{
		for (Clause::iterator j = (*i).begin(); j != (*i).end(); ++j)
		{
			Literal pivot (*j);
			
			auto hasPivotConjugate = [&pivot] (const Clause & c)
			{
				return ( c.contains (*pivot));	
			};

			auto found = std::find_if (begin(), end(), hasPivotConjugate);
			
			if ( found == end() )
			{
				return true;
			}
		}
		
	}
	
	return false; 

}

Clauses & Clauses::operator +=(const Clauses & clauses)
{
	std::set_union (begin(), end(), clauses.end(), clauses.end(), std::inserter(*this, begin()) );

	return *this;
}

Clauses Clauses::operator +(const Clauses & c) const
{
	Clauses result;
	std::set_union (begin(), end(), c.begin(), c.end(), std::inserter (result, result.begin()));
	return result;
}

Clauses::iterator Clauses::find_if (const std::function < bool (const Clause &) > & unaryPredicate ) const
{
	return (std::find_if (begin(), end(), unaryPredicate));
}

Clauses Clauses::operator * (const Clauses & clauses)
{
	if ( (empty() and !clauses.empty()) or (!empty() and clauses.empty()) )
		return Clauses();
	else
	{
		if ( size() == 1 )
		{
			Clause c (*begin());
			if ( c.empty() )
				return clauses;
		}
		else
		if ( clauses.size() == 1 )
		{
			Clause c (*clauses.begin());
			if ( c.empty())
				return *this;
		}
		
		Clauses S;
		for (Clause a : *this)
		{
			for (const Clause & b: clauses)
			{
				S.insert (a+b);
			}
		}

		return S;
	}
}

Clauses Clauses::getMatches (const std::function < bool (const Clause &) > & unaryPredicate ) const
{
	Clauses result;
	auto first = begin();
	auto last = end();
	
	while ( first != last )
	{
		first = std::find_if (first, last, unaryPredicate);
		if ( first != last )
			result.insert (*first++);
	}

	return result;
}

