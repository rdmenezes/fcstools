all: Info Convert

FCS2: testFCS.cpp FCSIO.hpp
	g++ testFCS.cpp -O3 -o fcstest

Info: FCSIO.hpp FCSInfo.cpp
	g++ FCSInfo.cpp -O3 -o fcsinfo

Convert: FCSIO.hpp FCSTools.hpp FCSConvert.cpp
	g++ FCSConvert.cpp -O3 -o fcsconvert