## 

CXX				= g++
CFLAGS			= -Wall -std=c++1y -g
OFLAGS			= -O2

## 

TGT				=	build/solver
OBJ				=	obj/main.o \
					obj/solver.o \
					obj/solverException.o \
					obj/variable.o \
					obj/literal.o \
					obj/clause.o \
					obj/clauses.o 
## 

all:	$(TGT)
	@echo ":)"

##

$(TGT):	$(OBJ)
	$(CXX) -o $@ $^ -lpthread

##

obj/%.o:	src/%.cpp
	$(CXX) -c $(CFLAGS) $(OFLAGS) -o $@ $<

##

clean:	$(TGT) $(OBJ)
	rm $^
