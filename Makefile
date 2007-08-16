all: Info Convert Test FCSxToHR

Test: testFCS.cpp FCSIO.hpp FCSUtil.hpp
	g++ testFCS.cpp -O3 -o fcstest

Info: FCSIO.hpp FCSInfo.cpp FCSUtil.hpp
	g++ FCSInfo.cpp -O3 -o fcsinfo

Convert: FCSIO.hpp FCSTools.hpp FCSUtil.hpp FCSConvert.cpp
	g++ FCSConvert.cpp -O3 -o fcsconvert

FCSxToHR: FCSxToHR.cpp FCSTools.hpp FCSUtil.hpp FCSIO.hpp FCSHRIO.hpp
	g++ FCSxToHR.cpp -O3 -o fcstofchr