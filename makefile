AIR: air

air: air-resistance.o
	g++ -lglut air-resistance.o -o air

air-resistance.o: air-resistance.cc
	g++ -c air-resistance.cc
