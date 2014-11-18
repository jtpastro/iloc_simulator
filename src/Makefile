CC=g++
CFLAGS=-c -g -Wall -std=c++11
LDFLAGS=
SOURCES=Main.cpp Operation.cpp Program.cpp Machine.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=iloc

all: $(MAIN) $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o test
