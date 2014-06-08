source_files= libtest.cpp 

CXX=g++
CXXFLAGS= -MMD -I ../../../gsyslib
LDFLAGS = ../dcmapsys.so


libtest: $(patsubst   %.cpp, %.o, $(source_files))
	${CXX} $< ${CXXFLAGS} ${LDFLAGS} -o $@ 

clean:
	-rm  *~ *.o *.d *.so

-include *.d