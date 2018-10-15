CXXFLAGS =	-O3 -ggdb -Wall -fmessage-length=0  -no-pie  -I/usr/local/include  -pthread -I/home/patrickf/include -Iinclude 
LIBS = -no-pie   -L/home/patrickf/lib -lppl7 -lm -lresolv -lresolv -lresolv -lresolv -lresolv -lresolv -lresolv -pthread -L/usr/lib -lz -lbz2 -lpcre -L/usr/lib -lmcrypt -lmhash -ljpeg -lresolv -lresolv -lresolv -lresolv -lresolv -lresolv -lresolv -L/home/patrickf/lib -lpng -lz -lm -L /usr/lib -ltiff -llzma -L/home/patrickf/lib -Wl,-rpath,/home/patrickf/lib -Wl,--enable-new-dtags -lSDL2 -Wl,--no-undefined -lm -ldl -lsndio -lpthread -lrt -lfreetype -lcurl -L/usr/local/lib -lmicrohttpd -lidn -L/usr/lib -lssl -lcrypto -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -latomic -ldl -L/usr/lib/x86_64-linux-gnu -lpq -L/usr/lib -lsqlite3 -lmpg123 -lmp3lame -lm -lstdc++  -lstdc++
LIBDEP	:= $(shell /home/patrickf/lib/libppl7.a)

OBJS =		compile/object_title.o \
			compile/CStorage.o \
			compile/CDatabase.o \
			compile/Playlist.o \
			compile/Searchlist.o \
			compile/MusicalKeys.o \
			
			
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
	
