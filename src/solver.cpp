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
	
		Clause result (A*B);
		
		std::cout << "res=" << result << std::endl;
		resolvents.insert(result);
			
		//std::cout << resolvents << std::endl;
	}
	
	return "unsatisfiable";
}


Solver::Solver (const Clauses & clauses, const std::map < Variable, size_t > & variables):
	m_clauses (clauses),
	m_variables (variables)
{}

Solver * Solver::importFromDimacs (const char * filename) throw (SolverException &)
{
	std::ifstream f (filename);

	if ( f )
	{
		Clauses clauses	;
		std::map < Variable, size_t > variables; 

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

				if ( variables.find (x) != variables.end() )
					variables[x] = variables[x] + 1;
				else
					variables [x] = 0;
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

std::string Solver::DLL ()
{
	bool continuer (true);
	while (continuer)
	{

		if ( m_clauses.empty() )
		{
			return "satisfiable";
		}//end if
		
		// check empty clause []
		if ( m_clauses.hasEmptyClause() )
			return "unsatisfiable";

		// check if [_l] and [l] exists
		Clauses::iterator foundUnit = m_clauses.find_if (Clause::isUnit);
		if ( m_clauses.exists (foundUnit) )
		{
			Clause c (*foundUnit);
			Literal l (c.getFirst());
			Literal _l (*l);

			auto clauseWithConjugate = [&_l] (const Clause & c)
			{
				return ( Clause::isUnit(c) and c.contains (_l));	
			};

			auto found = m_clauses.find_if (clauseWithConjugate);
			if (m_clauses.exists (found))
				return "unsatisfiable";
		}
	
		continuer = false;


		Clauses::iterator foundUnitClause = m_clauses.find_if (Clause::isUnit);
		
		if ( m_clauses.exists(foundUnitClause) )
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

			Clauses matchesWithoutLiterals (m_clauses.getMatches (clauseWithoutLiterals));
			Clauses matchesWithConjugate (m_clauses.getMatches (clauseWithConjugate));

			matchesWithConjugate = matchesWithConjugate/_l;

			m_clauses = matchesWithoutLiterals + matchesWithConjugate;
			
			continuer = true;		
		}
		else 
		if ( m_clauses.hasPureLiteral() )
		{
			
			Literal l (*m_clauses.getPureLiteral());		
			m_clauses = m_clauses/l;
			continuer = true;
		}
	}
	auto maxOcc = [] (const std::pair < Variable, size_t > & p1, const std::pair < Variable, size_t > & p2 )
	{
		return ( p1.second > p2.second);
	};
	std::vector < std::pair < Variable, size_t > > vars (m_variables.begin(), m_variables.end());

	std::sort(vars.begin(), vars.end(), maxOcc);
	
	Variable x (vars[0].first);

	Literal l (x) ;
	Literal _l = *l;

	auto withl = [&l] (const Clause & c) 
	{	
		return ( c.contains (l) );
	};

	auto with_l = [&_l] (const Clause & c)
	{
		return ( c.contains (_l) );
	};

	Clauses S_l (m_clauses.getMatches (withl) );
	Clauses S__l (m_clauses.getMatches (with_l));
		
	auto clauseWithoutLiterals = [&l,&_l] (const Clause & c)
	{
		return ( !c.contains (l) and !c.contains (_l) );
	};
	
	Clauses withoutl (m_clauses.getMatches (clauseWithoutLiterals));
		
	std::map < Variable, size_t > vars1, vars2;

		
	Clauses case1 = (S__l/_l);
	case1 = case1 + withoutl ;
	for (auto cl : case1)	
	{
		for (auto lit : cl)
		{
			if ( vars1.find (lit.getVar()) != vars1.end()) 
				vars1 [lit.getVar()] = vars1[lit.getVar()]+1;
			else
				vars1 [lit.getVar()] = 0;
		}
	}

	Clauses case2 = (S_l/l);
	case2 = case2 + withoutl;
	for (auto cl : case2)	
	{
		for (auto lit : cl)
		{
			if ( vars2.find ( lit.getVar()) != vars2.end() ) 
				vars2 [lit.getVar()] = vars2[lit.getVar()]+1;
			else
				vars2 [lit.getVar()] = 0;
		}
	}
	std::cout << "case1:" << std::endl;
	std::cout << "\t" << case1.size() << std::endl;
	
	std::cout << "case2:" << std::endl;
	std::cout << "\t" << case2.size() << std::endl;
	

	Solver s1 (case1, vars1);
	Solver s2 (case2, vars2);

	if ( (s1.DLL () == "satisfiable") or ( s2.DLL  () == "satisfiable" ) )
			return "satisfiable";
		else
			return "unsatisfiable";
}


























std::ostream & operator << (std::ostream & os, const Solver & s)
{
	return os << s.getClauses();
}
