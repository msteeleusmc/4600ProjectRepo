#----------------------------------------
#
# This is a makefile for proj. 01
#
# Authored by:	Michael Steele
# UNT ID:	11198872
# Date:		03/22/2020
#
#----------------------------------------

all: main.o
	g++ main.o -lpthread -lm -lrt -o all

main.o: main.cpp
	g++ -c main.cpp -lpthread -lm -lrt

run:
	./all

clean:
	rm -f *.o
	rm ./all
