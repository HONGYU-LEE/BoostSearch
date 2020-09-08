FLAG=-std=c++11 -ljsoncpp -lboost_system

libsearcher.so:searcher.hpp ../index/index.hpp
	g++ searcher.hpp ../index/index.hpp -o $@ -shared -fPIC $(FLAG)

.PHONY:clean
clean:
	rm libsearcher.so
