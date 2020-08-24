FLAG =-std=c++11 -lboost_filesystem -lboost_system

parser:parser.cpp
	g++ $^ -o $@ $(FLAG)

.PHONY:clean
clean:
	rm parser
