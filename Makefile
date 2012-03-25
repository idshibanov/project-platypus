all: clean dist server nclient remove_obj
cc=c++
BIN_DEST = ./dist
OBJ_DEST = ./dist
CORE_SRC = ./core
NET_SRC = ./net
NC_SRC = ./ncurses
SERVER_SRC = ./server
GAME_SRC = ./game

# Binaries
server: server_main.o server.o packet.o socket.o server_socket.o game.o floor.o player.o
	$(cc) $(OBJ_DEST)/server_main.o $(OBJ_DEST)/server.o $(OBJ_DEST)/packet.o $(OBJ_DEST)/socket.o \
	$(OBJ_DEST)/server_socket.o $(OBJ_DEST)/game.o $(OBJ_DEST)/floor.o $(OBJ_DEST)/player.o        \
	$(shell mysql_config --libs) -o $(BIN_DEST)/server

nclient: nclient.o packet.o socket.o client_socket.o GameScreen.o MainMenu.o LoginMenu.o RegisterMenu.o SettingsMenu.o ScoresMenu.o GameMenu.o Prompt.o
	$(cc) -g $(OBJ_DEST)/nclient.o $(OBJ_DEST)/MainMenu.o $(OBJ_DEST)/SettingsMenu.o $(OBJ_DEST)/Prompt.o $(OBJ_DEST)/ScoresMenu.o \
    $(OBJ_DEST)/RegisterMenu.o $(OBJ_DEST)/LoginMenu.o $(OBJ_DEST)/GameMenu.o $(OBJ_DEST)/packet.o $(OBJ_DEST)/socket.o $(OBJ_DEST)/client_socket.o $(OBJ_DEST)/GameScreen.o -lncurses -o $(BIN_DEST)/nclient

# Server sources
server_main.o: $(SERVER_SRC)/server_main.cpp $(CORE_SRC)/defines.h
	$(cc) -o $(OBJ_DEST)/server_main.o $(shell mysql_config --cflags) -c $(SERVER_SRC)/server_main.cpp

server.o: $(SERVER_SRC)/server.cpp $(SERVER_SRC)/server.h
	$(cc) -o $(OBJ_DEST)/server.o $(shell mysql_config --cflags) -c $(SERVER_SRC)/server.cpp

database.o: $(SERVER_SRC)/database.cpp $(SERVER_SRC)/database.h
	$(cc) -o $(OBJ_DEST)/database.o $(shell mysql_config --cflags) -c $(SERVER_SRC)/database.cpp

# Game sources
game.o: $(GAME_SRC)/game.cpp $(GAME_SRC)/game.h
	$(cc) -o $(OBJ_DEST)/game.o -c $(GAME_SRC)/game.cpp

floor.o: $(GAME_SRC)/floor.cpp $(GAME_SRC)/floor.h
	$(cc) -o $(OBJ_DEST)/floor.o -c $(GAME_SRC)/floor.cpp

player.o: $(GAME_SRC)/player.cpp $(GAME_SRC)/player.h
	$(cc) -o $(OBJ_DEST)/player.o -c $(GAME_SRC)/player.cpp

# Client sources	
nclient.o: $(NC_SRC)/nclient.cpp $(NC_SRC)/nclient.h
	$(cc) -g -o $(OBJ_DEST)/nclient.o -c $(NC_SRC)/nclient.cpp

GameScreen.o: $(NC_SRC)/game/GameScreen.cpp $(NC_SRC)/game/GameScreen.h
	$(cc) -o $(OBJ_DEST)/GameScreen.o -c $(NC_SRC)/game/GameScreen.cpp

MainMenu.o: $(NC_SRC)/menu/MainMenu.cpp $(NC_SRC)/menu/MainMenu.h
	$(cc) -o $(OBJ_DEST)/MainMenu.o -c $(NC_SRC)/menu/MainMenu.cpp

LoginMenu.o: $(NC_SRC)/menu/LoginMenu.cpp $(NC_SRC)/menu/LoginMenu.h
	$(cc) -o $(OBJ_DEST)/LoginMenu.o -c $(NC_SRC)/menu/LoginMenu.cpp

RegisterMenu.o: $(NC_SRC)/menu/RegisterMenu.cpp $(NC_SRC)/menu/RegisterMenu.h
	$(cc) -o $(OBJ_DEST)/RegisterMenu.o -c $(NC_SRC)/menu/RegisterMenu.cpp

SettingsMenu.o: $(NC_SRC)/menu/SettingsMenu.cpp $(NC_SRC)/menu/SettingsMenu.h
	$(cc) -o $(OBJ_DEST)/SettingsMenu.o -c $(NC_SRC)/menu/SettingsMenu.cpp

ScoresMenu.o: $(NC_SRC)/menu/ScoresMenu.cpp $(NC_SRC)/menu/ScoresMenu.h
	$(cc) -o $(OBJ_DEST)/ScoresMenu.o -c $(NC_SRC)/menu/ScoresMenu.cpp

GameMenu.o: $(NC_SRC)/menu/GameMenu.cpp $(NC_SRC)/menu/GameMenu.h
	$(cc) -o $(OBJ_DEST)/GameMenu.o -c $(NC_SRC)/menu/GameMenu.cpp

Prompt.o: $(NC_SRC)/tools/Prompt.cpp $(NC_SRC)/tools/Prompt.h
	$(cc) -o $(OBJ_DEST)/Prompt.o -c $(NC_SRC)/tools/Prompt.cpp

# Network-core
client_socket.o: $(NET_SRC)/client_socket.cpp $(NET_SRC)/client_socket.h
	$(cc) -o $(OBJ_DEST)/client_socket.o -c $(NET_SRC)/client_socket.cpp

server_socket.o: $(NET_SRC)/server_socket.cpp $(NET_SRC)/server_socket.h
	$(cc) -o $(OBJ_DEST)/server_socket.o $(shell mysql_config --cflags) -c $(NET_SRC)/server_socket.cpp

packet.o: $(NET_SRC)/packet.cpp $(NET_SRC)/packet.h
	$(cc) -o $(OBJ_DEST)/packet.o -c $(NET_SRC)/packet.cpp

socket.o: $(NET_SRC)/socket.cpp $(NET_SRC)/socket.h
	$(cc) -o $(OBJ_DEST)/socket.o -c $(NET_SRC)/socket.cpp
	
dist:
	@@mkdir -p ${OBJ_DEST}

# Delete objects
clean:
	rm -rf $(OBJ_DEST)
remove_obj:
	rm -r $(OBJ_DEST)/*.o

