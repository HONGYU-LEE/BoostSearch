FLAG=-std=c++11 -lpthread -ljsoncpp -lboost_system -L . -lsearcher

http_server:http_server.cpp
	g++ $^ -o $@ $(FLAG)

.PHONY:clean
clean:
	rm  http_server