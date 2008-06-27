all: fcstest fcsinfo fcsconvert fcstofchr fchrtofcs

fcstest: testFCS.cpp FCSIO.hpp FCSUtil.hpp
	g++ testFCS.cpp -O3 -o fcstest

fcsinfo: FCSIO.hpp FCSInfo.cpp FCSUtil.hpp
	g++ FCSInfo.cpp -O3 -o fcsinfo

fcsconvert: FCSIO.hpp FCSTools.hpp FCSUtil.hpp FCSConvert.cpp
	g++ FCSConvert.cpp -O3 -o fcsconvert

fcstofchr: FCSxToHR.cpp FCSTools.hpp FCSUtil.hpp FCSIO.hpp FCSHRIO.hpp
	g++ FCSxToHR.cpp -O3 -o fcstofchr

fchrtofcs: fcstofchr
	cp fcstofchr fchrtofcs

clean:
	rm -f fcstest fcsinfo fcsconvert fcstofchr fchrtofcs