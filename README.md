# Calvin Vuong, Stephanie Yoon, Giovanni Topa: The Blazin Texans

## Texas Hold'em Poker

### List of Files in the Repository
* DESIGN.txt
* DEVLOG.txt
* algo.c
* algo.h
* client.c
* game_mec.c
* game_mec.h
* makefile
* networking.c
* networking.h
* server.c
* server.h

### Instructions
* Make.
* Run ./server.
* Run ./client on a different machine with the IP address of the server as the first command line argument.
* The game will automatically start on the client once at least 2 people are connected to the server.
* When it is your turn, options will show up (client side); enter the number that corresponds to the action you wish to take.
* You cannot manually exit the game; interrupting the client prematurely will result in undefined behavior
* When someone wins all of the money, all clients will automatically exit; you do not have to do anything.
* Quit the server my interrupting the process (Ctrl+C) when you are done testing and when you have no more clients waiting to connect.

### Notes
* Any clients connecting to the server after a game has started will be automatically queued to a "waitlist" of players wanting to play; once the current game finishes, the next 4 clients will begin gameplay in a few seconds.
* If all the other players have folded or went all in, then the remaining player has to keep checking until he gets to the end and wins.
* If absolutely everyone folds, then by default, the zeroth player gets all the mula.
* Do not type stuff into the client terminal unless it is your turn and you are prompted to do so.
* The server can be effectively ignored during gameplay; no user interactions take place there. 
* Have fun! Don't get addicted to gambling; it ruins lives.