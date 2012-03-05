all: server
cc=c++
BIN_DEST = ./dist
OBJ_DEST = ./dist
CORE_SRC = ./core
NET_SRC = ./net

# Binaries
server: server_main.o server.o packet.o socket.o server_socket.o
	$(cc) $(OBJ_DEST)/server_main.o $(OBJ_DEST)/server.o $(OBJ_DEST)/packet.o $(OBJ_DEST)/socket.o $(OBJ_DEST)/server_socket.o -o $(BIN_DEST)/server
client: client_main.o packet.o socket.o client_socket.o
	$(cc) $(OBJ_DEST)/client_main.o $(OBJ_DEST)/packet.o $(OBJ_DEST)/socket.o $(OBJ_DEST)/client_socket.o -o $(BIN_DEST)/client

# Server sources
server_main.o: server_main.cpp $(CORE_SRC)/defines.h
	$(cc) -o $(OBJ_DEST)/server_main.o -c server_main.cpp

server.o: server.cpp server.h
	$(cc) -o $(OBJ_DEST)/server.o -c server.cpp

# Client sources
client_main.o: client_main.cpp $(CORE_SRC)/defines.h
	$(cc) -o $(OBJ_DEST)/client_main.o -c client_main.cpp

# Network-core
client_socket.o: $(NET_SRC)/client_socket.cpp $(NET_SRC)/client_socket.h
	$(cc) -o $(OBJ_DEST)/client_socket.o -c $(NET_SRC)/client_socket.cpp

server_socket.o: $(NET_SRC)/server_socket.cpp $(NET_SRC)/server_socket.h
	$(cc) -o $(OBJ_DEST)/server_socket.o -c $(NET_SRC)/server_socket.cpp

packet.o: $(NET_SRC)/packet.cpp $(NET_SRC)/packet.h
	$(cc) -o $(OBJ_DEST)/packet.o -c $(NET_SRC)/packet.cpp

socket.o: $(NET_SRC)/socket.cpp $(NET_SRC)/socket.h
	$(cc) -o $(OBJ_DEST)/socket.o -c $(NET_SRC)/socket.cpp

# Delete objects
clean:
	rm $(OBJ_DEST)/*.o

