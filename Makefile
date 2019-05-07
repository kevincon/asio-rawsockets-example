#
#  Makefile 
# ~~~~~~~~~~~~~~~
#
#  Copyright (c) 2012 Kevin D. Conley (kcon at stanford dot edu)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
# 

all: example.cpp
	c++ -Wall -I -I/usr/local/Cellar/boost/1.69.0_2/include example.cpp -o example

clean: 
	rm example
