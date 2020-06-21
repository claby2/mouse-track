run:
	@g++ -c -lSDl2main -lSDL2 common.c
	@g++ -c cli.c
	@g++ -c -lSDL2main -lSDL2 heatmap.c
	@g++ -c -lSDl2main -lSDL2 source.cpp
	@g++ common.o cli.o heatmap.o source.o -lSDl2main -lSDL2 -o source && source.exe
main:
	@g++ -c -lSDl2main -lSDL2 common.c
	@g++ -c cli.c
	@g++ -c -lSDL2main -lSDL2 heatmap.c
	@g++ -c -lSDl2main -lSDL2 source.cpp
	@g++ common.o cli.o heatmap.o source.o -lSDL2main -lSDL2 -o source
playback:	
	@g++ -c -lSDl2main -lSDL2 common.c
	@g++ -c cli.c
	@g++ -c -lSDL2main -lSDL2 heatmap.c
	@g++ -c -lSDl2main -lSDL2 playback.cpp
	@g++ common.o cli.o heatmap.o playback.o -lSDL2main -lSDL2 -o playback && playback.exe