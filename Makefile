output: gantt.o scheduling.o schedsimulation.o
	g++ gantt.o scheduling.o schedsimulation.o -o main

gantt.o: gantt.h gantt.cpp
	g++ -c gantt.cpp

scheduling.o: scheduling.h scheduling.cpp
	g++ -c scheduling.cpp

schedsimulation.o: schedsimulation.cpp
	g++ -c schedsimulation.cpp

clean:
	rm -f *.o
