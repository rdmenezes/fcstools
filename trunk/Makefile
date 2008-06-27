PP = g++
FLAGS = -O3 -I.

all: fcsinfo fcsconvert fcstofchr fchrtofcs

fcstest: testFCS.cpp FCSIO.hpp FCSUtil.hpp
	${PP} testFCS.cpp ${FLAGS} -o fcstest

fcsinfo: FCSIO.hpp FCSInfo.cpp FCSUtil.hpp
	${PP} FCSInfo.cpp ${FLAGS} -o fcsinfo

fcsconvert: FCSIO.hpp FCSTools.hpp FCSUtil.hpp FCSConvert.cpp
	${PP} FCSConvert.cpp ${FLAGS} -o fcsconvert

fcstofchr: FCSxToHR.cpp FCSTools.hpp FCSUtil.hpp FCSIO.hpp FCSHRIO.hpp
	${PP} FCSxToHR.cpp ${FLAGS} -o fcstofchr

fchrtofcs: fcstofchr
	cp fcstofchr fchrtofcs

clean:
	rm -f fcstest fcsinfo fcsconvert fcstofchr fchrtofcs