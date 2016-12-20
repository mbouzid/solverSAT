#include "solver.h"

std::vector < std::string > split (std::string const & _data, std::string const & _separator)
{
	std::string dcopy = _data;
    std::vector<std::string> output;
    size_t pos;
    while (pos != std::string::npos)
	{
        pos = dcopy.find(_separator);
        output.push_back(dcopy.substr(0, pos));
        if (std::string::npos != pos)
            dcopy = dcopy.substr(pos + _separator.size());
	}
    return output;
}

std::string Solver::Robinson (Clauses & S)
{
	Clauses resolvents (S);

	std::random_device rd;
	std::mt19937 gen (rd());
	

	
	while ( !resolvents.hasEmptyClause() )
	{

		// insérer heuristique ici
		std::uniform_int_distribution<size_t> unif (0, resolvents.size()-1);
		Clauses::iterator first = resolvents.begin();
		size_t i = unif (gen);


		std::advance(first, i); 	
		Clause A (*first);
		Clause B;


		for (const Literal & a : A)
		{
			auto clauseWithConjugate = [&a] (const Clause & c)
			{
				return ( c.contains (*a) );
			};

			auto findClauseWithConjugate = resolvents.find_if (clauseWithConjugate);
			if ( resolvents.exists (findClauseWithConjugate) )
			{
				B = *findClauseWithConjugate;
			}
		}

		if ( B.empty() )
		{
			size_t j = unif (gen);

			first = resolvents.begin();
			std::advance (first, j);
			B = *first;
		}

		std::cout << "A=" << A << std::endl;
		std::cout << "B=" << B << std::endl;
	
		Clause result;
		for (auto a : A)
		{
			auto conjugateLiteral = [&a] (const Literal & b)
			{
				return ( a.isConjugate (b) );
			};
	

			auto findConjugateLiteral = std::find_if (B.begin(), B.end(), conjugateLiteral);
			
			if ( findConjugateLiteral != B.end() )
			{
				std::cout << "find: " << *findConjugateLiteral << std::endl; 
			}
			else
			{
				result.insert (a);
			}

		}

		for (auto b: B)
		{
			auto conjugateLiteral = [&b] (const Literal & a)
			{
				return ( b.isConjugate (a) );
			};
	

			auto findConjugateLiteral = std::find_if (A.begin(), A.end(), conjugateLiteral);
			
			if ( findConjugateLiteral != A.end() )
			{
				std::cout << "find: " << *findConjugateLiteral << std::endl; 
			}
			else
			{
				result.insert (b);
			}	
		}


		std::cout << "res=" << result << std::endl;
		resolvents.insert(result);
			
		std::cout << resolvents << std::endl;
	}
	
	return "unsatisfiable";
}


Solver::Solver (const Clauses & clauses, const std::set < Variable > & variables):
	m_clauses (clauses),
	m_variables (variables)
{}

Solver * Solver::importFromDimacs (const char * filename) throw (SolverException &)
{
	std::ifstream f (filename);

	if ( f )
	{
		Clauses clauses	;
		std::set < Variable > variables; 

		std::string line;
		std::getline (f,line); // saut première ligne

		while ( std::getline (f, line) )
		{
			std::vector < std::string > tokens (split (line, " "));
			Clause c;
			auto first = tokens.begin(); 
			auto last = tokens.end();

			for (auto i = first; i != std::prev (last); ++i)
			{
				long int id = std::stoi (*i);
				bool neg = false;
				if ( id < 0 )
				{
					id = abs (id);
					neg = true;
				}

				Variable x (id);
				Literal l (x, neg);

				variables.insert (x);
				c.insert(l);
				
			}
				clauses. insert (c);
		}
	
		f.close();

		return new Solver (clauses, variables);
	}
	else
		throw SolverException ("cannot read the input file");
}

std::string Solver::DLL (Clauses & S)
{
	bool continuer (true);
	while (continuer)
	{

		if ( S.empty() )
		{
			return "satisfiable";
		}//end if
		
		// check empty clause []
		if ( S.hasEmptyClause() )
			return "unsatisfiable";

		// check if [_l] and [l] exists
		Clauses::iterator foundUnit = S.find_if (Clause::isUnit);
		if ( S.exists (foundUnit) )
		{
			Clause c (*foundUnit);
			Literal l (c.getFirst());
			Literal _l (*l);

			auto clauseWithConjugate = [&_l] (const Clause & c)
			{
				return ( Clause::isUnit(c) and c.contains (_l));	
			};

			auto found = S.find_if (clauseWithConjugate);
			if (S.exists (found))
				return "unsatisfiable";
		}
	
		continuer = false;


		Clauses::iterator foundUnitClause = S.find_if (Clause::isUnit);
		
		if ( S.exists(foundUnitClause) )
		{
			Clause UnitClause (*foundUnitClause);
			Literal l (UnitClause.getFirst());
			Literal _l (*l);

			auto clauseWithoutLiterals = [&l, &_l] (const Clause & c)
			{
				return ( !c.contains (l) and !c.contains (_l) );
			};

			auto clauseWithConjugate = [&_l] (const Clause & c)
			{
				return ( c.contains (_l) );
			};

			Clauses matchesWithoutLiterals (S.getMatches (clauseWithoutLiterals));
			Clauses matchesWithConjugate (S.getMatches (clauseWithConjugate));

			matchesWithConjugate = matchesWithConjugate/_l;

			S = matchesWithoutLiterals + matchesWithConjugate;
			
			continuer = true;		
		}
		else 
		if ( S.hasPureLiteral() )
		{
			
			Literal l (*S.getPureLiteral());		
			S = S/l;
			continuer = true;
		}
	}
	
	Literal l ((*S.begin()).getFirst());
	Literal _l = *l;

	auto withl = [&l] (const Clause & c) 
	{	
		return ( c.contains (l) );
	};

	auto with_l = [&_l] (const Clause & c)
	{
		return ( c.contains (_l) );
	};

	Clauses S_l (S.getMatches (withl) );
	Clauses S__l (S.getMatches (with_l));
		
	auto clauseWithoutLiterals = [&l,&_l] (const Clause & c)
	{
		return ( !c.contains (l) and !c.contains (_l) );
	};
	
	Clauses withoutl (S.getMatches (clauseWithoutLiterals));
	
	Clauses case1 = (S__l/_l);
	case1 = case1 + withoutl ;
	Clauses case2 = (S_l/l);
	case2 = case2 + withoutl;
	
	std::cout << "case1:" << std::endl;
	std::cout << "\t" << case1.size() << std::endl;
	
	std::cout << "case2:" << std::endl;
	std::cout << "\t" << case2.size() << std::endl;


	if ( (DLL (case1) == "satisfiable") or ( DLL  (case2) == "satisfiable" ) )
		return "satisfiable";
	else
		return "unsatisfiable";
}


























std::ostream & operator << (std::ostream & os, const Solver & s)
{
	return os << s.getClauses();
}
