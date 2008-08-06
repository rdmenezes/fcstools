PP = g++
FLAGS = -O3 -I.

all: fcsinfo fcsconvert fcstofchr fchrtofcs py-fcs

fcstest: test/FCS.cpp fcs/io.hpp fcs/util.hpp
	${PP} test/FCS.cpp ${FLAGS} -o fcstest

fcsinfo: fcs/io.hpp source/info.cpp fcs/util.hpp
	${PP} source/info.cpp ${FLAGS} -o fcsinfo

fcsconvert: fcs/io.hpp fcs/tools.hpp fcs/util.hpp source/convert.cpp
	${PP} source/convert.cpp ${FLAGS} -o fcsconvert

fcstofchr: source/fcsxtohr.cpp fcs/tools.hpp fcs/util.hpp fcs/io.hpp fcs/hrio.hpp
	${PP} source/fcsxtohr.cpp ${FLAGS} -o fcstofchr

fchrtofcs: fcstofchr
	cp fcstofchr fchrtofcs

py-fcs: source/pyfcstools.cpp
	bjam toolset=darwin
	cp bin/darwin/debug/macosx-version-10.4/*.* .

test: fcstest
	./fcstest
	python test/fcstools.py

clean:
	rm -f fcstest fcsinfo fcsconvert fcstofchr fchrtofcs *.so *.o
	rm -rf bin