.PHONY run main
run:
	g++ source.cpp -w -lSDl2main -lSDL2 -o source && source.exe
main:
	g++ source.cpp -w -lSDL2main -lSDL2 -o source