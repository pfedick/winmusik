CXXFLAGS =	-O3 -ggdb -Wall -fmessage-length=0 `ppl7-config --cflags release` -Iinclude 
LIBS = -L/usr/local/lib `ppl7-config --libs release`  -lstdc++
LIBDEP	:= $(shell ppl7-config --ppllib release)

OBJS =		compile/object_title.o \
			compile/storage.o compile/database.o

TARGET =	compile/libwinmusik3.a

$(TARGET):	$(OBJS) $(LIBDEP)
			ar -r $(TARGET) $(OBJS)

all:		$(TARGET)

clean:
	rm -rf compile $(TARGET)


	
compile/object_title.o: src/object_title.cpp include/libwinmusik3.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/object_title.o -c src/object_title.cpp $(CXXFLAGS)
	
compile/storage.o: src/storage.cpp include/libwinmusik3.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/storage.o -c src/storage.cpp $(CXXFLAGS)

compile/database.o: src/database.cpp include/libwinmusik3.h Makefile
	-mkdir -p compile
	$(CXX) -o compile/database.o -c src/database.cpp $(CXXFLAGS)
	