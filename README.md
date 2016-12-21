# Solver SAT
Solver SAT basé sur la méthode de Davis, Logemann et Loveland; inclu la résolvante de Robinson.

### Installation

```
mkdir obj
make
```

### Utilisation

Appliquer la résolvante de Robinson:

```
./build/solver [.dimacs] --rr 
```

Appliquer DLL: 

```
./build/solver [.dimacs] --dll
`̀``


### Améliorations

Résolvante de Robinson :
	- heuristique pour la recherche des clauses à réduire
	- parallélisation 

