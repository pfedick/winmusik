GTEST_DIR	= /usr/local/gtest-1.7.0
EXTRA_CFLAGS = 
INCLUDE	= -I. -I$(GTEST_DIR)/include
CFLAGS	= -ggdb -Wall  $(INCLUDE)  -I/usr/local/include  -I/usr/local/include -I/usr/local/include  -D_THREAD_SAFE -pthread -I/home/patrick/include $(EXTRA_CFLAGS)
LIBS	= -L../compile -lwinmusik3   -L/home/patrick/lib -lppl7 -lm -D_THREAD_SAFE -pthread -L/usr/lib -lz -lbz2 -L/usr/local/lib -lpcre /usr/local/lib/libiconv.so -Wl,-rpath -Wl,/usr/local/lib -L/usr/local/lib -lmcrypt -L/usr/local/lib -lmhash -ljpeg -L/usr/local/lib -lpng16 -lz -L /usr/local/lib -ltiff -llzma -L/usr/local/lib -lfreetype -L/usr/local/lib -lMagickWand-6 -lMagickCore-6 -L/usr/local/lib -lcurl -fstack-protector -L/usr/local/lib -lcrypto -lldns -L/usr/local/lib -lmicrohttpd -L/usr/local/lib -lidn -L/usr/lib -lssl -lcrypto -L/usr/local/lib/mysql -lmysqlclient -pthread -lz -lm -lexecinfo -lssl -lcrypto -L/usr/local/lib -lpq -L/usr/local/lib -lsqlite3 -L/usr/local/lib -lcdio -lm -lcam -L /usr/local/lib -lmpg123 -L/usr/local/lib -lmp3lame -lm -lstdc++ -lstdc++
LIBS_COVERAGE	= -L../compile -lwinmusik3   -L/home/patrick/lib -lppl7 -lm -D_THREAD_SAFE -pthread -L/usr/lib -lz -lbz2 -L/usr/local/lib -lpcre /usr/local/lib/libiconv.so -Wl,-rpath -Wl,/usr/local/lib -L/usr/local/lib -lmcrypt -L/usr/local/lib -lmhash -ljpeg -L/usr/local/lib -lpng16 -lz -L /usr/local/lib -ltiff -llzma -L/usr/local/lib -lfreetype -L/usr/local/lib -lMagickWand-6 -lMagickCore-6 -L/usr/local/lib -lcurl -fstack-protector -L/usr/local/lib -lcrypto -lldns -L/usr/local/lib -lmicrohttpd -L/usr/local/lib -lidn -L/usr/lib -lssl -lcrypto -L/usr/local/lib/mysql -lmysqlclient -pthread -lz -lm -lexecinfo -lssl -lcrypto -L/usr/local/lib -lpq -L/usr/local/lib -lsqlite3 -L/usr/local/lib -lcdio -lm -lcam -L /usr/local/lib -lmpg123 -L/usr/local/lib -lmp3lame -lm -lstdc++  -lgcov -lstdc++
PROGNAME = testsuite


OBJECTS = compile/main.o compile/storage.o

$(PROGNAME): compile/libgtest.a $(OBJECTS) wmlib-tests.h
	$(CXX) -O -ggdb -o $(PROGNAME) $(CFLAGS) $(OBJECTS) compile/libgtest.a $(LIBS)

$(PROGNAME)_coverage: compile/libgtest.a $(OBJECTS) wmlib-tests.h
	$(CXX) -O -ggdb -o $(PROGNAME)_coverage $(CFLAGS) $(OBJECTS) compile/libgtest.a $(LIBS_COVERAGE)


all: $(PROGNAME) $(PROGNAME)_coverage

coverage: $(PROGNAME)_coverage
	rm -rf tmp
	mkdir -p tmp
	./$(PROGNAME)_coverage

 
test: $(PROGNAME)
	rm -rf tmp
	mkdir -p tmp
	./$(PROGNAME)
	
xml: $(PROGNAME)
	rm -rf tmp
	mkdir -p tmp
	./$(PROGNAME) --gtest_output=xml
	
memcheck: $(PROGNAME)
	valgrind --tool=memcheck --leak-check=full --show-reachable=no --suppressions=valgrind.suppressions --xml=yes --xml-file=testsuite.valgrind ./$(PROGNAME)

clean:
	-rm -rf compile tmp
	-rm -rf *.core
	-rm -rf $(PROGNAME) $(PROGNAME)_coverage

compile/libgtest.a:
	mkdir -p compile
	$(CXX) -I${GTEST_DIR}/include -I${GTEST_DIR} -o compile/gtest-all.o -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv compile/libgtest.a compile/gtest-all.o


compile/main.o: src/main.cpp Makefile wmlib-tests.h
	mkdir -p compile
	$(CXX) $(CFLAGS) -o compile/main.o -c src/main.cpp

compile/storage.o: src/storage.cpp Makefile wmlib-tests.h
	mkdir -p compile
	$(CXX) $(CFLAGS) -o compile/storage.o -c src/storage.cpp

	
