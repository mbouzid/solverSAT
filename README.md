# Solver SAT
Solver SAT basé sur la méthode de Davis, Logemann et Loveland; inclu la résolvante de Robinson.

### Installation

`
make -s
`

### Utilisation

Appliquer la résolvante de Robinson:

`
./build/solver [.dimacs] --rr 
`

Appliquer DLL: 

`
./build/solver [.dimacs] --dll
`

### Exemples

* SAT
	* examples/r125.1.dimacs (coloration du graphe r125.1 avec - son nombre chromatique - 5 couleurs)
	* examples/satexample.dimacs


* UNSAT
	* examples/r125.1\_4.dimacs	(coloration du graphe r125.1 avec 4 couleurs)
	* examples/unsatexample.dimacs

### Améliorations

* Etape de grounding 
* Résolvante de Robinson :
	* heuristique pour la recherche des clauses à réduire
	* parallélisation 

