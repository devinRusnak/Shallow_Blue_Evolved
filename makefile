sb_e: Driver.o Game.o Blue.o Updater.o Stats.o Pieces.o Piece.o Move.o
	sudo g++ Driver.o Game.o Blue.o Updater.o Stats.o Pieces.o Piece.o Move.o -o sb_e

all: Driver.o Game.o Blue.o Updater.o Stats.o Pieces.o Piece.o Move.o
	sudo g++ Driver.o Game.o Blue.o Updater.o Stats.o Pieces.o Piece.o Move.o -o sb_e

Driver.o: Driver.cpp Game.h
	sudo g++ -c Driver.cpp

Game.o: Game.cpp Game.h Stats.h Pieces.h
	sudo g++ -c Game.cpp 

Blue.o: Blue.cpp Blue.h Pieces.h Move.h
	sudo g++ -c Blue.cpp

Updater.o: Updater.cpp Updater.h Move.h
	sudo g++ -c Updater.cpp

Stats.o: Stats.cpp Stats.h
	sudo g++ -c Stats.cpp

Piece.o: Piece.cpp Piece.h
	sudo g++ -c Piece.cpp

Pieces.o: Pieces.cpp Pieces.h Piece.h
	sudo g++ -c Pieces.cpp

Move.o: Move.cpp Move.h
	sudo g++ -c Move.cpp

clean:
	sudo rm *.o
	sudo rm sb_e
