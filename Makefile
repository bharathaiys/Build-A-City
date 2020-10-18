risingCity: risingCity.o
	g++ risingCity.o -o risingCity
risingCity.o: risingCity.cpp
		g++ -c risingCity.cpp
