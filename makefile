compile: server.h server.c networking.h networking.c game_mec.h game_mec.c
	gcc server.h
	gcc networking.h
	gcc game_mec.h
	gcc -c networking.c
	gcc -c game_mec.c
	gcc -o server networking.o game_mec.o
	gcc -o client networking.o game_mec.o
