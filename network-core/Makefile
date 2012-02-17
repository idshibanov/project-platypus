all: server
cc=c++
server: server_main.o packet.o socket.o server_socket.o
	$(cc) server_main.o packet.o socket.o server_socket.o -o server
server_main.o: server_main.cpp datatypes.h defines.h
	$(cc) -c server_main.cpp
server_socket.o: server_socket.cpp server_socket.h
	$(cc) -c server_socket.cpp
packet.o: packet.cpp packet.h
	$(cc) -c packet.cpp
socket.o: socket.cpp socket.h
	$(cc) -c socket.cpp
client: client_main.o packet.o socket.o client_socket.o
	$(cc) client_main.o packet.o socket.o client_socket.o -o client
client_main.o: client_main.cpp datatypes.h defines.h
	$(cc) -c client_main.cpp
client_socket.o: client_socket.cpp client_socket.h
	$(cc) -c client_socket.cpp
clean:
	rm server_main.o client_main.o packet.o socket.o server_socket.o client_socket.o
