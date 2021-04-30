#output: main.o scheduling.o
	#g++ main.o scheduling.o -o main

#main.o: main.cpp
	#g++ -c main.cpp

scheduling.o: scheduling.cpp scheduling.h
	g++ -c scheduling.cpp

clean:
	rm -f *.o
