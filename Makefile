scheduling.o: scheduling.cpp scheduling.h
	g++ -c scheduling.cpp

clean:
	rm -f *.o
