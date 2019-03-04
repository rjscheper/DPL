dpl: recognizer.o lexer.o env.o evaluate.o
	g++ -Wall -g -std=c++11 recognizer.o lexer.o env.o evaluate.o -o dpl.out
recognizer.o: recognizer.cpp recognizer.h lexer.h env.h evaluate.h
	g++ -Wall -g -c -std=c++11 recognizer.cpp
lexer.o: lexer.cpp lexer.h
	g++ -Wall -g -c -std=c++11 lexer.cpp
env.o: env.cpp env.h lexer.h
	g++ -Wall -g -c -std=c++11 env.cpp
evaluate.o: evaluate.cpp evaluate.h lexer.h env.h
	g++ -Wall -g -c -std=c++11 evaluate.cpp
clean:
	rm -f *.o *.exe *.exe.stackdump *.out
error1:
	cat ./error1.dpl
error1x:
	./dpl ./error1.dpl
error2:
	cat ./error2.dpl
error2x:
	./dpl ./error2.dpl
error3:
	cat ./error3.dpl
error3x:
	./dpl ./error3.dpl
error4:
	cat ./error4.dpl
error4x:
	./dpl ./error4.dpl
error5:
	cat ./error5.dpl
error5x:
	./dpl ./error5.dpl
arrays:
	cat ./arrays.dpl
arraysx:
	./dpl ./arrays.dpl
conditionals:
	cat ./conditionals.dpl
conditionalsx:
	./dpl ./conditionals.dpl
recursion:
	cat ./recursion.dpl
recursionx:
	./dpl ./recursion.dpl
iteration:
	cat ./iteration.dpl
iterationx:
	./dpl ./iteration.dpl
functions:
	cat ./functions.dpl
functionsx:
	./dpl ./functions.dpl
lambda:
	cat ./lambda.dpl
lambdax:
	./dpl ./lambda.dpl