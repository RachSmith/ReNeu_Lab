CC=gcc-4.4
LDFLAGS=-ldl
OBJECTS=Example.o NiFpga.o
EXECUTABLE=Example

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)

Example.o: NiFpga_CTest.h NiFpga.h

NiFpga.o: NiFpga.h

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
