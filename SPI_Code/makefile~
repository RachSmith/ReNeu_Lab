CC=gcc-4.4
LDFLAGS=-ldl
OBJECTS=Example.o NiFpga.o SPI.o
EXECUTABLE=Example

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)

Example.o: NiFpga_SPI_Test.h NiFpga.h SPI.h

NiFpga.o: NiFpga.h

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
