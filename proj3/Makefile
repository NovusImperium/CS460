P2.out : Project2.o dfa.o syn.o SetLimits.o
	gcc -o P2.out Project2.o dfa.o syn.o SetLimits.o -g

Project2.o : Project2.c dfa.h syn.h
	gcc -c Project2.c -g

syn.o : syn.c syn.h
	gcc -c syn.c -g

dfa.o : dfa.c dfa.h
	gcc -c dfa.c -g

SetLimits.o : SetLimits.c SetLimits.h
	gcc -c SetLimits.c -g

clean :
	rm -f *.o *.out *.*~ *~ *.lst *.dbg

veryclean :
	rm -f *.in

run :
	./P02.out

submit: Project2.c dfa.c dfa.h syn.c syn.h Makefile SetLimits.c SetLimits.h
	rm -rf olsonP2
	mkdir olsonP2
	cp Project2.c olsonP2
	cp dfa.c olsonP2
	cp dfa.h olsonP2
	cp syn.c olsonP2
	cp syn.h olsonP2
	cp Makefile olsonP2
	cp SetLimits.c olsonP2
	cp SetLimits.h olsonP2
	tar cfvz olsonP2.tgz olsonP2
	cp olsonP2.tgz ~tiawatts/cs460drop
