FLAG=-std=c++11 -I ~/third_part/include -ljsoncpp -lboost_system

index_test:index_test.cpp
	g++ $^ -o $@ $(FLAG)

.PHONY:clean
clean:
	rm index_test
