
#g++ main.cpp cmd.cpp word.cpp -o wkb -I /usr/local/include/boost_1_57_0 -g
wkb: main.cpp cmd.cpp word.cpp 
	g++ main.cpp cmd.cpp word.cpp -o wkb -I /usr/local/include/boost_1_57_0 -g --std=c++11

clean:
	rm -f wkb

.PHONY:
	clean

