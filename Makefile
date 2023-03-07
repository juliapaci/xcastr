# Compiler flags
CFLAGS = -lX11 -lXext -lXtst -lXfixes -g -Wall

# Build Target
TARGET = XCastr

# Binary location
BIN = /usr/bin

$(TARGET): $(TARGET).c WindowFunctions.c config.h
	gcc $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm -f $(TARGET) $(objects)

install: $(TARGET)
	mkdir -p $(BIN)
	cp -f $(TARGET) $(BIN)
	chmod 775 $(BIN)/$(TARGET)
	# TODO: man page?

uninstall:
	rm -f $(BIN)/$(TARGET)
