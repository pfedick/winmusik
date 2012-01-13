CC      = gcc
EXTRA_CFLAGS = -D__UNIX__
INCLUDE = -I. -I/usr/local/include -I/local/include/mysql
CFLAGS  = -O $(INCLUDE) $(EXTRA_CFLAGS)
LIB     = -L/usr/local/lib -L/local/lib/mysql -lm -lppl4
PROGRAM = dlltest

OBJECTS = mp3.cpp dlltest.cpp

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(CFLAGS) $(LIB)
	-chmod 755 $(PROGRAM)

all:	$(PROGRAM)



