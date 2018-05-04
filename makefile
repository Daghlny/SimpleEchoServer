
UtilFiles = Tools.cpp Tools.hpp

ServerImphFiles = Server.cpp ServerMain.cpp Tools.cpp
ServerFiles = Server.hpp Server.cpp ServerMain.cpp

ClientImphFiles = Client.cpp ClientMain.cpp Tools.cpp
ClientFiles = Client.hpp Client.cpp ClientMain.cpp

CPPFLAGS = -std=c++11 -O0

server: $(ServerFiles) $(UtilFiles)
	g++ $(CPPFLAGS) $(ServerImphFiles) -lpthread -o server

client: $(ClientFiles) $(UtilFiles)
	g++ $(CPPFLAGS) $(ClientImphFiles) -o client

clean: server client
	rm -f server client

