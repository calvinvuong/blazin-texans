compile: server.h server.c networking.h networking.c game_mec.h game_mec.c
	gcc server.h
	gcc networking.h
	gcc game_mec.h
	gcc algo.h
	gcc -c networking.c
	gcc -c game_mec.c
	gcc -c algo.c
	gcc -c client.c
	gcc -c server.c
	gcc -o server server.o networking.o game_mec.o algo.o
	gcc -o client client.o networking.o game_mec.o algo.o
