CXXFLAGS =	-O3 -ggdb -Wall -fmessage-length=0  -I/usr/local/include  -I/usr/local/include -I/usr/local/include  -D_THREAD_SAFE -pthread -I/home/patrick/include -Iinclude 
LIBS = -no-pie   -L/home/patrick/lib -lppl7 -lm -D_THREAD_SAFE -pthread -L/usr/lib -lz -lbz2 -L/usr/local/lib -lpcre /usr/local/lib/libiconv.so -Wl,-rpath -Wl,/usr/local/lib -L/usr/local/lib -lmcrypt -L/usr/local/lib -lmhash -ljpeg -L/usr/local/lib -lpng16 -lz -L /usr/local/lib -ltiff -llzma -L/usr/local/lib -lSDL2 -L/usr/local/lib -lfreetype -L/usr/local/lib -lMagickWand-6 -lMagickCore-6 -L/usr/local/lib -lcurl -fstack-protector -L/usr/local/lib -lcrypto -lldns -L/usr/local/lib -lmicrohttpd -L/usr/local/lib -lidn -L/usr/local/lib -lssl -lcrypto -L/usr/local/lib/mysql -lmysqlclient -pthread -lz -lm -lexecinfo -lssl -lcrypto -L/usr/local/lib -lpq -L/usr/local/lib -lsqlite3 -L/usr/local/lib -lcdio -lm -lcam -L /usr/local/lib -lmpg123 -L/usr/local/lib -lmp3lame -lm -lstdc++  -lstdc++
LIBDEP	:= $(shell /home/patrick/lib/libppl7.a)

OBJS =		compile/object_title.o \
			compile/CStorage.o \
			compile/CDatabase.o \
			compile/Playlist.o \
			compile/Searchlist.o \
			compile/MusicalKeys.o \
			compile/traktor.o \
			
			#compile/CWMFile.o

TARGET =	compile/libwinmusik3.a

$(TARGET):	$(OBJS) $(LIBDEP)
			ar -r $(TARGET) $(OBJS)

all:		$(TARGET)

clean:
	rm -rf compile $(TARGET)


	
compile/object_title.o: src/object_title.cpp include/libwinmusik3.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/object_title.o -c src/object_title.cpp $(CXXFLAGS)
	
compile/CStorage.o: src/CStorage.cpp include/libwinmusik3.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/CStorage.o -c src/CStorage.cpp $(CXXFLAGS)

compile/CDatabase.o: src/CDatabase.cpp include/libwinmusik3.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/CDatabase.o -c src/CDatabase.cpp $(CXXFLAGS)

compile/CWMFile.o: src/CWMFile.cpp include/libwinmusik3.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/CWMFile.o -c src/CWMFile.cpp $(CXXFLAGS)

compile/Playlist.o: src/Playlist.cpp include/wm_playlist.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/Playlist.o -c src/Playlist.cpp $(CXXFLAGS)

compile/Searchlist.o: src/Searchlist.cpp include/wm_searchlist.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/Searchlist.o -c src/Searchlist.cpp $(CXXFLAGS)

compile/MusicalKeys.o: src/MusicalKeys.cpp include/wm_musicalkeys.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/MusicalKeys.o -c src/MusicalKeys.cpp $(CXXFLAGS)

compile/traktor.o: src/traktor.cpp include/traktor.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/traktor.o -c src/traktor.cpp $(CXXFLAGS)
	
