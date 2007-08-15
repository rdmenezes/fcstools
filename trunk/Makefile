all: Info Convert

FCS2: testFCS.cpp FCS2.hpp
	g++ testFCS.cpp -O3 -o fcstest

Info: FCS2.hpp FCSInfo.cpp
	g++ FCSInfo.cpp -O3 -o fcsinfo

Convert: FCS2.hpp FCSConvert.cpp
	g++ FCSConvert.cpp -O3 -o fcsconvert