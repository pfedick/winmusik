CXXFLAGS =	-O3 -ggdb -Wall -fmessage-length=0 `ppl7-config --cflags release` -Iinclude 
LIBS = -L/usr/local/lib `ppl7-config --libs release`  -lstdc++
LIBDEP	:= $(shell ppl7-config --ppllib release)

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
	
