CC=g++
CFLAGS=-c -Wall -std=c++0x
LDFLAGS=
SOURCES=src/main.cc src/myutilities.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=rolling_median

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) src/*.o

