all: example.cpp
	g++ -Wall -I asio-1.4.8/include -I boost_1_50_0 example.cpp -o example

clean: 
	rm example
