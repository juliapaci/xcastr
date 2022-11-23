# Compiler flags
CFLAGS = -lX11 -lm -lXfixes -g -Wall

# Build Target
TARGET = XCastr

# Binary location
BIN = /usr/bin

all: $(TARGET)

$(TARGET): $(TARGET).c
	gcc $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm -f $(TARGET) $(objects)

install: all
	mkdir -p $(BIN)
	cp -f $(TARGET) $(BIN)
	chmod 775 $(BIN)/$(TARGET)
	# TODO: man page

uninstall:
	rm -f $(BIN)/$(TARGET)
