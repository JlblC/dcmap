source_files= stdafx.cpp PlatformPosix.cpp dcmapsys.cpp \
DCMapCore.cpp PlanetDataStorage.cpp geostorage.cpp \
secretstorage.cpp BlobStorage.cpp BinReflection.cpp \
MapColoringTable.cpp DCMapConfig.cpp LocalStrings.cpp \
b64cdecode.o b64cencode.o


object_files=$(patsubst   %.cpp, %.o, $(source_files))

#-fvisibility=hidden
CXXFLAGS= -MMD -I ../../gsyslib  -fPIC -O3
CFLAGS= -MMD -I ../../gsyslib  -fPIC -O3

CXX=g++
LDFLAGS =  -Wl,--version-script=dcmapsys.so.map -liconv -static-libgcc \
-Wl,"-Bstatic" \
-lz \
-lstdc++ -Wl,"-Bdynamic" -lm -lm -Wl,"-Bstatic" \
-lgcc \
-Wl,"-Bdynamic" \
-lc -lc \
-Wl,"-Bstatic" \
-lgcc 


#-Wl,-Bstatic -Wl,-lgcc -Wl,-lstdc++ -Wl,-lcxx -Wl,-Bdynamic -Wl,-lc

.PHONY: libtest dcmapsys clean install all

all: libtest dcmapsys 

#%.o: %.c
#	${CXX} -c $< ${CXXFLAGS} -o $@ 


../dcmapsys.so: $(object_files)
	${CXX} $^ ${CXXFLAGS} -shared -fPIC -L. ${LDFLAGS} -Wl,-soname,$@ -o $@ 
	strip -x $@ 

dcmapsys: ../dcmapsys.so

libtest: dcmapsys
	$(MAKE) -C ../libtest

clean:
	-rm  *~ *.o *.d *.so
	-rm ../dcmapsys.so
	$(MAKE) clean -C ../libtest

install: all
	

-include *.d