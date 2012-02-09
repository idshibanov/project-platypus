all: server-base
cc=c++
server-base: server-base.o packet.o socket.o
	$(cc) server-base.o packet.o socket.o -o server-base
server-base.o: server-base.cpp datatypes.h defines.h
	$(cc) -c server-base.cpp
packet.o: packet.cpp packet.h
	$(cc) -c packet.cpp
socket.o: socket.cpp socket.h
	$(cc) -c socket.cpp
client: client-base.o packet.o socket.o
	$(cc) client-base.o packet.o socket.o -o client-base
client-base.o: client-base.cpp
	$(cc) -c client-base.cpp	
clean:
	rm server-base.o client-base.o packet.o socket.o
