CC      = gcc
EXTRA_CFLAGS = -D__UNIX__
INCLUDE = -I. -I/local/include
CFLAGS  = -O $(INCLUDE) $(EXTRA_CFLAGS)
LIB     = -L/local/lib -lppl4 -lm
PROGRAM = wmt2file
TARGETBIN=/usr/local/bin

OBJECTS = main.cpp

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(CFLAGS) $(LIB)
	-chmod 755 $(PROGRAM)

all:    $(PROGRAM)

bsd:
	$(CC) -o $(PROGRAM) $(OBJECTS) $(CFLAGS) $(LIB) 
	-chmod 755 $(PROGRAM)

sun4u:
	$(CC) -o $(PROGRAM) $(OBJECTS) $(CFLAGS) $(LIB) -D__SOLARIS__ 
	-chmod 755 $(PROGRAM)

install:
	-cp $(PROGRAM) $(TARGETBIN)
clean:
	-rm *.o $(PROGRAM)
