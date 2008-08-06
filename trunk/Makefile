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

py-fcs: pyfcstools.cpp
	bjam toolset=darwin
	cp bin/darwin/debug/macosx-version-10.4/*.* .

clean:
	rm -f fcstest fcsinfo fcsconvert fcstofchr fchrtofcs *.so *.o
	rm -rf bin